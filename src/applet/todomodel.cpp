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
#include "todomodel.h"

// qt headers
#include <QDate>
#include <QStandardItem>

// kde headers
#include <kcategorizedsortfilterproxymodel.h>
#include <KIcon>
#include <KLocale>

TodoModel::TodoModel(QObject *parent) : QStandardItemModel(parent)
{
}

TodoModel::~TodoModel()
{
}

void TodoModel::addTodoItem(const QMap <QString, QVariant> &values)
{
    QStandardItem *item = new QStandardItem(values ["summary"].toString());
    item->setData(values ["categories"].toString(), Qt::UserRole);
    item->setData(values ["percentComplete"].toInt(), TodoModel::PercentRole);
    item->setData(values ["uid"].toString(), TodoModel::UIDRole);
    item->setToolTip(values ["tooltip"].toString());

    // if today is the due date we show an alarm icon on the todo
    if (values ["dueDate"].toDate() == QDate::currentDate() ||
        (values ["dueDate"].toDate () < QDate::currentDate() && values ["percentComplete"].toInt() < 100)) {
        item->setIcon(KIcon("appointment-reminder"));
    }

    categorizeItem(item, values);
    insertRow(0, item);
}

void TodoModel::setCategory(const QModelIndex &index, const QColor &color)
{
    QStandardItem *item = itemFromIndex(index);
    item->setData(color, Qt::BackgroundRole);
}

void TodoModel::categorizeItem(QStandardItem *item, const QMap <QString, QVariant> &values)
{
    int categorySort = 0;

    QDate date = values ["startDate"].toDate();
    QVariant value;

    if (date == QDate::currentDate()) {
        value = QVariant(i18n("Started today"));
    }
    else if (date.daysTo(QDate::currentDate()) <= 1) {
        categorySort = 1;
        value = QVariant(i18n("Started yesterday"));
    }
    else if (date.daysTo(QDate::currentDate()) <= 7) {
        categorySort = 2;
        value = QVariant(i18n("Started last week"));
    }
    else if (date.daysTo(QDate::currentDate()) <= 30) {
        categorySort = 3;
        value = QVariant(i18n("Started last month"));
    }
    else if (date.isValid()) {
        categorySort = 4;
        value = QVariant(i18n("Started a long time ago"));
    }
    else {
        categorySort = 5;
        value = QVariant(i18n("Not started"));
    }

    item->setData(value, KCategorizedSortFilterProxyModel::CategoryDisplayRole);
    item->setData(categorySort, KCategorizedSortFilterProxyModel::CategorySortRole);
}