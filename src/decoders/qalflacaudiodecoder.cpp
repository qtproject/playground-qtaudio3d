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

#include "qalflacaudiodecoder.h"

#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtCore/QDebug>

#include <FLAC/stream_decoder.h>

class QALFlacAudioDecoder::Private
{
    public:
        Private()
            : flacStreamDecoder(0)
        {
        }

        ~Private()
        {
        }

        static FLAC__StreamDecoderReadStatus readCallback(const FLAC__StreamDecoder *decoder, FLAC__byte buffer[], size_t *bytes, void *client_data);
        static FLAC__StreamDecoderSeekStatus seekCallback(const FLAC__StreamDecoder *decoder, FLAC__uint64 absolute_byte_offset, void *client_data);
        static FLAC__StreamDecoderTellStatus tellCallback(const FLAC__StreamDecoder *decoder, FLAC__uint64 *absolute_byte_offset, void *client_data);
        static FLAC__StreamDecoderLengthStatus lengthCallback(const FLAC__StreamDecoder *decoder, FLAC__uint64 *stream_length, void *client_data);
        static FLAC__bool eofCallback(const FLAC__StreamDecoder *decoder, void *client_data);
        static FLAC__StreamDecoderWriteStatus writeCallback(const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 *const buffer[], void *client_data);
        static void metadataCallback(const FLAC__StreamDecoder *decoder, const FLAC__StreamMetadata *metadata, void *client_data);
        static void errorCallback(const FLAC__StreamDecoder *decoder, FLAC__StreamDecoderErrorStatus status, void *client_data);

        QFile file;

        QByteArray encodedData;
        FLAC__StreamDecoder *flacStreamDecoder;
};

FLAC__StreamDecoderReadStatus
QALFlacAudioDecoder::Private::readCallback(const FLAC__StreamDecoder *decoder, FLAC__byte buffer[], size_t *bytes, void *client_data)
{
    Q_UNUSED(decoder)

    if (*bytes <= 0)
        return FLAC__STREAM_DECODER_READ_STATUS_ABORT;

    int retval;
    if ((retval = reinterpret_cast<QALFlacAudioDecoder::Private*>(client_data)->file.read(reinterpret_cast<char*>(buffer), *bytes)) == -1) {
        qWarning() << Q_FUNC_INFO << "Failed to read the data from the file";
        return FLAC__STREAM_DECODER_READ_STATUS_ABORT;
    }

    if (retval != 0)
        return FLAC__STREAM_DECODER_READ_STATUS_CONTINUE;

    return FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM;
}

FLAC__StreamDecoderSeekStatus
QALFlacAudioDecoder::Private::seekCallback(const FLAC__StreamDecoder *decoder, FLAC__uint64 absolute_byte_offset, void *client_data)
{
    Q_UNUSED(decoder)

    if (reinterpret_cast<QALFlacAudioDecoder::Private*>(client_data)->file.seek(absolute_byte_offset) == false) {
         qWarning() << Q_FUNC_INFO << "Failed to seek in the file";
         return FLAC__STREAM_DECODER_SEEK_STATUS_ERROR;
    }

    return FLAC__STREAM_DECODER_SEEK_STATUS_OK;
}

FLAC__StreamDecoderTellStatus
QALFlacAudioDecoder::Private::tellCallback(const FLAC__StreamDecoder *decoder, FLAC__uint64 *absolute_byte_offset, void *client_data)
{
    Q_UNUSED(decoder)

    *absolute_byte_offset = reinterpret_cast<QALFlacAudioDecoder::Private*>(client_data)->file.pos();

    return FLAC__STREAM_DECODER_TELL_STATUS_OK;
}

FLAC__StreamDecoderLengthStatus
QALFlacAudioDecoder::Private::lengthCallback(const FLAC__StreamDecoder *decoder, FLAC__uint64 *stream_length, void *client_data)
{
    Q_UNUSED(decoder)

    *stream_length = reinterpret_cast<QALFlacAudioDecoder::Private*>(client_data)->file.size();

    return FLAC__STREAM_DECODER_LENGTH_STATUS_OK;
}

