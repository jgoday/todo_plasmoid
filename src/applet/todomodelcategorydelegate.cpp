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
#include "todomodelcategorydelegate.h"
#include "todomodel.h"

// qt headers
#include <QDate>
#include <QStandardItem>

// kde headers
#include <kcategorizedsortfilterproxymodel.h>
#include <KLocale>

TodoModelCategoryDelegate::TodoModelCategoryDelegate(QObject *parent) :
    QObject(parent)
{
}

TodoModelCategoryDelegate::~TodoModelCategoryDelegate()
{
}

void TodoModelCategoryDelegate::categorizeItem(QStandardItem *item)
{
    Q_UNUSED(item)
}

void TodoModelCategoryDelegate::setDisplayRole(QStandardItem *item, const QVariant &value)
{
    item->setData(value, KCategorizedSortFilterProxyModel::CategoryDisplayRole);
}

void TodoModelCategoryDelegate::setSortRole(QStandardItem *item, const QVariant &value)
{
    item->setData(value, KCategorizedSortFilterProxyModel::CategorySortRole);
}

// start date category delegate
StartDateCategoryDelegate::StartDateCategoryDelegate(QObject *parent) :
    TodoModelCategoryDelegate(parent)
{
}

StartDateCategoryDelegate::~StartDateCategoryDelegate()
{
}

void StartDateCategoryDelegate::categorizeItem(QStandardItem *item)
{
    int categorySort = 0;

    QDate date = item->data(TodoModel::StartDateRole).toDate();
    QVariant value;

    if (item->data(TodoModel::PercentRole).toInt() == 100) {
        categorySort = 0;
        value = QVariant(i18n("Completed"));
    }
    else if (date == QDate::currentDate()) {
        categorySort = 1;
        value = QVariant(i18n("Started today"));
    }
    else if (date.daysTo(QDate::currentDate()) <= 1) {
        categorySort = 2;
        value = QVariant(i18n("Started yesterday"));
    }
    else if (date.daysTo(QDate::currentDate()) <= 7) {
        categorySort = 3;
        value = QVariant(i18n("Started last week"));
    }
    else if (date.daysTo(QDate::currentDate()) <= 30) {
        categorySort = 4;
        value = QVariant(i18n("Started last month"));
    }
    else if (date.isValid()) {
        categorySort = 5;
        value = QVariant(i18n("Started a long time ago"));
    }
    else {
        categorySort = 6;
        value = QVariant(i18n("Not started"));
    }

    setDisplayRole(item, value);
    setSortRole(item, categorySort);
}

// due date category delegate
DueDateCategoryDelegate::DueDateCategoryDelegate(QObject *parent) :
    TodoModelCategoryDelegate(parent)
{
}

DueDateCategoryDelegate::~DueDateCategoryDelegate()
{
}

void DueDateCategoryDelegate::categorizeItem(QStandardItem *item)
{
    int categorySort = 0;

    QDate date = item->data(TodoModel::DueDateRole).toDate();
    QVariant value;

    if (item->data(TodoModel::PercentRole).toInt() == 100) {
        categorySort = 0;
        value = QVariant(i18n("Completed"));
    }
    else if (!date.isValid()) {
        categorySort = 9;
        value = QVariant(i18n("Without planification"));
    }
    else if (date == QDate::currentDate()) {
        categorySort = 1;
        value = QVariant(i18n("For today"));
    }
    else if (QDate::currentDate().daysTo(date) < 1) {
        categorySort = 2;
        value = QVariant(i18n("This ones had to be done"));
    }
    else if (QDate::currentDate().daysTo(date) == 1) {
        categorySort = 3;
        value = QVariant(i18n("For tomorrow"));
    }
    else if (QDate::currentDate().daysTo(date) > 1 &&
             QDate::currentDate().daysTo(date) < 7) {
        if (date.dayOfWeek() >= QDate::currentDate().dayOfWeek()) {
            categorySort = 4;
            value = QVariant(i18n("For this week"));
        }
        else {
            categorySort = 5;
            value = QVariant(i18n("For the next week"));
        }
    }
    else if (QDate::currentDate().daysTo(date) > 7 &&
             QDate::currentDate().daysTo(date) < 30) {
        if (date.daysInMonth() >= QDate::currentDate().daysInMonth()) {
            categorySort = 6;
            value = QVariant(i18n("For this month"));
        }
        else {
            categorySort = 7;
            value = QVariant(i18n("For the next month"));
        }
    }
    else if (QDate::currentDate().daysTo(date) > 30) {
        categorySort = 8;
        value = QVariant(i18n("I still have enough time"));
    }

    setDisplayRole(item, value);
    setSortRole(item, categorySort);
}
