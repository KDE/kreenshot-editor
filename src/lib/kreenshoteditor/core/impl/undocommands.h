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
#ifndef UUID_689145c4_1a7e_11e4_b715_002454dd224f
#define UUID_689145c4_1a7e_11e4_b715_002454dd224f

#include <QUndoCommand>

/**
 * This file contains serveral QUndoCommand derivates
 */
namespace kreen {
namespace core {

/**
*
*/
class KreenUndoCmd : public QUndoCommand
{
public:

};

class AddItem : public KreenUndoCmd
{
public:

};

class MoveItem : public KreenUndoCmd
{
public:

};

class ResizeItem : public KreenUndoCmd
{
public:

};

class ApplyImageOperation : public KreenUndoCmd
{
public:

};

}
}

#endif

// kate: indent-mode cstyle; replace-tabs on;
