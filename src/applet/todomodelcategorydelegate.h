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

    virtual void categorizeItem(QStandardItem *);

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

    void categorizeItem(QStandardItem *);
};

/**
* Categorize the todo items by his due date
*/
class DueDateCategoryDelegate : public TodoModelCategoryDelegate
{
public:
    DueDateCategoryDelegate(QObject *parent = 0);
    ~DueDateCategoryDelegate();

    void categorizeItem(QStandardItem *);
};

#endif
