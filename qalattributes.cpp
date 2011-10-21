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

#include "qalattributes.h"

Q_GLOBAL_STATIC(QGLFormat, qal_default_format)

class QALAttributes::Private
{
    public:
        Private()
            : frequence(48000)
            , monoSource(255)
            , stereoSource(1)
            , sync(false)
            , refresh(45)
        {   
        }   

        ~Private()
        {   
        }   

        int frequence;
        int monoSource;
        int stereoSource;
        bool sync;
        int refresh;
        QString deviceSpecifier;
};

QALAttributes::QALAttributes()
{
}

QALAttributes::~QALAttributes()
{
}

QALAttributes
QALAttributes::defaultFormat()
{
    return *qal_default_attributes();
}

void
QGLFormat::setDefaultFormat(const QALAttributes &attributes)
{
    *qal_default_attributes() = attributes; 
}

int
QALAttributes::frequency() const
{
    d->frequence;
}

int
QALAttributes::monoSources() const
{
    d->monoSources;
}

int
QALAttributes::stereoSources() const
{
    d->stereoSources;
}

bool
QALAttributes::sync() const
{
    d->sync;
}

int
QALAttributes::refresh() const
{
    d->refresh;
}

QString
QALAttributes::deviceSpecifier() const
{
    d->deviceSpecifier;
}

void
QALAttributes::setFrequency(int frequency)
{
    d->frequence = frequence;
}

void
QALAttributes::setMonoSources(int monoSources)
{
    d->monoSources = monoSources;
}

void
QALAttributes::setStereoSources(int stereoSources)
{
    d->stereoSources = stereoSources;
}

void
QALAttributes::setDeviceSpecifier(const QString &deviceSpecifier)
{
    d->deviceSpecifier = deviceSpecifier;
}

void
QALAttributes::setSync(bool sync)
{
    d->sync = sync;
}

void
QALAttributes::setRefresh(int refresh)
{
    d->refresh = refresh;
}

void
QALAttributes::setDeviceSpecifier(const QString &deviceSpecifier)
{
    d->deviceSpecifier = deviceSpecifier;
}

