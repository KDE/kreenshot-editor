/*
 * This file is part of the kreenshot-editor project.
 *
 * Copyright (C) 2014 by Gregor Mi <codestruct@posteo.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 or the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */
#include "kreenshoteditor.h"
#include <QImage>
#include <QScrollArea>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QApplication>
#include <QClipboard>
#include <QDir>
#include <QStringList>
#include <QDragEnterEvent>
#include <QMimeData>
#include "ui/maineditorwidget.h"
#include "core/document.h"
#include "core/documentfile.h"
#include "core/impl/outputfilenamegenerator.h"
#include "core/settingsmanager.h"
#include "ui/settings/preferencesdialog.h"
#include "ui/impl/kreengraphicsscene.h"

namespace kreen {

#define tr(arg) QObject::tr(arg)

class KreenshotEditor::Impl
{
public:
    Impl(KreenshotEditor* owner)
    {
        _owner = owner;
        outputFilenameGenerator = OutputFilenameGenerator::make_shared();
        settingsManager = SettingsManager::instance();
    }

    /**
     * Creates a new document.
     * Set the base image data (image where you can add items) from the given parameter.
     * If there is no filename given the file is treated as new;
     *   else the given filename is associated with the document.
     * If no image data is given a default image will be used as base image.
     */
    void createNewDocument(QImage image, QString associatedFilename)
    {
        qDebug() << "KreenshotEditor::createNewDocument internal";

        settingsToOutputFilenameGenerator();

        QString filename;
        DocumentFile::FileStatus fileStatus;
        if (!associatedFilename.isEmpty()) { // file was loaded from disk
            fileStatus = DocumentFile::FileStatus_CreatedAndClean;
            filename = associatedFilename;
        }
        else {
            fileStatus = DocumentFile::DocumentFile::FileStatus_NotCreated;
            filename = outputFilenameGenerator->resultingFilename();
        }

        auto doc = Document::make_shared(image);
        scene = kreen::ui::KreenGraphicsScene::make_shared(); // scene is part of the Impl (as well as documentFile which will hold the document)
        scene->setDocument(doc); // for ObfuscateTool at the moment (todo: refac?)
        //
        // used to make "QImage Document::renderToImage()" (see there) work
        // (because we do not want the KreenGraphicsView be referenced by the Document):
        //
        connect(doc.get(), SIGNAL(requestRenderToImageSignal(kreen::core::Document*)),
                scene.get(), SLOT(slotRequestRenderToImage(kreen::core::Document*)));

        documentFile = std::make_shared<DocumentFile>(doc, filename, fileStatus);
        documentFile->setSettingsManager(settingsManager);

        _owner->connect(_owner->documentFile().get(), SIGNAL(fileStatusChanged()), _owner, SLOT(slotDocumentFileStatusChanged()));
        _owner->connect(_owner->document().get(), SIGNAL(contentChangedSignal()), _owner, SLOT(slotEditUndoRedoActionEnabledUpdate()));
        _owner->connect(_owner->document().get(), SIGNAL(contentChangedSignal()), _owner, SLOT(slotDocumentFileStatusChanged()));

        _owner->emit newDocumentCreatedSignal();
        _owner->slotDocumentFileStatusChanged(); // emits documentFileStatusChangedSignal() and set some action enabledness (save, undo, redo)
    }

    void settingsToOutputFilenameGenerator()
    {
        auto outputSettings = settingsManager->output;
        //                                                                                     == PathCombine
        outputFilenameGenerator->setFilenamePattern(QDir(outputSettings.defaultOutputDirectory).filePath(outputSettings.filenamePattern));
    }

    QAction* newToolAction(QString toolId, QIcon icon, QString text, QObject* parent, QKeySequence key)
    {
        auto action = new QAction(icon, text, parent);
        action->setCheckable(true);
        action->setShortcut(key);
        action->setData(toolId);
        action->setToolTip(text); // todo: more
        return action;
    }

