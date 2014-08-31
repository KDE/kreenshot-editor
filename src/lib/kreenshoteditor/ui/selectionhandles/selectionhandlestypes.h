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
#ifndef UUID_c0d16526310c11e4af57002454dd224f
#define UUID_c0d16526310c11e4af57002454dd224f

namespace kreen {
namespace ui {
namespace selhandles {

/**
 * behaviour of handles
 */
enum HandleTypeEnum
{
    HandleType_ResizeRect,
    HandleType_ResizeLine
};

/**
* 1   7   2
* 5   0   6
* 3   8   4
*/
enum PositionEnum
{
    // not used yet:
    Position0_Center = 0,

    // for resizing rect-based items:
    Position1_TopLeft = 1,
    Position2_TopRight = 2,
    Position3_BottomLeft = 3,
    Position4_BottomRight = 4,
    Position5_Left = 5,
    Position6_Right = 6,
    Position7_Top = 7,
    Position8_Bottom = 8,

    // for changing line-based items:
    Position_LineStart = 20,
    Position_LineEnd = 21
};

}
}
}

#endif
