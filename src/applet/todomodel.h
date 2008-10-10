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
#ifndef TODOMODEL_H
#define TODOMODEL_H

// qt headers
#include <QStandardItemModel>

class QStandardItem;
class TodoModelCategoryDelegate;

/**
* Model of the view
* Categorizes the todos using the startDate property
*/
class TodoModel : public QStandardItemModel
{
    Q_OBJECT
public:
    enum TodoRole {
        PercentRole = 0x10,
        UIDRole = 0x16
    };
    enum TodoCategoryType {
        ByStartDate,
        ByDueDate
    };
    TodoModel(QObject *parent = 0);
    ~TodoModel();

public:
    void setCategoryType(TodoCategoryType type);

public slots:
    void addTodoItem(const QMap <QString, QVariant> &values);
    void setCategory(const QModelIndex &index, const QColor &color);

private:
    void categorizeItem(QStandardItem *item, const QMap <QString, QVariant> &values);
    void categorizeItems();

private:
    TodoCategoryType m_categoryType;
    TodoModelCategoryDelegate *m_categoryDelegate;
};

#endif
