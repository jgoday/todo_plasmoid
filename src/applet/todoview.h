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
#ifndef TODOVIEW_H
#define TODOVIEW_H

// kde headers
#include <kcategorizedview.h>

class QAction;
class QContextMenuEvent;

/**
* Categorized view that represents the todo list in the plasma applet
* The todos are categorized by startDate by the model
*/
class TodoView : public KCategorizedView
{
    Q_OBJECT
public:
    TodoView(QWidget *parent = 0);
    ~TodoView();

    void contextMenuEvent(QContextMenuEvent *event);

private slots:
    void slotActionClicked(QAction *);
};

#endif