    /**
     * creates an action and adds it to the actionIdToActionMap
     */
    QAction* newAction(QString actionId, QIcon icon, QString text, QObject* parent, QKeySequence key)
    {
        auto action = new QAction(icon, text, parent);
        action->setShortcut(key);

        text.replace("&", ""); // remove any mnemnic stuff
        action->setToolTip(text); // todo: more and detailled tooltips

        action->setData(actionId);

        actionIdToActionMap.insert(actionId, action);

        return action;
    }

    /**
     * singleton
     */
    QList<QAction*> toolActions()
    {
        if (toolActionGroup == nullptr) {
            toolActionGroup = new QActionGroup(_owner);
            newToolAction("select", QIcon::fromTheme("edit-select"), tr("Select"), toolActionGroup, QKeySequence(tr("Esc")));
            newToolAction("rect", QIcon::fromTheme("draw-rectangle"), tr("Rectangle"), toolActionGroup, QKeySequence(tr("R")));
            newToolAction("ellipse", QIcon::fromTheme("draw-circle"), tr("Ellipse or circle"), toolActionGroup, QKeySequence(tr("E")));
            newToolAction("line", QIcon::fromTheme("draw-arrow-forward"), tr("Line or arrow"), toolActionGroup, QKeySequence(tr("L")));
            newToolAction("text", QIcon::fromTheme("draw-text"), tr("Text"), toolActionGroup, QKeySequence(tr("T")));
            newToolAction("highlight", QIcon::fromTheme("im-status-message-edit"), tr("Highlight"), toolActionGroup, QKeySequence(tr("H")));
            newToolAction("obfuscate", QIcon::fromTheme("edit-delete-shred"), tr("Obfuscate"), toolActionGroup, QKeySequence(tr("O")));
            newToolAction("op-crop", QIcon::fromTheme("transform-crop"), tr("Crop"), toolActionGroup, QKeySequence(tr("Ctrl+Shift+X")));
            newToolAction("op-ripout", QIcon::fromTheme("distribute-vertical-equal"), tr("Rip out"), toolActionGroup, QKeySequence(tr("Ctrl+Shift+R")));
        }

        return toolActionGroup->actions();
    }

    void handleSaveImageError(QWidget* parent, ErrorStatus errorStatus)
    {
        if (!errorStatus.isEmpty()) {
            QMessageBox::warning(parent, parent->tr("Error saving image"), errorStatus);
        }
    }

    QString saveFileNameFilterStringFromImageFileExtensions(QStringList fileextlist)
    {
        QString result;
        foreach (QString str, fileextlist)
        {
            result += "*." + str + " ";
        }
        return result;
    }

    bool askUserOnUnsavedData_handleAnswer_shouldContinue()
    {
        int response;
        if (_owner->isDocumentFileNotCreated()) {
            response = QMessageBox::information(_owner->mainEditorWidget(), _owner->tr("Document modified / No file for document"),
                                                QString(_owner->tr("The current document is not saved to a file.\nYou can save it to the predefined filename now, discard the changes or cancel the current action.")),
                                                QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                                                QMessageBox::Save);
        }
        else if (!_owner->document()->isClean()) {
            response = QMessageBox::information(_owner->mainEditorWidget(), _owner->tr("Document modified"),
                                                QString(_owner->tr("The current document changes are not saved to a file.\nYou can save it now, discard the changes or cancel the current action.")),
                                                QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                                                QMessageBox::Save);
        }
        else {
            return true; // continue
        }

        if (QMessageBox::Save == response) {
            _owner->slotDocumentSave();
            return true; // continue
        }
        else if (QMessageBox::Discard == response) {
            // If we do not set the document clean, it will trigger some cleanChanged signal which
            // will crash the program if it was closed by the user.
            // Thus, we set it clean which also makes some sense because we are about to willingly discard it.
            _owner->document()->setClean();
            return true; // continue
        }
        else {
            return false; // abort calling action
        }
    }

public:
    DocumentFilePtr documentFile;
    KreenGraphicsScenePtr scene;
    OutputFilenameGeneratorPtr outputFilenameGenerator;
    SettingsManagerPtr settingsManager;

    MainEditorWidget* mainEditorWidget = nullptr;

