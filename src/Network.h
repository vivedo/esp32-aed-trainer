#pragma once

#include <WiFi.h>

extern IPAddress localIP;
extern IPAddress gateway;
extern IPAddress subnet;

void WiFiAP_Start();
