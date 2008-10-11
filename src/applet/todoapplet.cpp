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
#include "todoapplet.h"
#include "todomodel.h"
#include "todoview.h"
#include "korganizerutil.h"

// qt headers
#include <QComboBox>
#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QVBoxLayout>

// kde headers
#include <KColorScheme>
#include <KConfigDialog>
#include <KIcon>
#include <KIconLoader>
#include <kcategorizedsortfilterproxymodel.h>

// plasma headers
#include <Plasma/Dialog>
#include <Plasma/Icon>
#include <Plasma/Theme>

K_EXPORT_PLASMA_APPLET(todoapplet, TodoApplet)

static const char *CATEGORY_SOURCE = "Categories";
static const char *COLOR_SOURCE    = "Colors";
static const char *TODO_SOURCE     = "Todos";

TodoApplet::TodoApplet(QObject *parent, const QVariantList &args) :
    Plasma::Applet(parent, args),
    m_engine(0),
    m_dialog(0),
    m_icon(0),
    m_layout(0),
    m_types(0),
    m_proxyWidget(0),
    m_widget(0),
    m_view(0),
    m_configUi()
{
    setHasConfigurationInterface(true);
}

TodoApplet::~TodoApplet()
{
    delete m_view;
    delete m_types;
}

void TodoApplet::init()
{
    // intialize the todo list model and the filter proxy
    m_model = new TodoModel(this);
    m_proxyModel = new KCategorizedSortFilterProxyModel();
    m_proxyModel->setCategorizedModel(true);
    m_proxyModel->sort(0);
    m_proxyModel->setSourceModel(m_model);
    m_proxyModel->setFilterRole(Qt::UserRole);

    KConfigGroup configGroup = config();
    m_model->setCategoryType(configGroup.readEntry("CategoryType", 0));

    // categories proxy
    m_types = new QComboBox();
    connect(m_types, SIGNAL(currentIndexChanged(const QString &)),
            m_proxyModel, SLOT(setFilterRegExp(const QString &)));

    // applet main widget
    m_widget = new QWidget();

    // applet main layout
    m_layout = new QGraphicsLinearLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    m_layout->setOrientation(Qt::Vertical);

    setLayout(m_layout);
    doLayout();

    m_engine = dataEngine("todo");
    if (m_engine) {
        m_engine->connectSource(TODO_SOURCE, this);
        m_engine->connectSource(CATEGORY_SOURCE, this);
        m_engine->connectSource(COLOR_SOURCE, this);
    }
}

void TodoApplet::constraintsEvent(Plasma::Constraints constraints)
{
    if (constraints & Plasma::FormFactorConstraint) {
        doLayout();
    }

    if (m_icon && constraints & Plasma::SizeConstraint) {
        m_icon->resize(geometry().size());
    }
}

void TodoApplet::dataUpdated(const QString &name, const Plasma::DataEngine::Data &data)
{
    if (QString::compare(name, CATEGORY_SOURCE) == 0) {
        updateCategories(data["categories"].toStringList());
    }
    else if (QString::compare(name, COLOR_SOURCE) == 0) {
        updateColors(data["colors"].toMap());
    }
    else if (QString::compare(name, TODO_SOURCE) == 0) {
        updateTodoList(data["todos"].toList());
    }
}

void TodoApplet::showDialog()
{
    if (m_dialog->isVisible()) {
        m_dialog->hide();
    }
    else {
        m_dialog->show();
        m_dialog->move(popupPosition(m_dialog->sizeHint()));
    }
}