    // document and file actions:
    QAction* actionDocumentNew;
    QAction* actionDocumentOpen;
    QAction* actionDocumentSave;
    QAction* actionDocumentSaveAs;
    QAction* actionDocumentCopyImageToClipboard;
    QAction* actionDocumentCopyFilenameToClipboard;
    QAction* actionDocumentSelectInFilemanager;
    QAction* actionDocumentOpenWithDefaultImageViewer;
    // edit actions:
    QAction* actionEditSelectAll;
    QAction* actionEditItemDelete;
    QAction* actionEditUndo;
    QAction* actionEditRedo;
    // tool actions:
    QActionGroup* toolActionGroup = nullptr;
    // all actions:
    QMap<QString, QAction*> actionIdToActionMap;

private:
    KreenshotEditor* _owner = nullptr;
};

#undef tr

KreenshotEditor::KreenshotEditor()
{
    KREEN_PIMPL_INIT_THIS(KreenshotEditor);

    d->settingsManager->load();

    allActionIds(); // init the action map

    // connect tool actions
    foreach (auto action, d->toolActions()) {

        // select action to tool request
        connect(action, SIGNAL(triggered()), this, SLOT(slotRequestToolBySenderAction()));
    }
}

KreenshotEditor::~KreenshotEditor()
{

}

void KreenshotEditor::dragEnterEventMainWindow(QDragEnterEvent* event)
{
    qDebug() << "KreenshotEditor::dragEnterEventMainWindow";
    if (event->mimeData()->hasImage() || event->mimeData()->hasText()) {
        event->acceptProposedAction();
    }
}

void KreenshotEditor::dropEventMainWindow(QDropEvent* event)
{
    if (!d->askUserOnUnsavedData_handleAnswer_shouldContinue()) {
        return;
    }

    if (event->mimeData()->hasImage()) {
        createNewDocument(qvariant_cast<QImage>(event->mimeData()->imageData()));
    }
    else if (event->mimeData()->hasText()) {
        qDebug() << event->mimeData()->text();
        QUrl url(event->mimeData()->text());
        createNewDocumentFromFile(url.toLocalFile()); // TODO: support also http images
    }
}

QString KreenshotEditor::actionToToolId(QAction* action)
{
    QString toolId = action->data().toString();
    Q_ASSERT_X(!toolId.isEmpty(), "actionToToolId", "Hint: setupActions must be called once before using this method");
    return toolId;
}

QString KreenshotEditor::actionToActionId(QAction* action)
{
    QString actionId = action->data().toString();
    Q_ASSERT_X(!actionId.isEmpty(), "actionToActionId", "Hint: setupActions must be called once before using this method");
    return actionId;
}

void KreenshotEditor::slotRequestToolBySenderAction()
{
    QString className = QObject::sender()->metaObject()->className();
    qDebug() << className;
    Q_ASSERT(className == "QAction");

    QString toolId = actionToToolId((QAction*)QObject::sender());
    QString message = QString("MainWindow::requestTool: tool id '%1'").arg(toolId);
    qDebug() << message;

    mainEditorWidget()->requestTool(toolId);
}

void KreenshotEditor::createNewDocument(QImage image/*, QString filename*/)
{
    //qDebug() << "KreenshotEditor::createNewDocument";
    d->outputFilenameGenerator->initCaptureTime(); // set time to current for new document (with or without default image; todo: this might change in the future)
    d->createNewDocument(image, QString());
}

void KreenshotEditor::createNewDocumentFromFile(QString filename)
{
    qDebug() << "KreenshotEditor::createNewDocumentFromFile";
    d->outputFilenameGenerator->initCaptureTime(); // set time to current for new document from file
    d->createNewDocument(QImage(filename), filename);
}

void KreenshotEditor::createNewDocumentFromClipbard(/*QString filename*/)
{
    QClipboard* clipboard = QApplication::clipboard();
    auto image = clipboard->image();
    d->outputFilenameGenerator->initCaptureTime(); // set time to current for new document from clipboard
    d->createNewDocument(image, QString());
}

DocumentPtr KreenshotEditor::document()
{
    if (d->documentFile == nullptr) {
        return nullptr;
    }

    return d->documentFile->document();
}

DocumentFilePtr KreenshotEditor::documentFile()
{
    return d->documentFile;
}

KreenGraphicsScenePtr KreenshotEditor::graphicsScene()
{
    return d->scene;
}

MainEditorWidget* KreenshotEditor::mainEditorWidget()
{
    if (d->mainEditorWidget == nullptr) {
        auto mainEditorWidget = new MainEditorWidget(shared_from_this());
        //auto mainEditorWidget = new MainEditorWidget(this);

//         bool oldQScrollAreaLogic = false;
//         if (oldQScrollAreaLogic) {
//             auto scrollArea = new QScrollArea();
//             scrollArea->setBackgroundRole(QPalette::Background);
//             //scrollArea->setBackgroundRole(QPalette::Dark);
//             scrollArea->setWidget(mainEditorWidget);
//             d->mainEditorWidget = scrollArea;
//         }
//         else {
        d->mainEditorWidget = mainEditorWidget;
//         }

        connect(d->mainEditorWidget, SIGNAL(itemSelectionChanged()), this, SLOT(slotEditDeleteSelectedItemsActionEnabledUpdate()));
    }

    return d->mainEditorWidget;
}

QStringList KreenshotEditor::allActionIds()
{
    if (d->actionIdToActionMap.empty()) { // singleton

        //
        // file actions
        //
        d->actionDocumentNew = d->newAction("document-new", QIcon::fromTheme("document-new"), tr("&New"), this, QKeySequence(tr("Ctrl+N")));
        connect(d->actionDocumentNew, SIGNAL(triggered()), this, SLOT(slotDocumentNew()));

        auto actionDocumentNewFromClipboard = d->newAction("document-new-from-clipboard", QIcon::fromTheme("document-new"), tr("&New from clipboard"), this, QKeySequence(tr("Ctrl+Shift+N")));
        connect(actionDocumentNewFromClipboard, SIGNAL(triggered()), this, SLOT(slotDocumentNewFromClipboard()));

        d->actionDocumentOpen = d->newAction("document-open", QIcon::fromTheme("document-open"), tr("&Open"), this, QKeySequence(tr("Ctrl+O")));
        connect(d->actionDocumentOpen, SIGNAL(triggered()), this, SLOT(slotDocumentOpen()));

        d->actionDocumentSave = d->newAction("document-save", QIcon::fromTheme("document-save"), tr("Save"), this, QKeySequence(tr("Ctrl+S")));
        connect(d->actionDocumentSave, SIGNAL(triggered()), this, SLOT(slotDocumentSave()));

        d->actionDocumentSaveAs = d->newAction("document-save-as", QIcon::fromTheme("document-save-as"), tr("Save as..."), this, QKeySequence(tr("Ctrl+Shift+S")));
        connect(d->actionDocumentSaveAs, SIGNAL(triggered()), this, SLOT(slotDocumentSaveAs()));

        d->actionDocumentCopyImageToClipboard = d->newAction("document-image-to-clipboard", QIcon::fromTheme("todo"), tr("Copy image to clipboard"), this, QKeySequence(tr("Ctrl+Shift+C")));
        connect(d->actionDocumentCopyImageToClipboard, SIGNAL(triggered()), this, SLOT(slotDocumentCopyImageToClipboard()));

        d->actionDocumentCopyFilenameToClipboard = d->newAction("document-filename-to-clipboard", QIcon::fromTheme("todo"), tr("Copy filename to clipboard"), this, QKeySequence(tr("Ctrl+Alt+C")));
        connect(d->actionDocumentCopyFilenameToClipboard, SIGNAL(triggered()), this, SLOT(slotDocumentCopyFilenameToClipboard()));

        d->actionDocumentSelectInFilemanager = d->newAction("document-file-select-in-filemanager", QIcon::fromTheme("todo"), tr("Select in file manager"), this, QKeySequence(tr("Ctrl+Alt+S")));
        connect(d->actionDocumentSelectInFilemanager, SIGNAL(triggered()), this, SLOT(slotDocumentSelectInFilemanager()));

        d->actionDocumentOpenWithDefaultImageViewer = d->newAction("document-launch-default-viewer", QIcon::fromTheme("todo"), tr("Launch default image viewer"), this, QKeySequence(tr("Ctrl+Alt+L")));
        connect(d->actionDocumentOpenWithDefaultImageViewer, SIGNAL(triggered()), this, SLOT(slotDocumentLaunchDefaultViewer()));

        //
        // edit actions
        //
        d->actionEditSelectAll = d->newAction("edit-objects-select-all", QIcon::fromTheme("edit-select-all"), tr("Select All"), this, QKeySequence(tr("Ctrl+A")));
        connect(d->actionEditSelectAll, SIGNAL(triggered()), this, SLOT(slotEditSelectAll()));

        d->actionEditItemDelete = d->newAction("edit-objects-delete", QIcon::fromTheme("edit-delete"), tr("Delete"), this, QKeySequence(tr("Del")));
        d->actionEditItemDelete->setEnabled(false);
        connect(d->actionEditItemDelete, SIGNAL(triggered()), this, SLOT(slotEditDeleteSelectedItems()));

        //
        // undo actions
        //
        auto action = d->newAction("edit-undo", QIcon::fromTheme("edit-undo"), "Undo", this, QKeySequence(tr("Ctrl+Z")));
        connect(action, SIGNAL(triggered()), this, SLOT(slotEditUndo()));
        d->actionEditUndo = action;

        action = d->newAction("edit-redo", QIcon::fromTheme("edit-redo"), "Redo", this, QKeySequence(tr("Ctrl+Y")));
        connect(action, SIGNAL(triggered()), this, SLOT(slotEditRedo()));
        d->actionEditRedo = action;
    }

    return d->actionIdToActionMap.keys();
}

QAction* KreenshotEditor::actionFromId(QString actionId)
{
    if (!d->actionIdToActionMap.contains(actionId)) {
        qDebug() << "[WARN] Menu items will be missing: actionId '" << actionId << "' must be present in kreenshoteditor.cpp/actionIdToActionMap. See KreenshotEditor::allActionIds()";
        return nullptr;
    }

    return d->actionIdToActionMap.value(actionId);
}

QList<QAction*> KreenshotEditor::toolActions()
{
    return d->toolActions();
}

void KreenshotEditor::setCaptureTime(QDateTime datetime)
{
    d->outputFilenameGenerator->initCaptureTime(datetime);
}

void KreenshotEditor::setDescription(QString text)
{
    d->outputFilenameGenerator->setDescription(text);
}

void KreenshotEditor::addDemoItems()
{
    document()->addDemoItems();
}

bool KreenshotEditor::isDocumentFileNotCreated()
{
    return documentFile()->fileStatus() == DocumentFile::FileStatus_NotCreated;
}

bool KreenshotEditor::isDocumentClean()
{
    return document()->isClean();
}

bool KreenshotEditor::askUserOnUnsavedData_handleAnswer_shouldContinue()
{
    return d->askUserOnUnsavedData_handleAnswer_shouldContinue();
}

void KreenshotEditor::showPreferencesDialog(QWidget* parent)
{
    ui::settings::PreferencesDialog prefsDialog(d->settingsManager, d->outputFilenameGenerator);
    if (prefsDialog.exec() == QDialog::Accepted) {
        // only if file is still new (= not stored yet to disk) apply the new file path settings
        if (isDocumentFileNotCreated()) {
            qDebug() << "settingsToOutputFilenameManager";
            d->settingsToOutputFilenameGenerator();
            // d->outputFilenameGenerator->initCaptureTime(); // we explicitly do not change the capture time here
            documentFile()->setProposedFilename(d->outputFilenameGenerator->resultingFilename()); // emits fileStatusChanged() / to update filename in title bar
        }
    }
}

void KreenshotEditor::showAboutDialog(QWidget* parent)
{
    QMessageBox::information(parent, tr("About kreenshot-editor"),
                             tr("kreenshot-editor - screenshot image editing\n\n(c) 2014 by Gregor Mi\n\nIRC contact: #kreenshot-editor on irc.freenode.net\n\nBug reports go to codestruct@posteo.org\n(until offical issue tracker is initialized)\n\nWebsite: http://kreenshot.wordpress.com"));
}

void KreenshotEditor::slotDocumentFileStatusChanged()
{
    qDebug() << "KreenshotEditor::slotDocumentFileStatusChanged() fileStatus:" << documentFile()->fileStatus() << "isClean:" << document()->isClean();

    // the save button should be enabled when the file is not created yet
    // or the document is not clean
    d->actionDocumentSave->setEnabled(isDocumentFileNotCreated() || !document()->isClean());
    slotEditUndoRedoActionEnabledUpdate(); // also update undo/redo enabledness

    emit documentFileStatusChangedSignal();
}

void KreenshotEditor::slotDocumentNew()
{
    if (!d->askUserOnUnsavedData_handleAnswer_shouldContinue()) {
        return;
    }

    createNewDocument();
}

void KreenshotEditor::slotDocumentNewFromClipboard()
{
    if (!d->askUserOnUnsavedData_handleAnswer_shouldContinue()) {
        return;
    }

    createNewDocumentFromClipbard();
}

void KreenshotEditor::slotDocumentOpen()
{
    if (!d->askUserOnUnsavedData_handleAnswer_shouldContinue()) {
        return;
    }

    QString imageFilename = QFileDialog::getOpenFileName(this->mainEditorWidget(), tr("Open image"),
                            QString(),
                            tr("Images") + " ("
                            + d->saveFileNameFilterStringFromImageFileExtensions(DocumentFile::supportedImageFormats())
                            + ")");

    if (!imageFilename.isEmpty()) {
        createNewDocumentFromFile(imageFilename);
    }
}

void KreenshotEditor::slotDocumentSave()
{
    if (isDocumentFileNotCreated() && QFile::exists(documentFile()->filename())) {
        int ret = QMessageBox::warning(this->mainEditorWidget(), tr("Save file"),
                                       QString(tr("%1 already exists.\nDo you want to replace it?")).arg(documentFile()->filename()),
                                       QMessageBox::Yes | QMessageBox::No,
                                       QMessageBox::No);
        qDebug() << ret;
        if (ret != QMessageBox::Yes) {
            qDebug() << "not yes => abort";
            return;
        }
    }

    ErrorStatus errorStatus = documentFile()->save();
    d->handleSaveImageError(this->mainEditorWidget(), errorStatus);
}

void KreenshotEditor::slotDocumentSaveAs()
{
    QString filename = QFileDialog::getSaveFileName(this->mainEditorWidget(), tr("Save file as"),
                       documentFile()->filename(),
                       tr("Images") + " ("
                       + d->saveFileNameFilterStringFromImageFileExtensions(DocumentFile::supportedImageFormats())
                       + ")");
    if (!filename.isEmpty()) {
        ErrorStatus errorStatus = documentFile()->saveAs(filename);
        d->handleSaveImageError(this->mainEditorWidget(), errorStatus);
    }
}

void KreenshotEditor::slotDocumentCopyImageToClipboard()
{
    document()->copyImageToClipboard();
}

void KreenshotEditor::slotDocumentCopyFilenameToClipboard()
{
    documentFile()->copyFilenameToClipboard();
}

void KreenshotEditor::slotDocumentSelectInFilemanager()
{
    documentFile()->selectInFilemanager();
}

void KreenshotEditor::slotDocumentLaunchDefaultViewer()
{
    documentFile()->launchDefaultImageViewer();
}

void KreenshotEditor::slotEditUndo()
{
    document()->undo();
}

void KreenshotEditor::slotEditRedo()
{
    document()->redo();
}

void KreenshotEditor::slotEditUndoRedoActionEnabledUpdate()
{
    d->actionEditUndo->setEnabled(document()->canUndo());
    d->actionEditRedo->setEnabled(document()->canRedo());
}

void KreenshotEditor::slotEditDeleteSelectedItems()
{
    mainEditorWidget()->deleteSelectedItems();
}

void KreenshotEditor::slotEditDeleteSelectedItemsActionEnabledUpdate()
{
    // qDebug() << "slotEditDeleteSelectedItemsActionEnabledUpdate";
    d->actionEditItemDelete->setEnabled(mainEditorWidget()->selectedItemsCount() > 0);
}

void KreenshotEditor::slotEditSelectAll()
{
    mainEditorWidget()->selectAllItems();
}

}
