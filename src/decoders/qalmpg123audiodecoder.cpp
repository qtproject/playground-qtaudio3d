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

#include "qalmpg123audiodecoder.h"

#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtCore/QDebug>
#include <QtCore/QMutex>

#include <mpg123.h>

class QALMpg123AudioDecoder::Private
{
    public:
        Private()
            : mpg123Handle(0)
        {
            mutex.lock();
            if (++referenceCounter == 1) {
                int error;
                if ((error = mpg123_init()) == MPG123_OK) {
                    mutex.unlock();
                    isValid = true;
                } else {
                    mutex.unlock();
                    qWarning() << Q_FUNC_INFO << "Failed to initialize the mpg123 library:" << error;
                }
            }
        }

        ~Private()
        {
            mutex.lock();
            if (--referenceCounter == 0) {
                mpg123_exit();
            }
            mutex.unlock();
        }

        static sf_count_t fileLengthCallback(void *user_data);
        static sf_count_t seekCallback(sf_count_t offset, int whence, void *user_data);
        static sf_count_t readCallback(void *ptr, sf_count_t count, void *user_data);
        static sf_count_t tellCallback(void *user_data);

        static int referenceCounter;
        static bool isValid;

        QMutex mutex;

        QFile file;

        QByteArray encodedData;
        mpg123_handle *mpg123Handle;
};

bool QALMpg123AudioDecoder::Private::referenceCounter = 0;
bool QALMpg123AudioDecoder::Private::isValid = false;

sf_count_t
QALMpg123AudioDecoder::Private::fileLengthCallback(void *user_data)
{
    return reinterpret_cast<QALMpg123AudioDecoder::Private*>(user_data)->file.size();
}

sf_count_t
QALMpg123AudioDecoder::Private::seekCallback(sf_count_t offset, int whence, void *user_data)
{
    QFile &tmpFile = reinterpret_cast<QALMpg123AudioDecoder::Private*>(user_data)->file;
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

sf_count_t
QALMpg123AudioDecoder::Private::readCallback(void *ptr, sf_count_t count, void *user_data)
{
    return reinterpret_cast<QALMpg123AudioDecoder::Private*>(user_data)->file.read(reinterpret_cast<char*>(ptr), count);
}

sf_count_t
QALMpg123AudioDecoder::Private::tellCallback(void *user_data)
{
    return reinterpret_cast<QALMpg123AudioDecoder::Private*>(user_data)->file.pos();
}

QALMpg123AudioDecoder::QALMpg123AudioDecoder()
    : d(new Private)
{
}

QALMpg123AudioDecoder::~QALMpg123AudioDecoder()
{
}

bool
QALMpg123AudioDecoder::open(const QFile &file)
{
    return open(file.fileName());
}

bool
QALMpg123AudioDecoder::open(const QUrl &fileUrl)
{
    return open(fileUrl.toLocalFile());
}

bool
QALMpg123AudioDecoder::open(const QString &fileName)
{
    d->file.setFileName(fileName);

    mpg123Handle = mpg123_new(NULL, NULL);

    SF_INFO sfInfo;
    sfInfo.format = 0;

    SF_VIRTUAL_IO sfVirtualIO;
    sfVirtualIO.get_filelen = &d->fileLengthCallback;
    sfVirtualIO.seek = &d->seekCallback;
    sfVirtualIO.read = &d->readCallback;
    sfVirtualIO.write = 0;
    sfVirtualIO.tell = &d->tellCallback;

    if ((d->sndFile = sf_open_virtual(&sfVirtualIO, SFM_READ, &sfInfo, d)) == 0) {
        qWarning() << Q_FUNC_INFO << "Failed to open the file" << fileName.toUtf8().constData() << "for decoding:" << sf_strerror(d->sndFile);
        return false;
    }

    d->sfInfo = sfInfo;

    return true;
}

qint64
QALMpg123AudioDecoder::pos()
{
    int position;
    if ((position = sf_seek(d->sndFile, 0, SEEK_CUR)) == -1) {
        qWarning() << Q_FUNC_INFO << "Failed to tell the current position:" << sf_strerror(d->sndFile);
    }

    return position;
}

bool
QALMpg123AudioDecoder::seek(qint64 pos)
{
    if (sf_seek(d->sndFile, pos, SEEK_SET) == -1) {
        qWarning() << Q_FUNC_INFO << "Failed to seek in the file:" << sf_strerror(d->sndFile);
        return false;
    }

    return true;
}

bool
QALMpg123AudioDecoder::close()
{
    mpg123_delete_close(d->mp3Handle);
    d->mp3Handle = 0;

    return true;
}

void
QALMpg123AudioDecoder::setEncodedData(const QByteArray &encodedData)
{
    d->encodedData = encodedData;
}

int
QALMpg123AudioDecoder::channels() const
{
    return d->sfInfo.channels;
}

int
QALMpg123AudioDecoder::sampleRate() const
{
    return d->sfInfo.samplerate;
}

int
QALMpg123AudioDecoder::sampleSize() const
{
    return 16;
}

QByteArray
QALMpg123AudioDecoder::decode(qint64 maxlen)
{
    QByteArray result;
    result.reserve(maxlen);

    char *decodedData = result.data();

    if (maxlen != decode(decodedData, maxlen))
        qWarning() << Q_FUNC_INFO << "Could not to decode all the data:" << maxlen;

    return result;
}

qint64
QALMpg123AudioDecoder::decode(char *decodedData, qint64 maxlen)
{
    return sf_readf_short(d->sndFile, reinterpret_cast<short*>(decodedData), maxlen);
}
