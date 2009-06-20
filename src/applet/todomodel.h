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
        UIDRole = 0x16,
        StartDateRole = 0x17,
        DueDateRole = 0x18
    };
    enum TodoCategoryType {
        ByStartDate = 0,
        ByDueDate = 1
    };

    TodoModel(QObject *parent = 0, int type = TodoModel::ByStartDate);
    ~TodoModel();

public:
    void setCategoryType(int type);
    int categoryType() const;

public slots:
    void addTodoItem(const QMap <QString, QVariant> &values);
    void setCategory(const QModelIndex &index, const QColor &color);

private:
    void categorizeItem(QStandardItem *item);
    void categorizeItems();

private:
    int m_categoryType;
    TodoModelCategoryDelegate *m_categoryDelegate;
};

#endif
