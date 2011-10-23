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

class QALBufferFormatPrivate : public QSharedData
{
public:
    QALBufferFormatPrivate()
    {   
        frequency = -1; 
        channels = -1; 
        sampleSize = -1; 
        sampleType = QAudioFormat::Unknown;
    }   

    QAudioFormatPrivate(const QAudioFormatPrivate &other):
        QSharedData(other),
        codec(other.codec),
        sampleType(other.sampleType),
        frequency(other.frequency),
        channels(other.channels),
        sampleSize(other.sampleSize)
    {   
    }   

    QAudioFormatPrivate& operator=(const QAudioFormatPrivate &other)
    {   
        codec = other.codec;
        sampleType = other.sampleType;
        frequency = other.frequency;
        channels = other.channels;
        sampleSize = other.sampleSize;

        return *this;
    }   

    QString codec;
    QAudioFormat::SampleType sampleType;
    int frequency;
    int channels;
    int sampleSize;
};



QALBufferFormat::QALBufferFormat()
    : d(new QALBufferFormatPrivate)
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

void QALBufferFormat::setChannelCount(int channels)
{
    d->channels = channels;
}

void QALBufferFormat::setChannels(int channels)
{
    d->channels = channels;
}

int QALBufferFormat::channelCount() const
{
    return d->channels;
}

int QALBufferFormat::channels() const
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

void QALBufferFormat::setSampleType(QAudioFormat::SampleType sampleType)
{
    d->sampleType = sampleType;
}

QALBufferFormat::SampleType QALBufferFormat::sampleType() const
{
    return d->sampleType;
}

