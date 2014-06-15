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
#ifndef UTIL_PIMPLUTIL_H
#define UTIL_PIMPLUTIL_H

// "class" (TODO: need that to be found by prepare-include-files.sh / replace later with some export statement)

/**
 * Needs #include <memory>
 * Actions to be done:
 * - Use OrigClassName##ImplPtr to define a private d pointer in the h file
 * - Define a class OrigClassName##Impl in the cpp file
 */
#define KREEN_PIMPL_FORWARD_DECL(OrigClassName) \
class OrigClassName##Impl; \
typedef std::shared_ptr<OrigClassName##Impl> OrigClassName##ImplPtr;

/**
 * call this as first line in the ctor
 */
#define KREEN_PIMPL_INIT(OrigClassName) d = std::make_shared<OrigClassName##Impl>();


#endif // UTIL_PIMPLUTIL_H

// kate: indent-mode cstyle; replace-tabs on;
