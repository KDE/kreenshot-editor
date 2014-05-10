/*
 * This file is part of the kreenshot-editor project.
 *
 * Copyright (C) 2014 by Gregor Mi <codeminister@publicstatic.de>
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

#include <QWidget>
#include <memory>
#include "../kreenshoteditor.h"

class MyQGraphicsView;
class QPaintEvent;
class QGraphicsView;
class MainEditorWidgetImpl;
typedef std::shared_ptr<MainEditorWidgetImpl> MainEditorWidgetImplPtr;

class MainEditorWidget : public QWidget
{
    Q_OBJECT

public:
    MainEditorWidget(KreenshotEditorPtr kreenshotEditor);
    virtual ~MainEditorWidget();

Q_SIGNALS:
    void toolChosen(QString toolId);

public Q_SLOTS:
    void requestTool(QString toolId);

protected:
    void paintEvent(QPaintEvent*);

private:
    MyQGraphicsView* _graphicsView;

    MainEditorWidgetImplPtr d;
};

#endif // UI_MAIN_EDITOR_WIDGET_H

// kate: indent-mode cstyle; replace-tabs on;
