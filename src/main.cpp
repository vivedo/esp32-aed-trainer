#include <Arduino.h>
#include <Logger.h>
#include <Network.h>
#include <WebServer.h>
#include <AudioService.h>

void setup()
{
    Serial.begin(115200);
    LoggerNS::GlobalLogger.setLogLevel(LoggerNS::Logger::LogLevel::Debug);

    WiFiAP_Start();

    AudioService_Start();
    WebServer_Start();
}

void loop() {}
