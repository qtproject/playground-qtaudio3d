/****************************************************************************
**
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: http://www.qt-project.org/
**
** This file is part of the QtAudio3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qalsource.h"

class QALSource::Private
{
    public:
        Private()
            : source(0)
        {
        }

        ~Private()
        {
        }

        ALuint source;
};

QALSource::QALSource(unsigned int source)
    : d(new Private)
{
    d->source = source;
}

QALSource::~QALSource()
{
}

float QALSource::minimumGain() const
{
    float minimumGain;
    alGetSourcef(d->source, AL_MIN_GAIN, &minimumGain);

    return minimumGain;
}

float QALSource::maximumGain() const
{
    float maximumGain;
    alGetSourcef(d->source, AL_MAX_GAIN, &maximumGain);

    return maximumGain;
}

float QALSource::gain() const
{
    float gain;
    alGetSourcef(d->source, AL_GAIN, &gain);

    return gain;
}

bool QALSource::isRelative() const
{
    ALint isRelative;
    alGetSourcei(d->source, AL_SOURCE_RELATIVE, &isRelative);

    return (isRelative == AL_TRUE) ? true : false;
}

QALSource::Type QALSource::type() const
{
    ALint type;
    alGetSourcei(d->source, AL_SOURCE_TYPE, &type);

    Type sourceType;

    switch (type) {
    case AL_STATIC:
        sourceType = Static;
        break;

    case AL_STREAMING:
        sourceType = Streaming;
        break;

    case AL_UNDETERMINED:
        sourceType = Undetermined;
        break;

    default:
        sourceType = Undetermined;
        break;
    }

    return sourceType;
}

bool QALSource::isLooping() const
{
    ALint isLooping;
    alGetSourcei(d->source, AL_LOOPING, &isLooping);

    return (isLooping == AL_TRUE) ? true : false;
}

int QALSource::currentBuffer() const
{
    ALint currentBuffer;
    alGetSourcei(d->source, AL_BUFFER, &currentBuffer);

    return currentBuffer;
}

int QALSource::queuedBuffers() const
{
    ALint queuedBuffers;
    alGetSourcei(d->source, AL_BUFFERS_QUEUED, &queuedBuffers);

    return queuedBuffers;
}

int QALSource::processedBuffers() const
{
    ALint processedBuffers;
    alGetSourcei(d->source, AL_BUFFERS_PROCESSED, &processedBuffers);

    return processedBuffers;
}

float QALSource::referenceDistance() const
{
    float referenceDistance;
    alGetSourcef(d->source, AL_REFERENCE_DISTANCE, &referenceDistance);

    return referenceDistance;
}

float QALSource::maximumDistance() const
{
    float maximumDistance;
    alGetSourcef(d->source, AL_MAX_DISTANCE, &maximumDistance);

    return maximumDistance;
}

float QALSource::rollOffFactor() const
{
    float rollOffFactor;
    alGetSourcef(d->source, AL_ROLLOFF_FACTOR, &rollOffFactor);

    return rollOffFactor;
}

float QALSource::pitch() const
{
    float pitch;
    alGetSourcef(d->source, AL_PITCH, &pitch);

    return pitch;
}

float QALSource::coneInnerAngle() const
{
    float coneInnerAngle;
    alGetSourcef(d->source, AL_CONE_INNER_ANGLE, &coneInnerAngle);

    return coneInnerAngle;
}

float QALSource::coneOuterAngle() const
{
    float coneOuterAngle;
    alGetSourcef(d->source, AL_CONE_OUTER_ANGLE, &coneOuterAngle);

    return coneOuterAngle;
}

float QALSource::coneOuterGain() const
{
    float coneOuterGain;
    alGetSourcef(d->source, AL_CONE_OUTER_GAIN, &coneOuterGain);

    return coneOuterGain;
}

float QALSource::secondOffset() const
{
    float secondOffset;
    alGetSourcef(d->source, AL_SEC_OFFSET, &secondOffset);

    return secondOffset;
}

float QALSource::sampleOffset() const
{
    float sampleOffset;
    alGetSourcef(d->source, AL_SAMPLE_OFFSET, &sampleOffset);

    return sampleOffset;
}

float QALSource::byteOffset() const
{
    float byteOffset;
    alGetSourcef(d->source, AL_BYTE_OFFSET, &byteOffset);

    return byteOffset;
}

void QALSource::setPosition(int px, int py, int pz)
{
    alSource3i(d->source, AL_POSITION, px, py, pz);
}

void QALSource::setPosition(float px, float py, float pz)
{
    alSource3f(d->source, AL_POSITION, px, py, pz);
}

void QALSource::setVelocity(int vx, int vy, int vz)
{
    alSource3i(d->source, AL_VELOCITY, vx, vy, vz);
}

void QALSource::setVelocity(float vx, float vy, float vz)
{
    alSource3f(d->source, AL_VELOCITY, vx, vy, vz);
}

void QALSource::setMinimumGain(float minimumGain)
{
    alSourcef(d->source, AL_MIN_GAIN, minimumGain);
}

void QALSource::setMaximumGain(float maximumGain)
{
    alSourcef(d->source, AL_MAX_GAIN, maximumGain);
}

void QALSource::setGain(float gain)
{
    alSourcef(d->source, AL_GAIN, gain);
}

void QALSource::setRelative(bool relative)
{
    alSourcei(d->source, AL_SOURCE_RELATIVE, relative);
}

void QALSource::setLooping(bool looping)
{
    alSourcei(d->source, AL_LOOPING, looping);
}

void QALSource::setCurrentBuffer(int currentBuffer)
{
    alSourcei(d->source, AL_BUFFER, currentBuffer);
}

void QALSource::setReferenceDistance(int referenceDistance)
{
    alSourcei(d->source, AL_REFERENCE_DISTANCE, referenceDistance);
}

void QALSource::setReferenceDistance(float referenceDistance)
{
    alSourcef(d->source, AL_REFERENCE_DISTANCE, referenceDistance);
}

void QALSource::setMaximumDistance(int maximumDistance)
{
    alSourcei(d->source, AL_MAX_DISTANCE, maximumDistance);
}

void QALSource::setMaximumDistance(float maximumDistance)
{
    alSourcef(d->source, AL_MAX_DISTANCE, maximumDistance);
}

void QALSource::setRollOffFactor(int rollOffFactor)
{
    alSourcei(d->source, AL_ROLLOFF_FACTOR, rollOffFactor);
}

void QALSource::setRollOffFactor(float rollOffFactor)
{
    alSourcef(d->source, AL_ROLLOFF_FACTOR, rollOffFactor);
}

void QALSource::setPitch(float pitch)
{
    alSourcef(d->source, AL_PITCH, pitch);
}

void QALSource::setConeInnerAngle(int coneInnerAngle)
{
    alSourcei(d->source, AL_CONE_INNER_ANGLE, coneInnerAngle);
}

void QALSource::setConeInnerAngle(float coneInnerAngle)
{
    alSourcef(d->source, AL_CONE_INNER_ANGLE, coneInnerAngle);
}

void QALSource::setConeOuterAngle(int coneOuterAngle)
{
    alSourcei(d->source, AL_CONE_OUTER_ANGLE, coneOuterAngle);
}

void QALSource::setConeOuterAngle(float coneOuterAngle)
{
    alSourcef(d->source, AL_CONE_OUTER_ANGLE, coneOuterAngle);
}

void QALSource::setConeOuterGain(float coneOuterGain)
{
    alSourcef(d->source, AL_CONE_OUTER_GAIN, coneOuterGain);
}

void QALSource::setSecondOffset(int secondOffset)
{
    alSourcei(d->source, AL_SEC_OFFSET, secondOffset);
}

void QALSource::setSecondOffset(float secondOffset)
{
    alSourcef(d->source, AL_SEC_OFFSET, secondOffset);
}

void QALSource::setSampleOffset(int sampleOffset)
{
    alSourcei(d->source, AL_SAMPLE_OFFSET, sampleOffset);
}

void QALSource::setSampleOffset(float sampleOffset)
{
    alSourcef(d->source, AL_SAMPLE_OFFSET, sampleOffset);
}

void QALSource::setByteOffset(int byteOffset)
{
    alSourcei(d->source, AL_BYTE_OFFSET, byteOffset);
}

void QALSource::setByteOffset(float byteOffset)
{
    alSourcef(d->source, AL_BYTE_OFFSET, byteOffset);
}
