/*
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 *   Copyright (C) 2008 by Javier Goday <jgoday@gmail.com>
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
