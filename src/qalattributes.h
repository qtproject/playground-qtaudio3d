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

#include "q_openal_export.h"

#include <QtCore/QString>
#include <QtCore/QSharedPointer>

class Q_OPENAL_EXPORT QALAttributes
{
    //Q_DECLARE_PRIVATE(QALAttribues)
public:
    QALAttributes();
    QALAttributes(const QALAttributes &other);
    virtual ~QALAttributes();

    QALAttributes& operator=(const QALAttributes &other);
    bool operator==(const QALAttributes &other) const;
    bool operator!=(const QALAttributes &other) const;

    /*!
      Returns the default QALAttributes for the application. All QALContext
      objects that are created use these attributes unless another format is
      specified, e.g. when they are constructed.

      If no special default attributes has been set using
      setDefaultAttributes(), the default format is the same as that created
      with QALAttributes().

      \sa setDefaultAttributes()
      */
    static QALAttributes defaultAttributes();

    /*!
      Sets a new default QALAttributes for the application to \a attributes.

      \sa defaultAttributes()
      */
    static void setDefaultAttributes(const QALAttributes& attributes); 

    int frequency() const;
    int monoSources() const;
    int stereoSources() const;
    bool sync() const;
    int refresh() const;
    QString deviceSpecifier() const;

    void setFrequency(int frequency);
    void setMonoSources(int monoSources);
    void setStereoSources(int stereoSources);
    void setSync(bool sync);
    void setRefresh(int refresh);
    void setDeviceSpecifier(const QString& deviceSpecifier);

    private:
        class Private;
        QSharedDataPointer<Private> d;
};