void TodoApplet::doLayout()
{
    if (!m_view) {
        m_view = new TodoView();
        m_view->setModel(m_proxyModel);
        connect(m_view, SIGNAL(doubleClicked(const QModelIndex &)),
                        SLOT(slotOpenTodo(const QModelIndex &)));
        KColorScheme colorTheme = KColorScheme(QPalette::Active, KColorScheme::View,
                                               Plasma::Theme::defaultTheme()->colorScheme());

        // applet's title
        QLabel *title = new QLabel();
        title->setText(i18n("<font color=\"%1\">Todo list</font>",
                       colorTheme.foreground(KColorScheme::NormalText).color().name()));
        QLabel *icon = new QLabel();
        icon->setPixmap(KIcon("view-pim-tasks").pixmap(KIconLoader::SizeMedium, KIconLoader::SizeMedium));

        QHBoxLayout *titleLayout = new QHBoxLayout();
        titleLayout->setSpacing(0);
        titleLayout->setMargin(0);

        titleLayout->addWidget(icon);
        titleLayout->addWidget(title, 1);

        // Add new todo button
        QToolButton *addButton = new QToolButton();
        addButton->setIcon(KIcon("list-add"));
        connect(addButton, SIGNAL(clicked()), SLOT(slotAddTodo()));

        // types and add button
        QHBoxLayout *footerLayout = new QHBoxLayout();
        footerLayout->addWidget(addButton);
        footerLayout->addWidget(m_types);

        // main widget
        QVBoxLayout *layout = new QVBoxLayout();
        layout->addLayout(titleLayout);
        layout->addWidget(m_view);
        layout->addLayout(footerLayout);

        m_widget->setLayout(layout);
    }

    if (formFactor() != Plasma::Planar && formFactor() != Plasma::MediaCenter) {
        if (m_proxyWidget) {
            m_proxyWidget->setWidget(0);
            m_layout->removeItem(m_proxyWidget);
            delete m_proxyWidget;
        }

        if (!m_icon) {
            setBackgroundHints(NoBackground);
            setAspectRatioMode(Plasma::ConstrainedSquare);
            setMinimumSize(QSize(16, 16));

            QVBoxLayout *dialogLayout = new QVBoxLayout();
            dialogLayout->addWidget(m_widget);

            m_dialog = new Plasma::Dialog(0);
            m_dialog->setMinimumSize(QSize(300, 200));
            m_dialog->setFocusPolicy(Qt::NoFocus);
            m_dialog->setWindowFlags(Qt::Popup);
            m_dialog->setLayout(dialogLayout);

            // panel icon
            m_icon = new Plasma::Icon(KIcon("view-pim-tasks"), QString(), this);
            m_layout->addItem(m_icon);
            connect(m_icon, SIGNAL(clicked()), SLOT(showDialog()));

            m_icon->resize(geometry().size());
        }
    }
    else {
        if (m_icon) {
            m_layout->removeItem(m_icon);
        }
        delete m_dialog;
        delete m_icon;

        if (!m_proxyWidget) {
            setBackgroundHints(StandardBackground);

            m_proxyWidget = new QGraphicsProxyWidget(this);
            m_proxyWidget->setWidget(m_widget);

            m_layout->addItem(m_proxyWidget);

            resize(QSize(160, 190));
            setMinimumSize(QSize(160, 190));
        }
    }
}

void TodoApplet::updateCategories(const QStringList &categories)
{
    m_types->addItem(QString());

    foreach(const QString &category, categories) {
        m_types->addItem(category);
    }
}

void TodoApplet::updateColors(const QMap <QString, QVariant> &colors)
{
    foreach(const QString &category, colors.keys()) {
        m_types->setItemData(m_types->findText(category), 
                             colors[category].value<QColor>(),
                             Qt::DecorationRole);

        // search for the categories todo's to set the category color
        foreach(const QModelIndex &index, m_model->match(m_model->index(0, 0), 
                                                         Qt::UserRole, category, -1)) {
            m_model->setCategory(index, colors[category].value<QColor>());
        }
    }
}

void TodoApplet::updateTodoList(const QList <QVariant> &todos)
{
    m_model->clear();

    foreach(const QVariant &todo, todos) {
        QMap <QString, QVariant> values = todo.toMap();

        m_model->addTodoItem(values);
    }

	m_view->reset();
}

void TodoApplet::slotOpenTodo(const QModelIndex &index)
{
    KOrganizerUtil::showTodo(m_model->data(m_proxyModel->mapToSource(index),
                                           TodoModel::UIDRole).toString());
}

void TodoApplet::slotAddTodo()
{
    KOrganizerUtil::showAddTodo();
}

void TodoApplet::createConfigurationInterface(KConfigDialog *parent)
{
    QWidget *widget = new QWidget();
    m_configUi.setupUi(widget);
    parent->setButtons(KDialog::Ok | KDialog::Cancel | KDialog::Apply);

    connect(parent, SIGNAL(applyClicked()), this, SLOT(configAccepted()));
    connect(parent, SIGNAL(okClicked()), this, SLOT(configAccepted()));

    parent->addPage(widget, parent->windowTitle(), icon());

    // category types
    m_configUi.categoryTypeBox->addItem(i18n("By start date"),
                                     QVariant(TodoModel::ByStartDate));
    m_configUi.categoryTypeBox->addItem(i18n("By due date"),
                                     QVariant(TodoModel::ByDueDate));
    m_configUi.categoryTypeBox->setCurrentIndex(m_model->categoryType());
}

void TodoApplet::configAccepted()
{
    m_model->setCategoryType(m_configUi.categoryTypeBox->itemData(
                               m_configUi.categoryTypeBox->currentIndex()).toInt());
	m_view->reset();

    KConfigGroup cg = config();
    cg.writeEntry("CategoryType", QVariant(m_model->categoryType()));

    emit configNeedsSaving();
}
