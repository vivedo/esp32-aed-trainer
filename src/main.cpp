#include <Arduino.h>

#include "AudioService.h"
#include "Logger.h"
#include "Network.h"
#include "WebServer.h"

void setup() {
    Serial.begin(115200);
    GlobalLogger.setLogLevel(Logger::LogLevel::Debug);

    WiFiAP_Start();

    AudioService_Start();
    WebServer_Start();
}

void loop() {}
