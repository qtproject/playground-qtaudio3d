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

#include "qalvorbisfileaudiodecoder.h"

#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtCore/QDebug>

#include <vorbis/vorbisfile.h>

class QALVorbisFileAudioDecoder::Private
{
    public:
        Private()
        {
        }

        ~Private()
        {
        }

        // static sf_count_t fileLengthCallback(void *user_data);
        static int seekCallback(void *datasource, ogg_int64_t offset, int whence);
        static size_t readCallback(void *ptr, size_t size, size_t nmemb, void *datasource);
        static long tellCallback(void *datasource);
        static int closeCallback(void *datasource);

        QFile file;

        QByteArray encodedData;
        OggVorbis_File* oggVorbisFile;
};

int
QALVorbisFileAudioDecoder::Private::closeCallback(void *datasource)
{
    reinterpret_cast<QALVorbisFileAudioDecoder::Private*>(datasource)->file.close();
    return 0;
}

int
QALVorbisFileAudioDecoder::Private::seekCallback(void *datasource, ogg_int64_t offset, int whence)
{
    QFile &tmpFile = reinterpret_cast<QALVorbisFileAudioDecoder::Private*>(datasource)->file;
    switch (whence) {
    case SEEK_SET:
        if (tmpFile.seek(offset) == true)
            return offset;

        break;

    case SEEK_CUR:
        offset += tmpFile.pos();
        if (tmpFile.seek(offset) == true)
            return offset;

        break;

    case SEEK_END:
          offset += tmpFile.size();
          if (tmpFile.seek(offset) == true)
              return offset;

          break;

    default:
          qWarning() << Q_FUNC_INFO << "Failed to seek the file:" << tmpFile.fileName() << "Invalid whence value:" << whence;
          break;
    }

    qWarning() << Q_FUNC_INFO << "Failed to seek the file:" << tmpFile.fileName();
    return -1;
}

size_t
QALVorbisFileAudioDecoder::Private::readCallback(void *ptr, size_t size, size_t nmemb, void *datasource)
{
    return reinterpret_cast<QALVorbisFileAudioDecoder::Private*>(datasource)->file.read(reinterpret_cast<char*>(ptr), size * nmemb);
}

long
QALVorbisFileAudioDecoder::Private::tellCallback(void *datasource)
{
    return reinterpret_cast<QALVorbisFileAudioDecoder::Private*>(datasource)->file.pos();
}

QALVorbisFileAudioDecoder::QALVorbisFileAudioDecoder()
    : d(new Private)
{
}

QALVorbisFileAudioDecoder::~QALVorbisFileAudioDecoder()
{
}

bool
QALVorbisFileAudioDecoder::open(const QFile &file)
{
    return open(file.fileName());
}

bool
QALVorbisFileAudioDecoder::open(const QUrl &fileUrl)
{
    return open(fileUrl.toLocalFile());
}

bool
QALVorbisFileAudioDecoder::open(const QString &fileName)
{
    d->file.setFileName(fileName);

    ov_callbacks oggVorbisCallbacks;
    oggVorbisCallbacks.close_func = &d->fileLengthCallback;
    oggVorbisCallbacks.seek_func = &d->seekCallback;
    oggVorbisCallbacks.read_func = &d->readCallback;
    oggVorbisCallbacks.tell_func = &d->tellCallback;

    int error = 0;
    if ((error = ov_open_callbacks(d, d->oggVorbisFile, 0, 0, &oggVofbisCallback) < 0)) {
        qWarning() << Q_FUNC_INFO << "Failed to open the file" << fileName.toUtf8().constData() << "for decoding:" << error;
        return false;
    }

    return true;
}

qint64
QALVorbisFileAudioDecoder::pos()
{
    int position;
    if ((position = sf_seek(d->sndFile, 0, SEEK_CUR)) == -1) {
        qWarning() << Q_FUNC_INFO << "Failed to tell the current position:" << sf_strerror(d->sndFile);
    }

    return position;
}

bool
QALVorbisFileAudioDecoder::seek(qint64 pos)
{
    if (sf_seek(d->sndFile, pos, SEEK_SET) == -1) {
        qWarning() << Q_FUNC_INFO << "Failed to seek in the file:" << sf_strerror(d->sndFile);
        return false;
    }

    return true;
}

bool
QALVorbisFileAudioDecoder::close()
{
    if (ov_clear(d->oggVorbisFile)) {
        qWarning() << Q_FUNC_INFO << "Failed to close the file:" <<  sf_strerror(d->sndFile);
        return false;
    }

    return true;
}

void
QALVorbisFileAudioDecoder::setEncodedData(const QByteArray &encodedData)
{
    d->encodedData = encodedData;
}

int
QALVorbisFileAudioDecoder::channels() const
{
    return d->sfInfo.channels;
}

int
QALVorbisFileAudioDecoder::sampleRate() const
{
    return d->sfInfo.samplerate;
}

int
QALVorbisFileAudioDecoder::sampleSize() const
{
    return 16;
}

QByteArray
QALVorbisFileAudioDecoder::decode(qint64 maxlen)
{
    QByteArray result;
    result.reserve(maxlen);

    char *decodedData = result.data();

    if (maxlen != decode(decodedData, maxlen))
        qWarning() << Q_FUNC_INFO << "Could not to decode all the data:" << maxlen;

    return result;
}

qint64
QALVorbisFileAudioDecoder::decode(char *decodedData, qint64 maxlen)
{
    return sf_readf_short(d->sndFile, reinterpret_cast<short*>(decodedData), maxlen);
}
