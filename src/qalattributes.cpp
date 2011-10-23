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

Q_GLOBAL_STATIC(QALAttributes, qal_default_attributes)

class QALAttributes::Private
{
    public:
        Private()
            : frequency(48000)
            , monoSources(255)
            , stereoSources(1)
            , sync(false)
            , refresh(45)
        {   
        }   

        ~Private()
        {   
        }   

        int frequency;
        int monoSources;
        int stereoSources;
        bool sync;
        int refresh;
        QString deviceSpecifier;
};

QALAttributes::QALAttributes()
    : d(new Private)
{
}

QALAttributes::~QALAttributes()
{
}

QALAttributes
QALAttributes::defaultAttributes()
{
    return *qal_default_attributes();
}

void
QALAttributes::setDefaultAttributes(const QALAttributes &attributes)
{
    *qal_default_attributes() = attributes; 
}

int
QALAttributes::frequency() const
{
    return d->frequency;
}

int
QALAttributes::monoSources() const
{
    return d->monoSources;
}

int
QALAttributes::stereoSources() const
{
    return d->stereoSources;
}

bool
QALAttributes::sync() const
{
    return d->sync;
}

int
QALAttributes::refresh() const
{
    return d->refresh;
}

QString
QALAttributes::deviceSpecifier() const
{
    return d->deviceSpecifier;
}

void
QALAttributes::setFrequency(int frequency)
{
    d->frequency = frequency;
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

