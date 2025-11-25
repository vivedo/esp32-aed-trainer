#pragma once

#include <WiFi.h>

namespace Network {
extern IPAddress localIP;
extern IPAddress gateway;
extern IPAddress subnet;

void StartWiFiAP();
}  // namespace Network
