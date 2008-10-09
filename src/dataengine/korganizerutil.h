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
#ifndef KORGANIZERUTIL_H
#define KORGANIZERUTIL_H

// qt headers
#include <QMap>
#include <QObject>

class KOrganizerUtil : public QObject
{
    Q_OBJECT
public:
    KOrganizerUtil(QObject *parent=0);
    ~KOrganizerUtil();

    QStringList categories() const;
    QMap <QString, QVariant> colors() const;
    QList <QVariant> todos() const;

signals:
    void calendarChanged();

private:
    class Private;
    Private *d;
};

#endif
