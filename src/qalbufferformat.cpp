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

#include "qalbufferformat.h"

#include <QtCore/QString>
#include <QtCore/QDebug>

class QALBufferFormat::Private : public QSharedData
{
public:
    Private()
        : sampleType(QALBufferFormat::Unknown)
        , frequency(-1)
        , sampleSize(-1)
        , channels(QALBufferFormat::UnknownChannel)
    {
    }

    Private(const Private &other)
        : QSharedData(other)
        , codec(other.codec)
        , sampleType(other.sampleType)
        , frequency(other.frequency)
        , sampleSize(other.sampleSize)
        , channels(other.channels)
    {
    }

    Private& operator=(const Private &other)
    {
        codec = other.codec;
        sampleType = other.sampleType;
        frequency = other.frequency;
        sampleSize = other.sampleSize;
        channels = other.channels;

        return *this;
    }

    ~Private()
    {
    }

    QString codec;
    QALBufferFormat::SampleType sampleType;
    int frequency;
    int sampleSize;
    QALBufferFormat::Channels channels;
};

QALBufferFormat::QALBufferFormat()
    : d(new Private)
{
}

QALBufferFormat::QALBufferFormat(const QALBufferFormat &other):
    d(other.d)
{
}

QALBufferFormat::~QALBufferFormat()
{
}

QALBufferFormat& QALBufferFormat::operator=(const QALBufferFormat &other)
{
    d = other.d;
    return *this;
}

bool QALBufferFormat::operator==(const QALBufferFormat &other) const
{
    return d->frequency == other.d->frequency &&
            d->channels == other.d->channels &&
            d->sampleSize == other.d->sampleSize &&
            d->codec == other.d->codec &&
            d->sampleType == other.d->sampleType;
}

bool QALBufferFormat::operator!=(const QALBufferFormat& other) const
{
    return !(*this == other);
}

void QALBufferFormat::setSampleRate(int samplerate)
{
    d->frequency = samplerate;
}

void QALBufferFormat::setFrequency(int frequency)
{
    d->frequency = frequency;
}

int QALBufferFormat::sampleRate() const
{
    return d->frequency;
}

int QALBufferFormat::frequency() const
{
    return d->frequency;
}

void QALBufferFormat::setChannels(QALBufferFormat::Channels channels)
{
    d->channels = channels;
}

QALBufferFormat::Channels QALBufferFormat::channels() const
{
    return d->channels;
}

void QALBufferFormat::setSampleSize(int sampleSize)
{
    d->sampleSize = sampleSize;
}

int QALBufferFormat::sampleSize() const
{
    return d->sampleSize;
}

void QALBufferFormat::setCodec(const QString &codec)
{
    d->codec = codec;
}

QString QALBufferFormat::codec() const
{
    return d->codec;
}

void QALBufferFormat::setSampleType(QALBufferFormat::SampleType sampleType)
{
    d->sampleType = sampleType;
}

QALBufferFormat::SampleType QALBufferFormat::sampleType() const
{
    return d->sampleType;
}

