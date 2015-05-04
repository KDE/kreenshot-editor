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
#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>
#include "ui/mainwindow.h"
#include <kreen/kreenshoteditor.h>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>

#include <iostream>
#include <stdexcept>

#define tr(arg) QObject::tr(arg)

class Arguments
{
public:
    QString baseImagePath;
    QString description;
    bool newFromFile;
    bool newFromClipboard;
    bool addDemoItems;
};

Arguments parseArgumentsOrExit(QApplication& app);

int main(int argc, char *argv[])
{
    const QString version = "0.1";
    QApplication app(argc, argv);

    QTranslator qtTranslator;
    qtTranslator.load(QLatin1String("qt_") + QLocale::system().name(),
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    QLatin1String localeSuffix("/locale");
    QString localeName(QLatin1String("kreenshot_editor_") +
                       (qgetenv("KDE_LANG") == "x-test" ? QLatin1String("x_test") : QLocale::system().name()));

    QTranslator appSystemTranslator;
    appSystemTranslator.load(localeName, localeSuffix);
    app.installTranslator(&appSystemTranslator);

    QCoreApplication::setApplicationVersion(version);
    auto arguments = parseArgumentsOrExit(app);

    auto kreenshotEditor = kreen::KreenshotEditor::make_shared();

    kreenshotEditor->performSelfTest();

    kreenshotEditor->setCaptureTime();
    kreenshotEditor->setDescription(arguments.description);
    // TODO later: set pattern from command line (--output-file-pattern)

    if (arguments.newFromClipboard) {
        kreenshotEditor->createNewDocumentFromClipbard();
    }
    else {
        if (arguments.newFromFile) {
            kreenshotEditor->createNewDocument(QImage(arguments.baseImagePath)); // load base image data from file
        }
        else {
            kreenshotEditor->createNewDocumentFromFile(arguments.baseImagePath); // load document from file
        }
    }

    if (arguments.addDemoItems) {
        kreenshotEditor->addDemoItems();
    }

    MainWindow mainWindow(kreenshotEditor);
    //mainWindow.resize(640, 480);
    mainWindow.show();

    return app.exec();
}

Arguments parseArgumentsOrExit(QApplication& app)
{
    qDebug() << "parseArgumentsOrExit";

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.setApplicationDescription(QObject::tr("screenshot image editing, (c) 2014"));

    parser.addPositionalArgument("image", QObject::tr("Filename (i.e. with or without path) of primary image to be loaded. If file does not exist, default image data will be used."),
                                 QObject::tr("[filename]")); // optional

    QCommandLineOption newFromFileOption(QStringList() << "new-from-file",
                                        QObject::tr("Image data of the given primary image filename which will be used as base image for a new document. A default filename will be proposed automatically. Not allowed with --new-from-clipboard."));
    parser.addOption(newFromFileOption);

    QCommandLineOption newFromClipboardOption(QStringList() << "new-from-clipboard",
                                              QObject::tr("Like --new-from-file but takes the image data from the clipboard (image data or filename of image). If no valid image data present a default image will be used. Not allowed with --new-from-file. Positional image filename argument not allowed."));
    parser.addOption(newFromClipboardOption);

    QCommandLineOption image2Option(QStringList() << "image2",
                                    QObject::tr("Filename (i.e. with or without path) of secondary image to be inserted as object, e. g. the captured mouse cursor."),
                                    QObject::tr("filename"));
    parser.addOption(image2Option);

    QCommandLineOption image2xOption(QStringList() << "image2x",
                                     QObject::tr("X coordinate in pixel of the upper left corner of the secondary image provided by --image2. Default value is 0."),
                                     QObject::tr("number"));
    parser.addOption(image2xOption);

    QCommandLineOption image2yOption(QStringList() << "image2y",
                                     QObject::tr("Y coordinate, see --image2x."),
                                     QObject::tr("number"));
    parser.addOption(image2yOption);

    QCommandLineOption descriptionOption(QStringList() << "description",
                                         QObject::tr("Description, e. g. window title of the captured window."),
                                         QObject::tr("text"));
    parser.addOption(descriptionOption);

    QCommandLineOption addDemoItemsOption(QStringList() << "add-demo-items",
                                         QObject::tr("Adds a set of demo items to the loaded image."));
    parser.addOption(addDemoItemsOption);

    parser.process(app);

    const QStringList args = parser.positionalArguments();

    Arguments arguments;

    try {
        if (args.length() == 0) {
            arguments.baseImagePath = "";
            qDebug() << "  baseImagePath: no image given";
        }
        else if (args.length() == 1) {
            arguments.baseImagePath = args[0];
            qDebug() << "  baseImagePath:" << arguments.baseImagePath;
        }
        else {
            // http://stackoverflow.com/questions/4214369/how-to-convert-qstring-to-stdstring
            throw std::runtime_error("Zero or one positional argument expected");
        }

        QString image2Filepath = parser.value(image2Option);
        qDebug() << "  image2Filepath:" << image2Filepath;

        QString image2xStr = parser.value(image2xOption);
        qDebug() << "  image2x:" << image2xStr;
        int image2x = 0;
        if (!image2xStr.isEmpty()) {
            bool ok;
            image2x = image2xStr.toInt(&ok);
            if (!ok) throw std::runtime_error(QObject::tr("Wrong argument (cannot parse x value)").toUtf8().constData());
        }
        qDebug() << "  image2x:" << image2x;

        QString image2yStr = parser.value(image2yOption);
        qDebug() << "  image2y:" << image2yStr;
        int image2y = 0;
        if (!image2yStr.isEmpty()) {
            bool ok;
            image2y = image2yStr.toInt(&ok);
            if (!ok) throw std::runtime_error(QObject::tr("Wrong argument (cannot parse y value)").toUtf8().constData());
        }
        qDebug() << "  image2y:" << image2y;

        QString descriptionStr = parser.value(descriptionOption);
        arguments.description = descriptionStr;
        qDebug() << "  description:" << arguments.description;

        arguments.newFromFile = parser.isSet(newFromFileOption);
        qDebug() << "  treatAsNew:" << arguments.newFromFile;

        arguments.newFromClipboard = parser.isSet(newFromClipboardOption);
        qDebug() << "  newFromClipboard:" << arguments.newFromClipboard;

        arguments.addDemoItems = parser.isSet(addDemoItemsOption);

        // check for invalid combinations
        //
        if (arguments.newFromClipboard && arguments.newFromFile) {
            throw std::runtime_error(QObject::tr("--new-from-file not allowed together with --new-from-clipboard.").toUtf8().constData());
        }

        if (arguments.newFromFile && arguments.baseImagePath.isEmpty()) {
            throw std::runtime_error(QObject::tr("Positional image filename argument is required for --new-from-file.").toUtf8().constData());
        }

        if (arguments.newFromClipboard && !arguments.baseImagePath.isEmpty()) {
            throw std::runtime_error(QObject::tr("Positional image filename argument not allowed together with --new-from-clipboard.").toUtf8().constData());
        }
    }
    catch (const std::runtime_error& e) {
        std::cout << QObject::tr("Error with command line usage: ").toUtf8().constData() << e.what() << std::endl << std::endl;
        parser.showHelp(1); // exits application
    }

    return arguments;
}
