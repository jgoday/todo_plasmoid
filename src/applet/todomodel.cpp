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
#include "todomodel.h"
#include "todomodelcategorydelegate.h"

// qt headers
#include <QDate>
#include <QStandardItem>

// kde headers
#include <KIcon>

TodoModel::TodoModel(QObject *parent, int type) : QStandardItemModel(parent),
    m_categoryType(type),
    m_categoryDelegate(0)
{
}

TodoModel::~TodoModel()
{
}

void TodoModel::setCategoryType(int type)
{
    m_categoryType = type;

    delete m_categoryDelegate;

    switch (type) {
        case TodoModel::ByDueDate:
            m_categoryDelegate = new DueDateCategoryDelegate(this);
            break;
        default:
            m_categoryDelegate = new StartDateCategoryDelegate(this);
            break;
    }

    categorizeItems();
}

int TodoModel::categoryType() const
{
    return m_categoryType;
}

void TodoModel::addTodoItem(const QMap <QString, QVariant> &values)
{
    QStandardItem *item = new QStandardItem(values ["summary"].toString());
    item->setData(values ["categories"].toString(), Qt::UserRole);
    item->setData(values ["percentComplete"].toInt(), TodoModel::PercentRole);
    item->setData(values ["uid"].toString(), TodoModel::UIDRole);
    item->setData(values ["startDate"].toDate(), TodoModel::StartDateRole);
    item->setData(values ["dueDate"].toDate(), TodoModel::DueDateRole);

    item->setToolTip(values ["tooltip"].toString());

    // if today is the due date we show an alarm icon on the todo
    if (values ["dueDate"].toDate() == QDate::currentDate() ||
        (values ["dueDate"].toDate () < QDate::currentDate() && values ["percentComplete"].toInt() < 100)) {
        item->setIcon(KIcon("appointment-reminder"));
    }

    categorizeItem(item);
    insertRow(0, item);
}

void TodoModel::setCategory(const QModelIndex &index, const QColor &color)
{
    QStandardItem *item = itemFromIndex(index);
    item->setData(color, Qt::BackgroundRole);
}

void TodoModel::categorizeItem(QStandardItem *item)
{
    m_categoryDelegate->categorizeItem(item);
}

void TodoModel::categorizeItems()
{
    for (int i = 0; i < rowCount(); i++) {
        m_categoryDelegate->categorizeItem(item(i));
    }
}
