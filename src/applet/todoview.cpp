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
#include "todoview.h"
#include "todomodel.h"
#include "todoitemdelegate.h"

// Qt headers
#include <QAction>
#include <QContextMenuEvent>
#include <QDebug>
#include <QMenu>
#include <QStandardItemModel>

// kde headers
#include <kcategorydrawer.h>
#include <KIcon>

TodoView::TodoView(QWidget *parent) : KCategorizedView(parent)
{
    setItemDelegate(new TodoItemDelegate(this));
    setCategoryDrawer(new KCategoryDrawer());

    setAlternatingBlockColors(true);
}

TodoView::~TodoView()
{
}

void TodoView::contextMenuEvent(QContextMenuEvent *event)
{
    QModelIndex index = currentIndex();
    const QStandardItemModel *model = static_cast <const QStandardItemModel *> (index.model());

    if (index.isValid() && model) {
        QString title = model->data(index, Qt::DisplayRole).toString();

        qDebug() << "Creating menu with title: " << title;

        QMenu *menu = new QMenu(title, this);

        if (model->data(index, TodoModel::PercentRole).toInt() < 100) {
            qDebug() << "Percent " << model->data(index, TodoModel::PercentRole).toInt();

            QAction *action = new QAction("Set task as completed ?", this);
            action->setCheckable(true);
            menu->addAction(action);
            menu->addSeparator();
        }

        QAction viewAction("View task", this);
        viewAction.setIcon(KIcon("document-edit"));
        menu->addAction(&viewAction);

        menu->exec(event->globalPos());
    }
}

