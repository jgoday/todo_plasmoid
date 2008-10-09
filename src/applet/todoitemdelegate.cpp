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
#include "todoitemdelegate.h"
#include "todomodel.h"

// Qt headers
#include <QAbstractItemModel>
#include <QApplication>

TodoItemDelegate::TodoItemDelegate(QObject *parent) : QItemDelegate(parent)
{
}

TodoItemDelegate::~TodoItemDelegate()
{
}

QWidget *TodoItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                                         const QModelIndex &index) const
{
    Q_UNUSED(parent)
    Q_UNUSED(option)
    Q_UNUSED(index)

    return 0;
}

void TodoItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                                const QModelIndex &index) const
{
    const QAbstractItemModel *model = index.model();

    QStyleOptionViewItem textOption = option;

    // only draw the progress bar if the todo is already started
    if (model && model->data(index, TodoModel::PercentRole).toInt() > 0) {
        QStyleOptionProgressBar progressBarOption;
        progressBarOption.rect = QRect(option.rect.x() + option.rect.width() - 60,
                                       option.rect.y() + 1,
                                       59,
                                       option.rect.height() - 2);
        progressBarOption.fontMetrics = QApplication::fontMetrics();
        progressBarOption.minimum = 0;
        progressBarOption.maximum = 100;
        progressBarOption.textAlignment = Qt::AlignCenter;
        progressBarOption.textVisible = true;

        progressBarOption.progress = model->data(index, TodoModel::PercentRole).toInt();
        progressBarOption.text = QString().sprintf("%d%%", progressBarOption.progress);

        QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);

        textOption.rect.setWidth(option.rect.width() - 60);
    }

    QItemDelegate::paint(painter, textOption, index);
}

void TodoItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    Q_UNUSED(editor)
    Q_UNUSED(index)
}

void TodoItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                                     const QModelIndex &index) const
{
    Q_UNUSED(editor)
    Q_UNUSED(model)
    Q_UNUSED(index)
}

QSize TodoItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)

    return QSize(200, 25);
}
