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

#ifndef QALCONTEXT_H
#define QALCONTEXT_H

#include "q_openal_export.h"

#include "qalattributes.h"

class QALBufferQueue;

class Q_OPENAL_EXPORT QALContext
{
    //Q_DECLARE_PRIVATE(QALContext)
public:
    /**
     * Constructor
     */
    QALContext(const QALAttributes &attributes = QALAttributes::defaultAttributes());

    /**
     * Desctructor
     */
    virtual ~QALContext();

    /**
     * Opens the device, starts it in the context, and then creates the
     * context accordingly
     *
     * @return True if the operation was successful; otherwise false
     */
    bool create();

    /**
     * Returns the attributes that the application requested, and what create()
     * will use to create the device/context with
     *
     * @see create
     */
    QALAttributes requestedAttributes() const;

    /*! 
      Sets the requested attributes for the devices.

      \sa attributes()
      */
    void setRequestedAttributes(const QALAttributes &attributes);

    /*! 
      Return the current attributes as given by the device. It constructs a new
      QALAttributes using the values directly from the device

      \sa requestedAttributes()
      */
    QALAttributes attributes() const;

    /**
     * Checks whether or not the context is valid.
     *
     * @return True, if the context is valid; otherwise false
     */
    bool isValid() const;

    /**
     * Destroys the context and closes the device
     *
     * @return True, if the destroy and close were successful; otherwise false
     */
    bool reset();

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
     *
     * @param filename The file to open
     */
    ALuint cacheBuffer(const QString& filename);

    /**
     * Uses a QALBufferQueue to hold multiple buffers that get little portions
     * of the audio instead of loading the sound fully into a buffer
     */
    QALBufferQueue streamBuffer(const QString& filename);

    bool deleteBuffer(ALuint bufferId);

    bool deleteBuffers();

    void deleteBufferQueue(QALBufferQueue &bufferQueue);

    /**
     * Calls alcGetProcAddress with the opened device
     */
    void *getProcAddress(const QString &proc) const;


    private:
        class Private;
        Private *const d;
};

#endif
