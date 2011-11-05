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

#ifndef QALBUFFERFORMAT_H
#define QALBUFFERFORMAT_H

#include "q_openal_export.h"

#include <QtCore/QSharedPointer>

class Q_OPENAL_EXPORT QALBufferFormat
{
public:
    enum SampleType { Unknown, SignedInt, UnSignedInt, Float };
    enum ChannelFlag {
        UnknownChannel      = 0,
        FrontLeft           = 1,
        FrontRight          = 1 << 1,
        FrontCenter         = 1 << 2,
        LowFrequency        = 1 << 3,
        BackLeft            = 1 << 4,
        BackRight           = 1 << 5,
        FrontLeftOfCenter   = 1 << 6,
        FrontRightOfCenter  = 1 << 7,
        BackCenter          = 1 << 8,
        SideLeft            = 1 << 9,
        SideRight           = 1 << 10,
        TopCenter           = 1 << 11,
        TopFrontLeft        = 1 << 12,
        TopFrontCenter      = 1 << 13,
        TopFrontRight       = 1 << 14,
        TopBackLeft         = 1 << 15,
        TopBackCenter       = 1 << 16,
        TopBackRight        = 1 << 17,
    };

    Q_DECLARE_FLAGS(Channels, ChannelFlag)

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

    void setChannels(QALBufferFormat::Channels channels);
    QALBufferFormat::Channels channels() const;

    void setSampleSize(int sampleSize);
    int sampleSize() const;

    void setCodec(const QString &codec);
    QString codec() const;

    void setSampleType(QALBufferFormat::SampleType sampleType);
    QALBufferFormat::SampleType sampleType() const;

private:
    class Private;
    QSharedDataPointer<Private> d;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QALBufferFormat::Channels)

#endif
