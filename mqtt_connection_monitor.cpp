/*
 * Copyright (c) 2018 pkjq
 * All rights reserved
 *
 * Licensed under the Apache License, Version 2.0 (the ""License"");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an ""AS IS"" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mqtt_connection_monitor.h"
#include <mgos_mqtt.h>
#include <memory>


namespace mqtt
{
namespace
{
namespace online_messages
{
    constexpr const char online[]   = "{\"online\":true}";
    constexpr const char *die       = "{\"online\":false}";
}


class MqttConnectionMonitor
{
public:
    inline explicit MqttConnectionMonitor(ConnectionEvents events_, const char *online_topic):
        events(std::move(events_))
    {
        mgos_mqtt_add_global_handler(MqttConnectionEventsCallback, &events);

        if (online_topic)
        {
            mgos_sys_config_set_mqtt_will_topic(online_topic);
            mgos_sys_config_set_mqtt_will_retain(true);
            mgos_sys_config_set_mqtt_will_message(online_messages::die);
        }
    }

    MqttConnectionMonitor(const MqttConnectionMonitor&) = delete;
    MqttConnectionMonitor(MqttConnectionMonitor&&) = delete;

private:
    static inline void Call2Callback(const ConnectionEvents::FuncType &func)
    {
        if (func)
            func();
    }

    static void MqttConnectionEventsCallback(struct mg_connection *c, int ev, void *p, void *data)
    {
        (void)c;
        (void)p;

        const auto &callbacks = *static_cast<const ConnectionEvents*>(data);

        switch (ev)
        {
        case MG_EV_CLOSE:
            Call2Callback(callbacks.OnDisconnect);
            break;

        case MG_EV_MQTT_CONNACK:
            if (mgos_sys_config_get_mqtt_will_topic())
            {
                constexpr auto msgLen = sizeof(online_messages::online) / sizeof(online_messages::online[0]) - 1;
                mgos_mqtt_pub(mgos_sys_config_get_mqtt_will_topic(), online_messages::online, msgLen, 1, true);
            }

            Call2Callback(callbacks.OnConnect);
            break;
        }
    }

private:
    ConnectionEvents events;
};
}


///////////////////////////////////////////////////
void CreateConnectionMonitor(ConnectionEvents events, const char *online_topic)
{
    static MqttConnectionMonitor monitor(std::move(events), online_topic);
    (void)monitor;
}
}