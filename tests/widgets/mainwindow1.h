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
#ifndef UI_MAIN_WINDOW1_H
#define UI_MAIN_WINDOW1_H

#include <QMainWindow>
#include <QLayout>

class MainWindow1 : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow1();
    virtual ~MainWindow1();

public:
    void setupUi();

protected Q_SLOTS:
    void slotMessageBox(QString message);
    void slotSelectInFilemanager();
    void slotChooseColor1();
    void slotChooseColor2();
    void slotChooseColor3();

private:
    void addDescriptionLabel(QBoxLayout* layout, QString text);
};

#endif

// kate: indent-mode cstyle; replace-tabs on;
