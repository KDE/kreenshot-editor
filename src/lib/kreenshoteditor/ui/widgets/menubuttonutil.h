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
#ifndef UUID_26b34cda_d030_4356_a926_534f71be9e3f
#define UUID_26b34cda_d030_4356_a926_534f71be9e3f

#include <QWidget>
#include <QToolButton>

class MenuButtonUtil : public QObject
{
    Q_OBJECT

public:
    /**
     * connects click event
     */
    MenuButtonUtil(QToolButton* toolButton);

    virtual ~MenuButtonUtil();

public Q_SLOTS:

private:

};

#endif

// kate: indent-mode cstyle; replace-tabs on;