ALenum sampleFormat(ALuint channels, ALuint bits, bool isFloat)
{
    ALenum format;
    if(!isFloat)
    {
        if(bits == 8)
        {
            if (channels == 1) {
                format = alGetEnumValue("AL_FORMAT_MONO8");
                if(alGetError() == AL_NO_ERROR && format != 0 && format != -1)
                    return format;
            } else if (channels == 2) {
                format = alGetEnumValue("AL_FORMAT_STEREO8");
                if(alGetError() == AL_NO_ERROR && format != 0 && format != -1)
                    return format;
            }

            if (alIsExtensionPresent("AL_EXT_MCFORMATS"))
            {
                switch (channels) {
                case 4:
                    format = alGetEnumValue("AL_FORMAT_QUAD8");
                    if(alGetError() == AL_NO_ERROR && format != 0 && format != -1)
                        return format;
                    break;
                case 6:
                    format = alGetEnumValue("AL_FORMAT_51CHN8");
                    if(alGetError() == AL_NO_ERROR && format != 0 && format != -1)
                        return format;
                    break;
                case 7:
                    format = alGetEnumValue("AL_FORMAT_61CHN8");
                    if(alGetError() == AL_NO_ERROR && format != 0 && format != -1)
                        return format;
                    break;
                case 8:
                    format = alGetEnumValue("AL_FORMAT_71CHN8");
                    if(alGetError() == AL_NO_ERROR && format != 0 && format != -1)
                        return format;
                    break;
                default:
                    break;
                }
            }

            if (alIsExtensionPresent("AL_LOKI_quadriphonic"))
            {
                if (channels == 4) {
                    format = alGetEnumValue("AL_FORMAT_QUAD8_LOKI");
                    if(alGetError() == AL_NO_ERROR && format != 0 && format != -1)
                        return format;
                }
            }

            qWarning() << Q_FUNC_INFO << "Unsupported 8-bit channel count";
            return AL_NONE;
        }

        if(bits == 16)
        {
            if (channels == 1) {
                format = alGetEnumValue("AL_FORMAT_MONO16");
                if(alGetError() == AL_NO_ERROR && format != 0 && format != -1)
                    return format;
            } else if (channels == 2) {
                format = alGetEnumValue("AL_FORMAT_STEREO16");
                if(alGetError() == AL_NO_ERROR && format != 0 && format != -1)
                    return format;
            }

            if (alIsExtensionPresent("AL_EXT_MCFORMATS"))
            {
                switch (channels) {
                case 4:
                    format = alGetEnumValue("AL_FORMAT_QUAD16");
                    if(alGetError() == AL_NO_ERROR && format != 0 && format != -1)
                        return format;
                    break;

                case 6:
                    format = alGetEnumValue("AL_FORMAT_51CHN16");
                    if(alGetError() == AL_NO_ERROR && format != 0 && format != -1)
                        return format;
                    break;

                case 7:
                    format = alGetEnumValue("AL_FORMAT_61CHN16");
                    if(alGetError() == AL_NO_ERROR && format != 0 && format != -1)
                        return format;
                    break;

                case 8:
                    format = alGetEnumValue("AL_FORMAT_71CHN16");
                    if(alGetError() == AL_NO_ERROR && format != 0 && format != -1)
                        return format;
                    break;
                }
            }

            if (alIsExtensionPresent("AL_LOKI_quadriphonic"))
            {
                if (channels == 4) {
                    format = alGetEnumValue("AL_FORMAT_QUAD16_LOKI");
                    if(alGetError() == AL_NO_ERROR && format != 0 && format != -1)
                        return format;
                }
            }

            qWarning() << Q_FUNC_INFO << "Unsupported 16-bit channel count";
            return AL_NONE;
        }

        qWarning() << Q_FUNC_INFO << "Unsupported PCM bit depth";
        return AL_NONE;
    }

    if(bits == 32 && alIsExtensionPresent("AL_EXT_FLOAT32"))
    {
        if (channels == 1) {
            format = alGetEnumValue("AL_FORMAT_MONO_FLOAT32");
            if(alGetError() == AL_NO_ERROR && format != 0 && format != -1)
                return format;
        } else if (channels == 2) {
            format = alGetEnumValue("AL_FORMAT_STEREO_FLOAT32");
            if(alGetError() == AL_NO_ERROR && format != 0 && format != -1)
                return format;
        }

        if (alIsExtensionPresent("AL_EXT_MCFORMATS"))
        {
            switch (channels) {
            case 4:
                format = alGetEnumValue("AL_FORMAT_QUAD32");
                if(alGetError() == AL_NO_ERROR && format != 0 && format != -1)
                    return format;
                break;

            case 6:
                format = alGetEnumValue("AL_FORMAT_51CHN32");
                if(alGetError() == AL_NO_ERROR && format != 0 && format != -1)
                    return format;
                break;

            case 7:
                format = alGetEnumValue("AL_FORMAT_61CHN32");
                if(alGetError() == AL_NO_ERROR && format != 0 && format != -1)
                    return format;
                break;

            case 8:
                format = alGetEnumValue("AL_FORMAT_71CHN32");
                if(alGetError() == AL_NO_ERROR && format != 0 && format != -1)
                    return format;
                break;
            }
        }

        qWarning() << Q_FUNC_INFO << "Unsupported float32 channel count";
        return AL_NONE;
    }

    if (bits == 64 && alIsExtensionPresent("AL_EXT_DOUBLE"))
    {
        if (channels == 1) {
            format = alGetEnumValue("AL_FORMAT_MONO_DOUBLE_EXT");
            if(alGetError() == AL_NO_ERROR && format != 0 && format != -1)
                return format;
        } else if (channels == 2) {
            format = alGetEnumValue("AL_FORMAT_STEREO_DOUBLE_EXT");
                    if(alGetError() == AL_NO_ERROR && format != 0 && format != -1)
                    return format;
        }

        qWarning() << Q_FUNC_INFO << "Unsupported double channel count";
        return AL_NONE;
    }

    qWarning() << Q_FUNC_INFO << "Unsupported float bit depth";

    return AL_NONE;
}

