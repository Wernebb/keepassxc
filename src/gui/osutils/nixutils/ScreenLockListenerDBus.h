/*
 *  Copyright (C) 2017 KeePassXC Team <team@keepassxc.org>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 or (at your option)
 *  version 3 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SCREENLOCKLISTENERDBUS_H
#define SCREENLOCKLISTENERDBUS_H
#include "gui/osutils/ScreenLockListenerPrivate.h"
#include <QDBusMessage>

class ScreenLockListenerDBus : public ScreenLockListenerPrivate
{
    Q_OBJECT
public:
    explicit ScreenLockListenerDBus(QObject* parent = nullptr);

private slots:
    void gnomeSessionStatusChanged(uint status);
    void logindPrepareForSleep(bool beforeSleep);
    void unityLocked();
    void freedesktopScreenSaver(bool status);
    void login1SessionObjectReceived(QDBusMessage);
};

#endif // SCREENLOCKLISTENERDBUS_H
