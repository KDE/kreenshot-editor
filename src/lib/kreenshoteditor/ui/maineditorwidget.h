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
#ifndef UI_MAIN_EDITOR_WIDGET_H
#define UI_MAIN_EDITOR_WIDGET_H

#include <kreen/util/exportutil.h>
#include <kreen/util/pimplutil.h>
#include <QWidget>
#include <memory>
#include <kreen/kreenshoteditor.h>
#include <kreen/core/kreenitem.h>

using namespace kreen::core;

class QPaintEvent;
class QGraphicsView;

namespace kreen {
namespace ui {

class KreenGraphicsView;

KREEN_PIMPL_FORWARD_DECL(MainEditorWidget)

class KREEN_DECL_EXPORT MainEditorWidget : public QWidget
{
    Q_OBJECT

public:
    MainEditorWidget(KreenshotEditorPtr kreenshotEditor);
    virtual ~MainEditorWidget();

    /**
     */
    void deleteSelectedItems();

    void selectAllItems();

    /**
     * tmp
     */
    int selectedItemsCount();

Q_SIGNALS:
    void toolChosenSignal(QString toolId);
    void itemSelectionChanged();

public Q_SLOTS:
    /**
     * possible toolIds:
     * select, rect, ellipse, line, ...
     * op-crop, ...
     */
    void requestTool(QString toolId);

protected Q_SLOTS:
    void slotDocumentCreated();
    void slotDocumentContentChanged();

    void slotUpdateItemsGeometryFromModel();
    void slotUpdateSceneWithImageOperationItem(KreenItemPtr item);
    void slotImageOperationAccepted();
    void slotImageOperationAcceptedDecoupled();
    void slotImageOperationCanceled();

    /**
     * a draw item like rect is added to model
     * an operation item like op-crop is handled otherwise
     */
    void slotHandleNewItem(KreenItemPtr item);

    void slotSceneSelectionChanged();
    void slotRedrawSelectionHandles();

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    MainEditorWidgetImplPtr d;
};

}
}

#endif

// kate: indent-mode cstyle; replace-tabs on;
