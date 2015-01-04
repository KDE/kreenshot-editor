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

#include "sharedptrutil.h"

// KREEN_DECL_EXPORT, needed by prepare-include-files.sh

/**
 * Utility to faciliate the usage of the Pimpl idiom
 * 
 * Usage after using the forward decl:
 * - Use OrigClassName##ImplPtr to define a private d pointer in the h file
 * - Define a class OrigClassName##Impl in the cpp file
 */
#define KREEN_PIMPL_DEFINE_D_PTR \
class Impl; \
typedef std::shared_ptr<Impl> ImplPtr; \
ImplPtr d;

/**
 * QT style d_ptr definition
 *
 * Needs:
 * - class class Document::Private { ... }; in impl
 * - : d_ptr(new Private) in owner class ctor
 * - define d when needed with Q_D();
 */
#define KREEN_Q_DECLARE_PRIVATE \
class Private; \
Private* const d_ptr; \
Q_DECLARE_PRIVATE(); \

/**
 * call this as first line in the ctor
 * example: KREEN_PIMPL_INIT(Document)
 */
#define KREEN_PIMPL_INIT(OrigClassName) d = std::make_shared<OrigClassName::Impl>();

/**
 * call this as first line in the ctor and give this as first parameter
 * example: KREEN_PIMPL_INIT(Document)
 */
#define KREEN_PIMPL_INIT_THIS(OrigClassName) d = std::make_shared<OrigClassName::Impl>(this);

#endif

// kate: indent-mode cstyle; replace-tabs on;
