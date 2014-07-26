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
#include "kreentoolbutton.h"
#include <QProxyStyle>
#include <QMenu>

namespace kreen {
namespace ui {

class MyStyle : public QProxyStyle
{
public:
    MyStyle(int maxIconSize)
    {
        _maxIconSize = maxIconSize;
    }

protected:
    virtual int pixelMetric(PixelMetric metric, const QStyleOption* option = 0, const QWidget* widget = 0) const
    {
        // http://www.qtcentre.org/threads/15742-How-to-change-Icon-size-in-QMenu
        if (metric == PM_SmallIconSize) {
            return _maxIconSize;
        }
        else
            return QProxyStyle::pixelMetric(metric, option, widget);
    }

//     virtual QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const
//     {
//         auto s = QProxyStyle::sizeFromContents(type, option, size, widget);
//         s.setHeight(40);
//         return s;
//     }

//     virtual QRect itemPixmapRect(const QRect& r, int flags, const QPixmap& pixmap) const
//     {
//         auto rect = QProxyStyle::itemPixmapRect(r, flags, pixmap);
//         rect.setHeight(40);
//         return rect;
//     }

private:
    int _maxIconSize;
};

// KreenToolButton::setDefaultActionShowMenu()
// {
//     connect(this, SIGNAL(clicked()), this, SLOT(showMenu()));
// }

KreenToolButton::KreenToolButton(QWidget* parent) : QToolButton(parent)
{
    _menu = new QMenu(this);
    setMenu(_menu);
}

KreenToolButton::~KreenToolButton()
{

}

void KreenToolButton::addAction(QAction* action)
{
    Q_ASSERT_X(false, "KreenToolButton::addAction", "Do not use. Use addMenuAction instead");
}

void KreenToolButton::addMenuAction(QAction* action)
{
    menu()->addAction(action);
}

void KreenToolButton::setMaxIconSize(int maxIconSize)
{
    _menu->setStyle(new MyStyle(maxIconSize));
}

}
}
