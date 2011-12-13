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

#ifndef QALABSTRACTAUDIODECODER_H
#define QALABSTRACTAUDIODECODER_H

#include "q_openal_export.h"

class QString;
class QFile;
class QUrl;

class Q_OPENAL_EXPORT QALAbstractAudioDecoder
{
    //Q_DECLARE_PRIVATE(QALAbstractAudioDecoder)
public:
    QALAbstractAudioDecoder();
    virtual ~QALAbstractAudioDecoder();

    virtual bool open(const QFile &file);
    virtual bool open(const QUrl &fileUrl);
    virtual bool open(const QString &fileName) = 0;

    virtual qint64 pos() = 0;

    virtual bool seek(qint64 pos) = 0;

    virtual bool rewind();

    virtual bool close() = 0;

    virtual void setEncodedData(const QByteArray &encodedData) = 0;

    virtual int channels() const = 0;
    virtual int sampleRate() const = 0;
    virtual int sampleSize() const = 0;

    virtual QByteArray decode(qint64 maxlen);
    virtual qint64 decode(char *decodedData, qint64 maxlen) = 0;
};

#endif
