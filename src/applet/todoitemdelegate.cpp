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
#include "todoitemdelegate.h"
#include "todomodel.h"

// Qt headers
#include <QAbstractItemModel>
#include <QApplication>

TodoItemDelegate::TodoItemDelegate(QObject *parent) : QStyledItemDelegate(parent)
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
                                       option.rect.y() + 4,
                                       56,
                                       option.rect.height() - 8);
        progressBarOption.fontMetrics = QApplication::fontMetrics();
        progressBarOption.minimum = 0;
        progressBarOption.maximum = 100;
        progressBarOption.textAlignment = Qt::AlignCenter;
        progressBarOption.textVisible = true;

        progressBarOption.progress = model->data(index, TodoModel::PercentRole).toInt();
        progressBarOption.text = QString().sprintf("%d%%", progressBarOption.progress);

        QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
    }

    textOption.rect = option.rect.adjusted(1, 2, -64, -2);
    QStyledItemDelegate::paint(painter, textOption, index);
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

    return QSize(200, 32);
}
