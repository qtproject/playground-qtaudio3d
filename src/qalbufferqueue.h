/******************************************************************************
 * This file is part of the QtOpenAL project
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

#ifndef QALBUFFERQUEUE_H
#define QALBUFFERQUEUE_H

#include "q_openal_export.h"

//class QString;

class Q_OPENAL_EXPORT QALBufferQueue
{
    //Q_DECLARE_PRIVATE(QALBufferQueue)
public:
    QALBufferQueue();
    QALBufferQueue(const QString& fileName);
    virtual ~QALBufferQueue();

    void *decode(unsigned int &len);
    bool processedBuffers();
};

#endif
