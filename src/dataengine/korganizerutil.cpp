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
#include "korganizerutil.h"
#include "korganizerutil_p.h"

// qt headers
#include <QColor>
#include <QList>
#include <QString>

// kde headers
#include <KConfig>
#include <KConfigGroup>
#include <KDebug>
#include <KStandardDirs>

// kdepim headers
#include <kcal/calendarresources.h>
#include <kcal/incidenceformatter.h>
#include <kcal/resourcelocal.h>


KOrganizerUtil::Private::Private(QObject *parent) : QObject(parent),
    m_resource(0)
{
    m_config = new KConfig("korganizerrc");
}

KOrganizerUtil::Private::~Private()
{
    m_resource->close();

    delete m_config;
    delete m_resource;
}

QStringList KOrganizerUtil::Private::categories()
{
    KConfigGroup general(m_config, "General");
    return general.readEntry("Custom Categories", QStringList());
}

QMap <QString, QVariant> KOrganizerUtil::Private::colors()
{
    KConfigGroup general(m_config, "Category Colors2");

    QMap <QString, QVariant> colors;
    foreach(const QString &category, categories()) {
        colors.insert(category, QVariant(general.readEntry(category, QColor())));
    }

    return colors;
}

QList <QVariant> KOrganizerUtil::Private::todos()
{
    if (!m_resource) {
        kDebug() << "Opening todo file " << calendarName();
        m_resource = new KCal::ResourceLocal(calendarName());
        connect(m_resource, SIGNAL(resourceChanged(ResourceCalendar *)),
                            SIGNAL(calendarChanged()));
    }

    QList <QVariant> todos;

    if (m_resource->load()) {
        foreach(KCal::Todo *todo, m_resource->rawTodos()) {
            QMap <QString, QVariant> values;
            values ["summary"] = todo->summary();
            values ["categories"] = todo->categoriesStr();
            values ["status"] = todo->statusStr();
            values ["percentComplete"] = todo->percentComplete();
            values ["startDate"] = todo->dtStart().date();
            values ["dueDate"] = todo->dtDue().date();
            values ["uid"] = todo->uid();
            values ["tooltip"] = KCal::IncidenceFormatter::toolTipString(todo, true);

            todos << values;
        }
    }
    else {
        kDebug() << "Fail to load todo list";
    }

    return todos;
}

QString KOrganizerUtil::Private::calendarName()
{
    KConfigGroup general(m_config, "General");

    if (general.readPathEntry("Active Calendar", QString()).isEmpty()) {
        // get the calendar name from the kresources configuration
        KConfig config("kresources/calendar/stdrc");
        KConfigGroup resources(&config, "General");
        KConfigGroup calendar(&config, "Resource_" + resources.readEntry("Standard", ""));

        // return KStandardDirs::locateLocal("data", "korganizer/std.ics");
        return calendar.readEntry("CalendarURL", QString()).replace("file://", "");
    }
    else {
        return general.readPathEntry("Active Calendar", QString()).replace("file://", "");
    }
}

KOrganizerUtil::KOrganizerUtil(QObject *parent) : QObject(parent)
{
    d = new KOrganizerUtil::Private(this);

    connect(d, SIGNAL(calendarChanged()),
               SIGNAL(calendarChanged()));
}

KOrganizerUtil::~KOrganizerUtil()
{
    delete d;
}

QStringList KOrganizerUtil::categories() const
{
    return d->categories();
}

QMap <QString, QVariant> KOrganizerUtil::colors() const
{
    return d->colors();
}

QList <QVariant> KOrganizerUtil::todos() const
{
    return d->todos();
}
