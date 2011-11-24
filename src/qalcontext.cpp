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

#include "qalcontext.h"

#include "qalbufferqueue.h"

#include <QtCore/QDebug>

class QALContext::Private
{
    public:
        Private(const QALAttributes &attributes)
            : alcDevice(0)
            , alcContext(0)
            , requestedAttributes(attributes)
        {
        }

        ~Private()
        {
        }

        ALCdevice *alcDevice;
        ALCcontext *alcContext;
        QALAttributes requestedAttributes;
        QHash<QString, ALuint> loadedBuffers;
};

QALContext::QALContext(const QALAttributes &attributes)
    : d(new Private(attributes))
{
}

QALContext::~QALContext()
{
}

bool
QALContext::create()
{
    // Open the default device
    if ((d->alcDevice = alcOpenDevice(NULL)) == false)
        return false;

    if ((d->alcContext = alcCreateContext(d->alcDevice, NULL)) == 0)
    {
        alcCloseDevice(d->alcDevice);
        d->alcDevice = 0;
        return false;
    }

    return true;
}

QALAttributes
QALContext::requestedAttributes() const
{
    return d->requestedAttributes;
}

void
QALContext::setRequestedAttributes(const QALAttributes &attributes)
{
    d->requestedAttributes = attributes;
}

QALAttributes
QALContext::attributes() const
{
}

bool
QALContext::isValid() const
{
    return alcGetContextsDevice(d->alcContext) != 0;
}

bool
QALContext::reset()
{
    ALCenum error;
    if ((error = alcGetError(d->alcDevice)) != ALC_NO_ERROR) {
        qWarning() << "Error before trying to destroy the context:" << alcGetString(d->alcDevice, error);
    };

    alcDestroyContext(d->alcContext);
    if ((error = alcGetError(d->alcDevice)) != ALC_NO_ERROR) {
        qWarning() << "Failed to destroy the context:" << alcGetString(d->alcDevice, error);
        return false;
    };

    d->alcContext = 0;

    alcCloseDevice(d->alcDevice);
    d->alcDevice = 0;

    return true;
}

bool
QALContext::makeCurrent()
{
    if (isValid() == false) {
        qWarning() << Q_FUNC_INFO << "Cannot make invalid context current";
        return false;
    }

    return alcMakeContextCurrent(d->alcContext);
}

bool
QALContext::doneCurrent()
{
    return alcMakeContextCurrent(0);
}

ALuint
QALContext::cacheBuffer(const QString& filename)
{
}

bool
QALContext::deleteBuffer(ALuint bufferId)
{
    ALenum error;
    if ((error = alGetError()) != ALC_NO_ERROR) {
        qWarning() << "Error before trying to delete the buffer:" << alGetString(error);
    };

    alDeleteBuffers(1, &bufferId);

    if ((error = alGetError()) != AL_NO_ERROR) {
        qWarning() << "Failed to delete to buffer:" << alGetString(error);
        return false;
    };

    d->loadedBuffers.remove(d->loadedBuffers.key(bufferId));

    return true;
}

bool
QALContext::deleteBuffers()
{
    ALenum error;
    if ((error = alGetError()) != ALC_NO_ERROR) {
        qWarning() << "Error before trying to delete the buffers:" << alGetString(error);
    };

    QList<ALuint> identifiers = d->loadedBuffers.values();
    alDeleteBuffers(identifiers.count(), identifiers.toVector().data());

    if ((error = alGetError()) != AL_NO_ERROR) {
        qWarning() << "Failed to delete to buffers:" << alGetString(error);
        return false;
    };

    d->loadedBuffers.clear();

    return true;
}

void
QALContext::deleteBufferQueue(QALBufferQueue &bufferQueue)
{
}

void*
QALContext::getProcAddress(const QString &proc) const
{
    return alGetProcAddress(proc.toAscii().constData());
}

