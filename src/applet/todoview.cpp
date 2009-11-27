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
#include "korganizerutil.h"

// Qt headers
#include <QAction>
#include <QContextMenuEvent>
#include <QDebug>
#include <QStandardItemModel>

// kde headers
#include <kcategorydrawer.h>
#include <KIcon>
#include <KLocale>
#include <KMenu>
#include <KMessageBox>

static const QString EDIT_ACTION_NAME          = "edit_action";
static const QString DELETE_ACTION_NAME        = "delete_action";
static const QString MARK_COMPLETE_ACTION_NAME = "mark_complete_action";
static const QString OPEN_ACTION_NAME          = "open_action";

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

        KMenu *menu = new KMenu(title, this);
/*
        if (model->data(index, TodoModel::PercentRole).toInt() < 100) {
            QAction *action = new QAction(i18n("Set task as completed ?"), this);
            action->setCheckable(true);
            action->setData(model->data(index, TodoModel::UIDRole).toString());
            action->setObjectName(MARK_COMPLETE_ACTION_NAME);
            menu->addAction(action);
            menu->addSeparator();
        }
*/
        QAction editAction(i18n("Edit task"), this);
        editAction.setIcon(KIcon("document-edit"));
        editAction.setData(model->data(index, TodoModel::UIDRole).toString());
        editAction.setObjectName(EDIT_ACTION_NAME);

        QAction viewAction(i18n("View task"), this);
        viewAction.setIcon(KIcon("view-task"));
        viewAction.setData(model->data(index, TodoModel::UIDRole).toString());
        viewAction.setObjectName(OPEN_ACTION_NAME);

        QAction deleteAction(i18n("Delete task"), this);
        deleteAction.setIcon(KIcon("edit-delete"));
        deleteAction.setData(model->data(index, TodoModel::UIDRole).toString());
        deleteAction.setObjectName(DELETE_ACTION_NAME);

        menu->addAction(&editAction);
        menu->addAction(&viewAction);
        menu->addAction(&deleteAction);

        connect(menu, SIGNAL(triggered(QAction *)), SLOT(slotActionClicked(QAction *)));

        menu->exec(event->globalPos());
    }
}

void TodoView::slotActionClicked(QAction *action)
{
    if (action->objectName() == OPEN_ACTION_NAME) {
        KOrganizerUtil::showTodo(action->data().toString());
    }
    else if (action->objectName() == EDIT_ACTION_NAME) {
        KOrganizerUtil::editTodo(action->data().toString());
    }
    else if (action->objectName() == DELETE_ACTION_NAME) {
        if (KMessageBox::Yes == KMessageBox::questionYesNo(this, i18n("Delete current task?"))) {
            KOrganizerUtil::deleteTodo(action->data().toString());
        }
    }
    else if (action->objectName() == MARK_COMPLETE_ACTION_NAME) {
        qDebug() << "TODO : MARK COMPLETE " << action->data().toString();
    }
}

