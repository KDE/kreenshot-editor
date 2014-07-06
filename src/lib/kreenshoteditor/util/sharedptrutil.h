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
#ifndef UTIL_SHAREDPTRUTIL_H
#define UTIL_SHAREDPTRUTIL_H

// KREEN_DECL_EXPORT, needed by prepare-include-files.sh

#include <memory>

/**
 * NOTE that this might be wrapped with in a correct namespace declaration
 */
#define KREEN_SHAREDPTR_FORWARD_DECL(OrigClassName) \
class OrigClassName;\
typedef std::shared_ptr<OrigClassName> OrigClassName##Ptr;

#endif

// kate: indent-mode cstyle; replace-tabs on;
