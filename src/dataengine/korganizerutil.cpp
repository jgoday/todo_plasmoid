/*
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
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
#include "korganizerutil.h"

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
#include <kcal/incidenceformatter.h>
#include <kcal/todo.h>

// Akonadi
#include <akonadi/monitor.h>
#include <akonadi/collection.h>
#include <akonadi/collectionfetchjob.h>
#include <akonadi/item.h>
#include <akonadi/itemfetchjob.h>
#include <akonadi/itemfetchscope.h>


class KOrganizerUtil::Private
{
public:
    Private(QObject *parent)
    {
        m_config = new KConfig("korganizerrc");
        m_akonadiMonitor = new Akonadi::Monitor();

        m_akonadiMonitor->setAllMonitored(true);
    }

    ~Private()
    {
        delete m_config;
        delete m_akonadiMonitor;
    }

    Akonadi::Monitor *akonadiMonitor() const
    {
        return m_akonadiMonitor;
    }

    QStringList categories()
    {
        KConfigGroup general(m_config, "General");
        return general.readEntry("Custom Categories", QStringList());
    }

    QMap <QString, QVariant> colors()
    {
        KConfigGroup general(m_config, "Category Colors2");

        QMap <QString, QVariant> colors;
        foreach(const QString &category, categories()) {
            colors.insert(category, QVariant(general.readEntry(category, QColor())));
        }

        return colors;
    }

    QList <QVariant> todos()
    {
        QList <QVariant> todos;
        Akonadi::CollectionFetchJob *job = new Akonadi::CollectionFetchJob(Akonadi::Collection::root(),
                                                                        Akonadi::CollectionFetchJob::Recursive);
        if (job->exec()) {
            Akonadi::Collection::List collections = job->collections();
            foreach(const Akonadi::Collection &collection, collections) {
                Akonadi::ItemFetchJob *ijob = new Akonadi::ItemFetchJob(collection);
                ijob->fetchScope().fetchFullPayload();

                if (ijob->exec()) {
                    Akonadi::Item::List items = ijob->items();
                    foreach(const Akonadi::Item &item, items) {
                        if (item.hasPayload <KCal::Todo::Ptr>()) {
                            KCal::Todo *todo = item.payload <KCal::Todo::Ptr>().get();

                            if (todo) {
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
                    }
                }
            }
        }

        return todos;
    }

private:
    KConfig *m_config;
    Akonadi::Monitor *m_akonadiMonitor;
};


KOrganizerUtil::KOrganizerUtil(QObject *parent) : QObject(parent)
{
    d = new KOrganizerUtil::Private(this);

    connect(d->akonadiMonitor(), SIGNAL(itemChanged (const Akonadi::Item &, const QSet< QByteArray > &)),
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
