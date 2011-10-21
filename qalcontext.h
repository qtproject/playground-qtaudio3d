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

#include "q_openal_export.h"

class Q_OPENAL_EXPORT QALContext
{
    //Q_DECLARE_PRIVATE(QALContext)
public:
    QALContext();
    virtual ~QALContext();

    /**
     * Opens the device, starts it in the context, and then creates the
     * context accordingly
     *
     * @return True if the operation was successful; otherwise false
     */
    virtual bool create();

    /**
     * Checks whether or not the context is valid.
     *
     * @return True, if the context is valid; otherwise false
     */
    bool isValid() const;

    /**
     * Destroys the context
     */
    void reset();

    /**
     * Tries to set the context current
     *
     * @return True, if the context could be set as current; otherwise false
     */
    bool makeCurrent();

    /**
     * Tries to set the current context to NULL
     *
     * @return  True, if the current context could be to NULL; otherwise false
     */
    bool doneCurrent();

    /**
     * Generates a buffer, opens the file with the audio decoder and then loads
     * the sound into it
     */
    ALuint cacheBuffer(const QString& filename);
    QALBufferQueue streamBuffer();

    /**
     * Calls alcGetProcAddress with the opened device
     */
    void *getProcAddress(const QString &proc) const;
};
