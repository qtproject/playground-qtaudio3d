/******************************************************************************
 * This file is part of the Mula project
 * Copyright (c) 2011 Laszlo Papp <lpapp@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef Q_OPENAL_EXPORT_H
#define Q_OPENAL_EXPORT_H

#include <QtCore/qglobal.h>

#ifndef Q_OPENAL_EXPORT
#if defined(MAKE_Q_OPENAL_LIB)
#define Q_OPENAL_EXPORT Q_DECL_EXPORT
#else
#define Q_OPENAL_EXPORT Q_DECL_IMPORT
#endif
#endif

#ifndef Q_OPENAL_EXPORT_DEPRECATED
#define Q_OPENAL_EXPORT_DEPRECATED Q_DECL_DEPRECATED Q_OPENAL_EXPORT
#endif

#if !defined(ALC_VERSION_0_1) || !defined(AL_VERSION_1_0)
#ifdef Q_OS_WIN
#include <al.h>
#include <alc.h>
#elif Q_OS_MAC
#include <OpenAL/alc.h>
#include <OpenAL/al.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif
#endif

#endif // Q_OPENAL_EXPORT_H

