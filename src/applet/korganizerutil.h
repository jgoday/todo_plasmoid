#ifndef KORGANIZERUTIL_H
#define KORGANIZERUTIL_H

#include <QObject>

class KOrganizerUtil : public QObject
{
public:
    static void showTodo(const QString &uid);
    static void showAddTodo();

private:
    static void showMainWindow();
};
#endif

