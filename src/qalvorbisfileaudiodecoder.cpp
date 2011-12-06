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
            : bitStream(0)
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
        OggVorbis_File oggVorbisFile;
        vorbis_info vorbisInfo;

        int bitStream;
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
    oggVorbisCallbacks.close_func = &d->closeCallback;
    oggVorbisCallbacks.read_func = &d->readCallback;
    oggVorbisCallbacks.seek_func = &d->seekCallback;
    oggVorbisCallbacks.tell_func = &d->tellCallback;

    int error;
    if ((error = ov_open_callbacks(d, &d->oggVorbisFile, 0, 0, oggVorbisCallbacks) < 0)) {
        qWarning() << Q_FUNC_INFO << "Failed to open the file" << fileName.toUtf8().constData() << "for decoding:" << error;
        return false;
    }

    d->vorbisInfo = *ov_info(&d->oggVorbisFile, -1);

    return true;
}

qint64
QALVorbisFileAudioDecoder::pos()
{
    int retval;
    if ((retval = ov_pcm_tell(&d->oggVorbisFile)) == OV_EINVAL) {
        qWarning() << Q_FUNC_INFO << "Failed to tell the current position because the argument was invalid. The requested bitstream did not exist.";
    }

    return retval;
}

bool
QALVorbisFileAudioDecoder::seek(qint64 pos)
{
    int retval;
    if ((retval = ov_pcm_seek(&d->oggVorbisFile, pos) < 0)) {
        qWarning() << Q_FUNC_INFO << "Failed to seek in the file:" << retval;
        return false;
    }

    return true;
}

bool
QALVorbisFileAudioDecoder::close()
{
    int error;
    if ((error = ov_clear(&d->oggVorbisFile))) {
        qWarning() << Q_FUNC_INFO << "Failed to close the file:" << error;
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
    return d->vorbisInfo.channels;
}

int
QALVorbisFileAudioDecoder::sampleRate() const
{
    return d->vorbisInfo.rate;
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
#if Q_BYTE_ORDER == Q_BIG_ENDIAN
    return ov_read(&d->oggVorbisFile, decodedData, maxlen, 1, 2, 1, &d->bitStream);
#elif Q_BYTE_ORDER == Q_LITTLE_ENDIAN
    return ov_read(&d->oggVorbisFile, decodedData, maxlen, 0, 2, 1, &d->bitStream);
#endif
}
