/*
 * This file is part of the kreenshot-editor project.
 *
 * Copyright (C) 2014 by Gregor Mitsch <codeminister@publicstatic.de>
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

namespace Ui {
    class MainWindow;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow();
        virtual ~MainWindow();

    public Q_SLOTS:
//         void saveFile();
//         void loadFile();

    protected:
        void setupActions();
        void setupUi();

    private:
        Ui::MainWindow* _ui;
        // std::unique_ptr<Ui::MainWindow> _ui; // TODO:  ‘unique_ptr’ in namespace ‘std’ does not name a type

//         tikz::ui::LinePropertyWidget * m_linePropertyWidget;
};

#endif // UI_MAIN_WINDOW_H

// kate: indent-width 4; replace-tabs on;
