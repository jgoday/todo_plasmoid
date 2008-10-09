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
#include "korganizerutil.h"

// autogenerated dbus interfaces
#include "calendar_interface.h"
#include "korganizer_interface.h"

// qt headers
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>

// kde headers
#include <KLocale>
#include <KToolInvocation>
#include <KWindowSystem>


void KOrganizerUtil::showMainWindow()
{
    QDBusInterface kontactInterface("org.kde.korganizer",
                                    "/kontact/MainWindow_1",
                                    "org.kde.KMainWindow",
                                    QDBusConnection::sessionBus());
    QDBusReply<long> kontactWinId = kontactInterface.call("winId");

    KWindowSystem::forceActiveWindow (kontactWinId, 1);
}

void KOrganizerUtil::showTodo(const QString &uid)
{
    if(!QDBusConnection::sessionBus().interface()->isServiceRegistered("org.kde.kontact"))
    {
        KToolInvocation::kdeinitExecWait("kontact");
    }

    OrgKdeKorganizerKorganizerInterface interface("org.kde.korganizer",
                                                  "/Korganizer",
                                                  QDBusConnection::sessionBus());
    interface.showIncidence(uid);
    KOrganizerUtil::showMainWindow();
}

void KOrganizerUtil::showAddTodo()
{
    OrgKdeKorganizerCalendarInterface interface("org.kde.korganizer",
                                                  "/Calendar",
                                                  QDBusConnection::sessionBus());
    interface.openTodoEditor(i18n("new todo"));
    KOrganizerUtil::showMainWindow();
}

