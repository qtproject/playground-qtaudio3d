/****************************************************************************
**
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: http://www.qt-project.org/
**
** This file is part of the QtAudio3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qallistener.h"

QALListener::QALListener()
{
}

QALListener::~QALListener()
{
}

float QALListener::gain() const
{
    float gain;
    alGetListenerf(AL_GAIN, &gain);

    return gain;
}

void QALListener::setPosition(int px, int py, int pz)
{
    alListener3i(AL_POSITION, px, py, pz);
}

void QALListener::setPosition(float px, float py, float pz)
{
    alListener3f(AL_POSITION, px, py, pz);
}

void QALListener::setVelocity(int vx, int vy, int vz)
{
    alListener3i(AL_VELOCITY, vx, vy, vz);
}

void QALListener::setVelocity(float vx, float vy, float vz)
{
    alListener3f(AL_VELOCITY, vx, vy, vz);
}

void QALListener::setOrientation(int x1, int y1, int z1, int x2, int y2, int z2)
{
    ALint orientationVector[] = {x1, y1, z1, x2, y2, z2};
    alListeneriv(AL_ORIENTATION, orientationVector);
}

void QALListener::setOrientation(float x1, float y1, float z1, float x2, float y2, float z2)
{
    ALfloat orientationVector[] = {x1, y1, z1, x2, y2, z2};
    alListenerfv(AL_ORIENTATION, orientationVector);
}

void QALListener::setGain(float gain)
{
    alListenerf(AL_GAIN, gain);
}