QALFlacAudioDecoder::QALFlacAudioDecoder()
    : d(new Private)
{
}

QALFlacAudioDecoder::~QALFlacAudioDecoder()
{
}

bool
QALFlacAudioDecoder::open(const QFile &file)
{
    return open(file.fileName());
}

bool
QALFlacAudioDecoder::open(const QUrl &fileUrl)
{
    return open(fileUrl.toLocalFile());
}

bool
QALFlacAudioDecoder::open(const QString &fileName)
{
    d->file.setFileName(fileName);

    d->flacStreamDecoder = *FLAC__stream_decoder_new();
    if (d->flacStreamDecoder == 0) {
        qWarning() << Q_FUNC_INFO << "Could not allocate enough memory for the flac stream decoder handle";
        return false;
    }

    if (FLAC__stream_decoder_init_stream(d->flacStreamDecoder, &d->readCallback, &d->seekCallback,
                                         d->tellCallback, d->lengthCallback, d->eofCallback, d->writeCallback,
                                         d->metadataCallback, d->errorCallback, d) == FLAC__STREAM_DECODER_INIT_STATUS_OK)
    {   
        if(InitFlac())
        {   
            // all ok
            return;
        }   

        FLAC__stream_decoder_finish(flacFile);
    }   

    if ((d->sndFile = sf_open_virtual(&sfVirtualIO, SFM_READ, &sfInfo, d)) == 0) {
        qWarning() << Q_FUNC_INFO << "Failed to open the file" << fileName.toUtf8().constData() << "for decoding:" << sf_strerror(d->sndFile);
        return false;
    }

    d->sfInfo = sfInfo;

    return true;
}

qint64
QALFlacAudioDecoder::pos()
{
    int position;
    if ((position = sf_seek(d->sndFile, 0, SEEK_CUR)) == -1) {
        qWarning() << Q_FUNC_INFO << "Failed to tell the current position:" << sf_strerror(d->sndFile);
    }

    return position;
}

bool
QALFlacAudioDecoder::seek(qint64 pos)
{
    if (FLAC__stream_decoder_seek_absolute(d->flacStreamDecoder, pos) == false) {
        qWarning() << Q_FUNC_INFO << "Failed to seek in the file";
        return false;
    }

    return true;
}

bool
QALFlacAudioDecoder::close()
{
    FLAC__stream_decoder_delete(d->flacStreamDecoder);
    d->flacStreamDecoder = 0;

    return true;
}

void
QALFlacAudioDecoder::setEncodedData(const QByteArray &encodedData)
{
    d->encodedData = encodedData;
}

int
QALFlacAudioDecoder::channels() const
{
    return FLAC__stream_decoder_get_channels(d->flacStreamDecoder);
}

int
QALFlacAudioDecoder::sampleRate() const
{
    return FLAC__stream_decoder_get_sample_rate(d->flacStreamDecoder);
}

int
QALFlacAudioDecoder::sampleSize() const
{
    return FLAC__stream_decoder_get_bits_per_sample(d->flacStreamDecoder);
}

QByteArray
QALFlacAudioDecoder::decode(qint64 maxlen)
{
    QByteArray result;
    result.reserve(maxlen);

    char *decodedData = result.data();

    if (maxlen != decode(decodedData, maxlen))
        qWarning() << Q_FUNC_INFO << "Could not to decode all the data:" << maxlen;

    return result;
}

qint64
QALFlacAudioDecoder::decode(char *decodedData, qint64 maxlen)
{
    if (FLAC__stream_decoder_get_state(d->flacStreamDecoder) == FLAC__STREAM_DECODER_SEEK_ERROR
        && FLAC__stream_decoder_reset(d->flacStreamDecoder) == false)
        {
            qWarning() << Q_FUNC_INFO << "Failed to allocate memory while resetting before decoding";;
            return false;
        }
    }

    return sf_readf_short(d->sndFile, reinterpret_cast<short*>(decodedData), maxlen);
}
