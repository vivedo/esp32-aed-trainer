#ifndef NETWORK_H
#define NETWORK_H

#include <Logger.h>
#include <WiFi.h>

extern IPAddress localIP;
extern IPAddress gateway;
extern IPAddress subnet;

void WiFiAP_Start();

#endif // NETWORK_H