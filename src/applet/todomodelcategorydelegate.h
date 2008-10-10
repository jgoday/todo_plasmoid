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
#ifndef TODOMODELCATEGORYDELEGATE_H
#define TODOMODELCATEGORYDELEGATE_H

// qt headers
#include <QMap>
#include <QObject>

class QStandardItem;

/**
* Categorized delegate to allow categorize the model
* in different ways
*/
class TodoModelCategoryDelegate : public QObject
{
public:
    TodoModelCategoryDelegate(QObject *parent = 0);
    ~TodoModelCategoryDelegate();

    virtual void categorizeItem(QStandardItem *, const QMap <QString, QVariant> &values);

protected:
    void setDisplayRole(QStandardItem *, const QVariant &value);
    void setSortRole(QStandardItem *, const QVariant &value);
};

/**
* Categorize the todo items by his start date
*/
class StartDateCategoryDelegate : public TodoModelCategoryDelegate
{
public:
    StartDateCategoryDelegate(QObject *parent = 0);
    ~StartDateCategoryDelegate();

    void categorizeItem(QStandardItem *, const QMap <QString, QVariant> &values);
};

/**
* Categorize the todo items by his due date
*/
class DueDateCategoryDelegate : public TodoModelCategoryDelegate
{
public:
    DueDateCategoryDelegate(QObject *parent = 0);
    ~DueDateCategoryDelegate();

    void categorizeItem(QStandardItem *, const QMap <QString, QVariant> &values);
};

#endif
