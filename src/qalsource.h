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

#ifndef QALSOURCE_H
#define QALSOURCE_H

#include "q_openal_export.h"

class Q_OPENAL_EXPORT QALSource
{
public:

    enum Type {
        Static,
        Streaming,
        Undetermined
    };

    QALSource(unsigned int source);
    virtual ~QALSource();

    float minimumGain() const;
    float maximumGain() const;

    float gain() const;

    bool isRelative() const;

    Type type() const;

    bool isLooping() const;

    int currentBuffer() const;

    int queuedBuffers() const;
    int processedBuffers() const;

    float referenceDistance() const;

    float maximumDistance() const;

    float rollOffFactor() const;

    float pitch() const;

    float coneInnerAngle() const;
    float coneOuterAngle() const;

    float coneOuterGain() const;

    float secondOffset() const;
    float sampleOffset() const;
    float byteOffset() const;

    void setPosition(int px, int py, int pz);
    void setPosition(float px, float py, float pz);

    void setVelocity(int vx, int vy, int vz);
    void setVelocity(float vx, float vy, float vz);

    void setMinimumGain(float minimumGain);
    void setMaximumGain(float maximumGain);

    void setGain(float gain);

    void setRelative(bool relative);

    void setLooping(bool looping);

    void setCurrentBuffer(int currentBuffer);

    void setReferenceDistance(int referenceDistance);
    void setReferenceDistance(float referenceDistance);

    void setMaximumDistance(int maximumDistance);
    void setMaximumDistance(float maximumDistance);

    void setRollOffFactor(int rollOffFactor);
    void setRollOffFactor(float rollOffFactor);

    void setPitch(float pitch);

    void setDirection(int dx, int dy, int dz);
    void setDirection(float dx, float dy, float dz);

    void setConeInnerAngle(int coneInnerAngle);
    void setConeInnerAngle(float coneInnerAngle);

    void setConeOuterAngle(int coneOuterAngle);
    void setConeOuterAngle(float coneOuterAngle);

    void setConeOuterGain(float coneOuterGain);

    void setSecondOffset(int secondOffset);
    void setSecondOffset(float secondOffset);

    void setSampleOffset(int sampleOffset);
    void setSampleOffset(float sampleOffset);

    void setByteOffset(int byteOffset);
    void setByteOffset(float byteOffset);

    private:
        class Private;
        Private *const d;
};

#endif
