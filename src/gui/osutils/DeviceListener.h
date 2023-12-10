/*
 * Copyright (C) 2023 KeePassXC Team <team@keepassxc.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 or (at your option)
 * version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DEVICELISTENER_H
#define DEVICELISTENER_H

#include <QObject>
#include <QScopedPointer>

#if defined(Q_OS_WIN)
// TODO
#elif defined(Q_OS_MACOS)
#include "macutils/DeviceListenerMac.h"
#elif defined(Q_OS_UNIX)
#include "nixutils/DeviceListenerLibUsb.h"
#endif

class DeviceListener : public QObject
{
    Q_OBJECT

public:
    static constexpr int MATCH_ANY = -1;

    explicit DeviceListener(QObject* parent = nullptr);
    DeviceListener(const DeviceListener&) = delete;
    ~DeviceListener() override;

    /**
     * Register a hotplug notification callback.
     *
     * Fires devicePlugged() or deviceUnplugged() when the state of a matching device changes.
     * The signals are supplied with the platform-specific context and ID of the firing device.
     * Registering a new callback with the same DeviceListener will unregister any previous callbacks.
     *
     * @param arrived listen for new devices
     * @param left listen for device unplug
     * @param vendorId vendor ID to listen for or DeviceListener::MATCH_ANY
     * @param productId product ID to listen for or DeviceListener::MATCH_ANY
     * @return callback handle
     */
    void registerHotplugCallback(bool arrived, bool left, int vendorId = MATCH_ANY, int productId = MATCH_ANY);
    void deregisterHotplugCallback();

signals:
    void devicePlugged(bool state, void* ctx, void* device);

private:
    DEVICELISTENER_IMPL* impl();
    QScopedPointer<QObject> m_platformImpl;
};

#endif // DEVICELISTENER_H
