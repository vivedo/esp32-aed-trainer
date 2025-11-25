#include "Network.h"

#include <Logger.h>

namespace Network {

IPAddress localIP(192, 168, 100, 1);
IPAddress gateway(192, 168, 100, 1);
IPAddress subnet(255, 255, 255, 0);

void StartWiFiAP() {
    uint8_t mac[6];
    WiFi.macAddress(mac);
    char ssid[32];
    snprintf(ssid, sizeof(ssid), "AEDTrainer_%02X%02X%02X", mac[3], mac[4], mac[5]);

    WiFi.softAP(ssid, NULL);

    if (!WiFi.softAPConfig(localIP, gateway, subnet)) {
        Serial.println("Failed to configure AP IP");
    }

    LOG_D("WiFi Hotspot started");
    LOG_D("AP SSID:\t%s", ssid);
    LOG_D("AP Addr:\t%s", WiFi.softAPIP().toString().c_str());
}

}  // namespace Network