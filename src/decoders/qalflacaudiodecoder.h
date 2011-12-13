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

#ifndef QALFLACAUDIODECODER_H
#define QALFLACAUDIODECODER_H

#include "q_openal_export.h"

#include "qalabstractaudiodecoder.h"

class Q_OPENAL_EXPORT QALFlacAudioDecoder : public QALAbstractAudioDecoder
{
    //Q_DECLARE_PRIVATE(QALFlacAudioDecoder)
public:
    QALFlacAudioDecoder();
    virtual ~QALFlacAudioDecoder();

    bool open(const QString &fileName);

    qint64 pos();

    bool seek(qint64 pos);

    bool close();

    void setEncodedData(const QByteArray &encodedData);

    int channels() const;
    int sampleRate() const;
    int sampleSize() const;

    qint64 decodeData(char *decodedData, qint64 maxlen);

    private:
        class Private;
        Private *const d;
};

#endif // QALFLACAUDIODECODER_H
