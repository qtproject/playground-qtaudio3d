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
#include <QtCore/QtGlobal>

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
        QVector<ALubyte> initialData;
};

FLAC__StreamDecoderReadStatus
QALFlacAudioDecoder::Private::readCallback(const FLAC__StreamDecoder *decoder, FLAC__byte buffer[], size_t *bytes, void *client_data)
{
    Q_UNUSED(decoder)

    if (*bytes <= 0)
        return FLAC__STREAM_DECODER_READ_STATUS_ABORT;

    if ((*bytes = reinterpret_cast<QALFlacAudioDecoder::Private*>(client_data)->file.read(reinterpret_cast<char*>(buffer), *bytes)) == -1) {
        qWarning() << Q_FUNC_INFO << "Failed to read the data from the file";
        return FLAC__STREAM_DECODER_READ_STATUS_ABORT;
    }

    if (*bytes != 0)
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

    QFile &tmpFile = reinterpret_cast<QALFlacAudioDecoder::Private*>(client_data)->file;
    tmpFile.unsetError();

    *absolute_byte_offset = tmpFile.pos();

    if (tmpFile.error() != QFile::NoError) {
        qWarning() << Q_FUNC_INFO << "Failed to tell the current position:" << tmpFile.errorString();
        return FLAC__STREAM_DECODER_TELL_STATUS_ERROR;
    }

    return FLAC__STREAM_DECODER_TELL_STATUS_OK;
}

FLAC__StreamDecoderLengthStatus
QALFlacAudioDecoder::Private::lengthCallback(const FLAC__StreamDecoder *decoder, FLAC__uint64 *stream_length, void *client_data)
{
    Q_UNUSED(decoder)

    QFile &tmpFile = reinterpret_cast<QALFlacAudioDecoder::Private*>(client_data)->file;
    tmpFile.unsetError();

    *stream_length = tmpFile.size();

    if (tmpFile.error() != QFile::NoError) {
        qWarning() << Q_FUNC_INFO << "Failed to get the total length of the stream in bytes:" << tmpFile.errorString();
        return FLAC__STREAM_DECODER_LENGTH_STATUS_ERROR;
    }

    return FLAC__STREAM_DECODER_LENGTH_STATUS_OK;
}

FLAC__bool
QALFlacAudioDecoder::Private::eofCallback(const FLAC__StreamDecoder *decoder, void *client_data)
{
    Q_UNUSED(decoder)

    return reinterpret_cast<QALFlacAudioDecoder::Private*>(client_data)->file.atEnd();
}

FLAC__StreamDecoderWriteStatus
QALFlacAudioDecoder::Private::writeCallback(const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame,
                                             const FLAC__int32 *const buffer[], void *client_data)
{
    Q_UNUSED(decoder)
    Q_UNUSED(frame)
    Q_UNUSED(buffer)
    Q_UNUSED(client_data)

    return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
}

void
QALFlacAudioDecoder::Private::metadataCallback(const FLAC__StreamDecoder *decoder, const FLAC__StreamMetadata *metadata, void *client_data)
{
    Q_UNUSED(decoder)
    Q_UNUSED(metadata)
    Q_UNUSED(client_data)
}

void
QALFlacAudioDecoder::Private::errorCallback(const FLAC__StreamDecoder *decoder, FLAC__StreamDecoderErrorStatus status, void *client_data)
{
    Q_UNUSED(decoder)
    Q_UNUSED(status)
    Q_UNUSED(client_data)
}

QALFlacAudioDecoder::QALFlacAudioDecoder()
    : d(new Private)
{
}

QALFlacAudioDecoder::~QALFlacAudioDecoder()
{
}

bool
QALFlacAudioDecoder::open(const QString &fileName)
{
    d->file.setFileName(fileName);

    d->flacStreamDecoder = FLAC__stream_decoder_new();
    if (d->flacStreamDecoder == 0) {
        qWarning() << Q_FUNC_INFO << "Could not allocate enough memory for the flac stream decoder handle";
        return false;
    }

    if (FLAC__stream_decoder_init_stream(d->flacStreamDecoder, &d->readCallback, &d->seekCallback,
                                         d->tellCallback, d->lengthCallback, d->eofCallback, d->writeCallback,
                                         d->metadataCallback, d->errorCallback, d) == FLAC__STREAM_DECODER_INIT_STATUS_OK)
    {
        // outBytes = NULL;
        // outMax = 0;
        // outLen = 0;
        while (d->initialData.size() == 0)
        {   
            if (FLAC__stream_decoder_process_single(d->flacStreamDecoder) == false
                || FLAC__stream_decoder_get_state(d->flacStreamDecoder) == FLAC__STREAM_DECODER_END_OF_STREAM)
                break;
        }   

        if (d->initialData.size() > 0)
            return true;

        FLAC__stream_decoder_finish(d->flacStreamDecoder);
    }   

    return true;
}

qint64
QALFlacAudioDecoder::pos()
{
    FLAC__uint64 position;
    if (FLAC__stream_decoder_get_decode_position(d->flacStreamDecoder, &position) == false) {
        qWarning() << Q_FUNC_INFO << "Failed to tell the current position: the"
            "stream is not native FLAC, or there was an error from the 'tell'"
            "callback or it returned"
            "FLAC__STREAM_DECODER_TELL_STATUS_UNSUPPORTED.";
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

qint64
QALFlacAudioDecoder::decodeData(char *decodedData, qint64 maxlen)
{
    if (FLAC__stream_decoder_get_state(d->flacStreamDecoder) == FLAC__STREAM_DECODER_SEEK_ERROR
        && FLAC__stream_decoder_reset(d->flacStreamDecoder) == false) {
        {
            qWarning() << Q_FUNC_INFO << "Failed to allocate memory while resetting before decoding";;
            return -1;
        }
    }

    int tmplen = 0;

    if (d->initialData.size() > 0)
    {   
        tmplen = qMin(qint64(d->initialData.size()), maxlen);
        decodedData = reinterpret_cast<char*>(d->initialData.mid(0, tmplen).data());
        d->initialData.remove(0, tmplen);
    }   

    while (tmplen < maxlen)
    {   
        if (FLAC__stream_decoder_process_single(d->flacStreamDecoder) == false
            || FLAC__stream_decoder_get_state(d->flacStreamDecoder) == FLAC__STREAM_DECODER_END_OF_STREAM) {
            break;
        }
    }   

    return tmplen;
}
