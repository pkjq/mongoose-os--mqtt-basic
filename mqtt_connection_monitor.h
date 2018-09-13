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

#ifndef G30C93F6B_855B_4928_A4AB_E9B536F47B26
#define G30C93F6B_855B_4928_A4AB_E9B536F47B26


#include <functional>


namespace mqtt
{
struct ConnectionEvents
{
    //using FuncType = void();
    using FuncType = std::function<void()>;

    FuncType OnConnect {};
    FuncType OnDisconnect {};
};


//! Singleton (second call is ignored!)
//! @param online_topic - if defined then message about online state be generated on MQTT connect\disconnect
void CreateConnectionMonitor(ConnectionEvents events, const char *online_topic = nullptr);
}


#endif//G30C93F6B_855B_4928_A4AB_E9B536F47B26
