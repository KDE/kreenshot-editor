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
#ifndef UI_MAIN_WINDOW_H
#define UI_MAIN_WINDOW_H

#include <QMainWindow>
#include <memory>

namespace Ui {
class MainWindow;
typedef std::shared_ptr<MainWindow> MainWindowPtr;
}

namespace kreen {
class KreenshotEditor;
typedef std::shared_ptr<KreenshotEditor> KreenshotEditorPtr;
}

class MainWindowImpl;
typedef std::shared_ptr<MainWindowImpl> MainWindowImplPtr;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(kreen::KreenshotEditorPtr kreenshotEditor);
    virtual ~MainWindow();

public Q_SLOTS:
    void editPreferences();
    void helpAbout();

    void slotToolChosen(QString toolId);

    void updateOutputFileStatus();

public:
    void setupActions();
    void setupUi();

private:
    MainWindowImplPtr d;
};

#endif

// kate: indent-mode cstyle; replace-tabs on;
