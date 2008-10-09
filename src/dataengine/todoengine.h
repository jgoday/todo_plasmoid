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
#ifndef TODOENGINE_H
#define TODOENGINE_H

// plasma headers
#include <plasma/dataengine.h>

class KOrganizerUtil;

class TodoEngine : public Plasma::DataEngine
{
    Q_OBJECT
public:
    TodoEngine(QObject* parent, const QVariantList& args);
    ~TodoEngine();

    QStringList sources() const;

protected:
    bool sourceRequestEvent(const QString &name);
    bool updateSourceEvent(const QString& source);

private slots:
    void slotCalendarChanged();

private:
    KOrganizerUtil *m_util;
};

K_EXPORT_PLASMA_DATAENGINE(todo, TodoEngine)

#endif
