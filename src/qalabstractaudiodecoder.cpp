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

#include "qalabstractaudiodecoder.h"

#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QUrl>
#include <QtCore/QDebug>

QALAbstractAudioDecoder::QALAbstractAudioDecoder()
{
}

QALAbstractAudioDecoder::~QALAbstractAudioDecoder()
{
}

bool
QALAbstractAudioDecoder::rewind()
{
    return seek(0);
}

bool
QALAbstractAudioDecoder::open(const QFile &file)
{
    return open(file.fileName());
}

bool
QALAbstractAudioDecoder::open(const QUrl &fileUrl)
{
    return open(fileUrl.toLocalFile());
}

QByteArray
QALAbstractAudioDecoder::decode(qint64 maxlen)
{
    QByteArray result;
    result.reserve(maxlen);

    char *decodedData = result.data();

    if (maxlen != decodeData(decodedData, maxlen))
        qWarning() << Q_FUNC_INFO << "Could not to decode all the data:" << maxlen;

    return result;
}

