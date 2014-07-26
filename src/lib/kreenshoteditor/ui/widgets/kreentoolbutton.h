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
#ifndef UUID_KreenToolButton_H
#define UUID_KreenToolButton_H

#include <kreen/util/exportutil.h>
#include <QWidget>
#include <QToolButton>

class QMenu;

namespace kreen {
namespace ui {

/**
 * Gives more control over the item height of Actions
 */
class KREEN_DECL_EXPORT KreenToolButton : public QToolButton
{
    Q_OBJECT

public:
    KreenToolButton(QWidget* parent = 0);

    virtual ~KreenToolButton();

    void setMaxIconSize(int maxIconSize);

    /**
     * sets the action to the menu that was autoset via setMenu
     */
    void addMenuAction(QAction* action);

    void addAction(QAction* action);

public Q_SLOTS:

private:
    QMenu* _menu;
};

}
}

#endif

// kate: indent-mode cstyle; replace-tabs on;
