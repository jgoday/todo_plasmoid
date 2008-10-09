/*
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License version 2 as
 *   published by the Free Software Foundation
 *
 *   Copyright (C) 2008 by Javier Goday <jgoday@gmail.com>
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#ifndef KORGANIZERUTIL_P_H
#define KORGANIZERUTIL_P_H

#include "korganizerutil.h"

// Qt headers
#include <QObject>

namespace KCal {
    class ResourceLocal;
}
class KConfig;

class KOrganizerUtil::Private : public QObject
{
    Q_OBJECT
public:
    Private(QObject *parent);
    ~Private();

    QStringList categories();

    QMap <QString, QVariant> colors();

    QList <QVariant> todos();

signals:
    void calendarChanged();

private:
    /**
    * Returns the korganizer calendar file name
    */
    QString calendarName();

private:
    KConfig *m_config;
    KCal::ResourceLocal *m_resource;
};

#endif
