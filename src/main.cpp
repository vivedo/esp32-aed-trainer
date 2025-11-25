#include <Arduino.h>
#include <AudioService.h>
#include <Logger.h>
#include <WebServer.h>

#include "Network.h"

void setup() {
    Serial.begin(115200);
    GlobalLogger.setLogLevel(Logger::LogLevel::Debug);

    Network::StartWiFiAP();

    AudioService::Start();
    WebServer::Start();
}

void loop() {
}
