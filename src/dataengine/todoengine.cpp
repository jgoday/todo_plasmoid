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
#include "todoengine.h"
#include "korganizerutil.h"

// kde headers
#include <KDebug>

static const char *CATEGORY_SOURCE = "Categories";
static const char *COLOR_SOURCE    = "Colors";
static const char *TODO_SOURCE     = "Todos";

TodoEngine::TodoEngine(QObject* parent, const QVariantList& args)
    : Plasma::DataEngine(parent)
{
    Q_UNUSED(args);
    m_util = new KOrganizerUtil(this);

    connect(m_util, SIGNAL(calendarChanged()),
                    SLOT(slotCalendarChanged()));
}

TodoEngine::~TodoEngine()
{
    delete m_util;
}

QStringList TodoEngine::sources() const
{
    QStringList sources;
    sources << CATEGORY_SOURCE;
    sources << COLOR_SOURCE;
    sources << TODO_SOURCE;

    return sources;
}

bool TodoEngine::sourceRequestEvent(const QString &name)
{
    return updateSourceEvent(name);
}

bool TodoEngine::updateSourceEvent(const QString &name)
{
    if(QString::compare(name, CATEGORY_SOURCE) == 0) {
        setData(I18N_NOOP(name), I18N_NOOP("categories"), m_util->categories());
    }
    else if(QString::compare(name, COLOR_SOURCE) == 0) {
        setData(I18N_NOOP(name), I18N_NOOP("colors"), m_util->colors());
    }
    else if(QString::compare(name, TODO_SOURCE) == 0) {
        if (m_util->isAkonadiRunning()) {
            setData(I18N_NOOP(name), I18N_NOOP("todos"), m_util->todos());
        }
        else {
            setData(I18N_NOOP(name), I18N_NOOP("error"),
                    I18N_NOOP("Akonadi is not running"));
        }
    }
    return true;
}

void TodoEngine::slotCalendarChanged()
{
    updateSourceEvent(COLOR_SOURCE);
    updateSourceEvent(TODO_SOURCE);
}

#include "todoengine.moc"
