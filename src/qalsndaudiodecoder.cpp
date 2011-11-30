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

#include "qalsndaudiodecoder.h"

#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtCore/QDebug>

#include <sndfile.h>

class QALSndAudioDecoder::Private
{
    public:
        Private()
        {
        }

        ~Private()
        {
        }

        static sf_count_t seekCallback(sf_count_t offset, int whence, void *user_data);
        static sf_count_t readCallback(void *ptr, sf_count_t count, void *user_data);
        static sf_count_t tellCallback(void *user_data);

        static QFile file;

        QByteArray encodedData;
        SNDFILE *sndFile;
};

sf_count_t
QALSndAudioDecoder::Private::seekCallback(sf_count_t offset, int whence, void *user_data)
{
    sf_count_t tmpOffset;
    switch (whence) {
    case SEEK_SET:
        if (reinterpret_cast<QALSndAudioDecoder::Private*>(user_data)->file.seek(offset) == true)
            return offset;

        break;

    case SEEK_CUR:
        tmpOffset = file.pos() + offset;
        if (reinterpret_cast<QALSndAudioDecoder::Private*>(user_data)->file.seek(tmpOffset) == true)
            return tmpOffset;

        break;

    case SEEK_END:
          tmpOffset = file.size() + offset;
          if (reinterpret_cast<QALSndAudioDecoder::Private*>(user_data)->file.seek(tmpOffset) == true)
              return tmpOffset;

          break;

    default:
          qWarning() << Q_FUNC_INFO << "Failed to see the file:" << file.fileName() << "Invalid whence value:" << whence;
          break;
    }

    qWarning() << Q_FUNC_INFO << "Failed to seek the file:" << file.fileName();
    return -1;
}

sf_count_t
QALSndAudioDecoder::Private::readCallback(void *ptr, sf_count_t count, void *user_data)
{
    return reinterpret_cast<QALSndAudioDecoder::Private*>(user_data)->file.read(reinterpret_cast<char*>(ptr), count);
}

sf_count_t
QALSndAudioDecoder::Private::tellCallback(void *user_data)
{
    return reinterpret_cast<QALSndAudioDecoder::Private*>(user_data)->file.pos();
}

QALSndAudioDecoder::QALSndAudioDecoder()
    : d(new Private)
{
}

QALSndAudioDecoder::~QALSndAudioDecoder()
{
}

bool
QALSndAudioDecoder::open(const QFile &file)
{
    return open(file.fileName());
}

bool
QALSndAudioDecoder::open(const QUrl &fileUrl)
{
    return open(fileUrl.toLocalFile());
}

bool
QALSndAudioDecoder::open(const QString &fileName)
{
    SF_INFO sfInfo;
    sfInfo.format = 0;
    if ((d->sndFile = sf_open(fileName.toUtf8().constData(), SFM_READ, &sfInfo)) == 0) {
        qWarning() << Q_FUNC_INFO << "Failed to open the file" << fileName.toUtf8().constData() << "for decoding:" << sf_strerror(d->sndFile);
        return false;
    }

    return true;
}

qint64
QALSndAudioDecoder::pos()
{
    int position;
    if ((position = sf_seek(d->sndFile, 0, SEEK_CUR)) == -1) {
        qWarning() << Q_FUNC_INFO << "Failed to tell the current position:" << sf_strerror(d->sndFile);
    }

    return position;
}

bool
QALSndAudioDecoder::seek(qint64 pos)
{
    if (sf_seek(d->sndFile, pos, SEEK_SET) == -1) {
        qWarning() << Q_FUNC_INFO << "Failed to seek in the file:" << sf_strerror(d->sndFile);
        return false;
    }

    return true;
}

bool
QALSndAudioDecoder::close()
{
    if (sf_close(d->sndFile)) {
        qWarning() << Q_FUNC_INFO << "Failed to close the file:" <<  sf_strerror(d->sndFile);
        return false;
    }

    return true;
}

void
QALSndAudioDecoder::setEncodedData(const QByteArray &encodedData)
{
    d->encodedData = encodedData;
}

QByteArray
QALSndAudioDecoder::decode(qint64 maxlen)
{
    QByteArray result;
    result.reserve(maxlen);

    char *decodedData = result.data();

    if (maxlen != decode(decodedData, maxlen))
        qWarning() << Q_FUNC_INFO << "Could not to decode all the data:" << maxlen;

    return result;
}

qint64
QALSndAudioDecoder::decode(char *decodedData, qint64 maxlen)
{
    return sf_readf_short(d->sndFile, reinterpret_cast<short*>(decodedData), maxlen);
}
