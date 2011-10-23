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

class Q_OPENAL_EXPORT QALBufferFormat
{
public:
    enum SampleType { Unknown, SignedInt, UnSignedInt, Float };

    QALBufferFormat();
    QALBufferFormat(const QALBufferFormat &other);
    ~QALBufferFormat();

    QALBufferFormat& operator=(const QALBufferFormat &other);
    bool operator==(const QALBufferFormat &other) const;
    bool operator!=(const QALBufferFormat &other) const;

    bool isValid() const;

    void setFrequency(int frequency);
    int frequency() const;
    void setSampleRate(int sampleRate);
    int sampleRate() const;

    void setChannels(int channels);
    int channels() const;
    void setChannelCount(int channelCount);
    int channelCount() const;

    void setSampleSize(int sampleSize);
    int sampleSize() const;

    void setCodec(const QString &codec);
    QString codec() const;

    void setSampleType(QAudioFormat::SampleType sampleType);
    QAudioFormat::SampleType sampleType() const;

private:
    QSharedDataPointer<QALBufferFormatPrivate> d;
};

