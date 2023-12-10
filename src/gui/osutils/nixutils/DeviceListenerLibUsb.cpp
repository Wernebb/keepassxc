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

#include "DeviceListenerLibUsb.h"

#include <QPointer>
#include <libusb.h>

DeviceListenerLibUsb::DeviceListenerLibUsb(QObject* parent)
    : QObject(parent)
    , m_ctx(nullptr)
    , m_callbackRef(0)
{
    if (libusb_init(reinterpret_cast<libusb_context**>(&m_ctx)) != LIBUSB_SUCCESS) {
        qWarning("Unable to initialize libusb. USB devices may not be detected properly.");
    }
}

DeviceListenerLibUsb::~DeviceListenerLibUsb()
{
    if (m_ctx) {
        libusb_exit(static_cast<libusb_context*>(m_ctx));
        m_ctx = nullptr;
    }
}

void DeviceListenerLibUsb::registerHotplugCallback(bool arrived, bool left, int vendorId, int productId)
{
    if (!m_ctx) {
        return;
    }

    if (m_callbackRef) {
        // libusb supports registering multiple callbacks, but other platforms don't.
        deregisterHotplugCallback();
        m_callbackRef = 0;
    }

    int events = 0;
    if (arrived) {
        events |= LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED;
    }
    if (left) {
        events |= LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT;
    }

    libusb_hotplug_callback_handle handle;
    const QPointer that = this;
    m_callbackRef = libusb_hotplug_register_callback(
        static_cast<libusb_context*>(m_ctx),
        events,
        0,
        vendorId,
        productId,
        LIBUSB_HOTPLUG_MATCH_ANY,
        [](libusb_context* ctx, libusb_device* device, libusb_hotplug_event event, void* that) -> int {
            if (!that) {
                return 0;
            }
            emit static_cast<DeviceListenerLibUsb*>(that)->devicePlugged(
                event == LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED, ctx, device);
            return 0;
        },
        that,
        &handle);
    if (m_callbackRef != LIBUSB_SUCCESS) {
        qWarning("Failed to register USB listener callback.");
        m_callbackRef = 0;
    }
}

void DeviceListenerLibUsb::deregisterHotplugCallback()
{
    if (m_ctx) {
        libusb_hotplug_deregister_callback(static_cast<libusb_context*>(m_ctx), m_callbackRef);
    }
}
