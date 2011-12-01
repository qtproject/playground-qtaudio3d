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
#include "qalsndaudiodecoder.h"

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
    if ((d->alcDevice = alcOpenDevice(d->requestedAttributes.deviceSpecifier().toAscii())) == false) {
        qWarning() << Q_FUNC_INFO << "Failed to open the device:" << d->requestedAttributes.deviceSpecifier();
        return false;
    }

    ALCenum error;
    if ((error = alcGetError(d->alcDevice)) != ALC_NO_ERROR) {
        qWarning() << Q_FUNC_INFO << "Error before trying to create the context:" << alcGetString(d->alcDevice, error);
    };

    ALCint attributes[] = {
        ALC_FREQUENCY, d->requestedAttributes.frequency(),
        ALC_MONO_SOURCES, d->requestedAttributes.monoSources(),
        ALC_REFRESH, d->requestedAttributes.refresh(),
        ALC_STEREO_SOURCES, d->requestedAttributes.stereoSources(),
        ALC_SYNC, d->requestedAttributes.sync(),
        0
    };

    d->alcContext = alcCreateContext(d->alcDevice, attributes);
    if ((error = alcGetError(d->alcDevice)) != ALC_NO_ERROR) {
        qWarning() << Q_FUNC_INFO << "Failed to create the context:" << alcGetString(d->alcDevice, error);
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
    if (isValid() == false)
        return QALAttributes::defaultAttributes();

    ALCenum error;

    if ((error = alcGetError(d->alcDevice)) != ALC_NO_ERROR) {
        qWarning() << Q_FUNC_INFO << "Error before trying to create attributes:" << alcGetString(d->alcDevice, error);
    };

    QALAttributes attributes;
    attributes.setDeviceSpecifier(alcGetString(d->alcDevice, ALC_DEVICE_SPECIFIER));

    ALCint tmp;

    alcGetIntegerv(d->alcDevice, ALC_FREQUENCY, 1, &tmp);
    attributes.setFrequency(tmp);

    alcGetIntegerv(d->alcDevice, ALC_REFRESH, 1, &tmp);
    attributes.setRefresh(tmp);

    alcGetIntegerv(d->alcDevice, ALC_SYNC, 1, &tmp);
    attributes.setSync(tmp);

    alcGetIntegerv(d->alcDevice, ALC_MONO_SOURCES, 1, &tmp);
    attributes.setMonoSources(tmp);

    alcGetIntegerv(d->alcDevice, ALC_STEREO_SOURCES, 1, &tmp);
    attributes.setStereoSources(tmp);

    return attributes;
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
        qWarning() << Q_FUNC_INFO << "Error before trying to destroy the context:" << alcGetString(d->alcDevice, error);
    };

    alcDestroyContext(d->alcContext);
    if ((error = alcGetError(d->alcDevice)) != ALC_NO_ERROR) {
        qWarning() << Q_FUNC_INFO << "Failed to destroy the context:" << alcGetString(d->alcDevice, error);
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
    ALuint buffer = d->loadedBuffers.value(filename, 0);
    if (!buffer) {
        QALSndAudioDecoder qalSndAudioDecoder;
        if (qalSndAudioDecoder.open(filename) == false)
            return 0;

        QByteArray decodedData;
        QByteArray tmpData;
        int maxlen = qalSndAudioDecoder.channels() * qalSndAudioDecoder.sampleRate() * qalSndAudioDecoder.sampleSize();

        forever {
            tmpData = qalSndAudioDecoder.decode(maxlen);
            decodedData.append(tmpData);
            if (tmpData.size() != maxlen)
                break;
        }

        ALenum error;
        if ((error = alGetError()) != AL_NO_ERROR) {
            qWarning() << Q_FUNC_INFO << "Error before trying to generate a buffer:" << alGetString(error);
        };

        alGenBuffers(1, &buffer);

        if ((error = alGetError()) != AL_NO_ERROR) {
            qWarning() << Q_FUNC_INFO << "Failed to generate a buffer:" << alGetString(error);
            return 0;
        };

        int channels = qalSndAudioDecoder.channels();
        int sampleSize = qalSndAudioDecoder.sampleSize();
        ALenum format;

        if (channels == 1) {
            if (sampleSize == 8)
                format = AL_FORMAT_MONO8;
            else if (sampleSize == 16)
                format = AL_FORMAT_MONO16;
        } else if (channels == 2) {
            if (sampleSize == 8)
                format = AL_FORMAT_STEREO8;
            else if (sampleSize == 16)
                format = AL_FORMAT_STEREO16;
        }

        alBufferData(buffer, format, reinterpret_cast<const ALvoid*>(decodedData.constData()), decodedData.size(), qalSndAudioDecoder.sampleRate());

        d->loadedBuffers.insert(filename, buffer);
    }

    return buffer;
}

bool
QALContext::deleteBuffer(ALuint bufferId)
{
    ALenum error;
    if ((error = alGetError()) != AL_NO_ERROR) {
        qWarning() << Q_FUNC_INFO << "Error before trying to delete the buffer:" << alGetString(error);
    };

    alDeleteBuffers(1, &bufferId);

    if ((error = alGetError()) != AL_NO_ERROR) {
        qWarning() << Q_FUNC_INFO << "Failed to delete to buffer:" << alGetString(error);
        return false;
    };

    d->loadedBuffers.remove(d->loadedBuffers.key(bufferId));

    return true;
}

bool
QALContext::deleteBuffers()
{
    ALenum error;
    if ((error = alGetError()) != AL_NO_ERROR) {
        qWarning() << Q_FUNC_INFO << "Error before trying to delete the buffers:" << alGetString(error);
    };

    QList<ALuint> identifiers = d->loadedBuffers.values();
    alDeleteBuffers(identifiers.count(), identifiers.toVector().data());

    if ((error = alGetError()) != AL_NO_ERROR) {
        qWarning() << Q_FUNC_INFO << "Failed to delete to buffers:" << alGetString(error);
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

