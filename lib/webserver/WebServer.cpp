#include "WebServer.h"

#include <Arduino.h>
#include <Logger.h>
#include <SD.h>
#include <WiFi.h>

#include <map>

#define REQUEST_TIMEOUT_MS 2000

namespace WebServer {

static char UI_BASE_PATH[] = "/ui";

static void Task(void*);
static void ProcessRequest(WiFiClient& client, String& request);
static void ServeFile(WiFiClient& client, HttpRequest* req);

void Start() {
    xTaskCreatePinnedToCore(Task, "WebServer", 6000, NULL, 0, NULL, 0);
}

static void Task(void*) {
    LOG_D("Web Server started");

    WiFiServer server(80);
    server.begin();

    unsigned long currentTime, previousTime;
    String request;

    for (;;) {
        WiFiClient client = server.available();

        if (client) {
            currentTime = millis();
            previousTime = currentTime;

            LOG_D("New Client.");
            String currentLine = "";

            while (client.connected() && currentTime - previousTime <= REQUEST_TIMEOUT_MS) {
                currentTime = millis();
                if (client.available()) {
                    char c = client.read();
                    request += c;

                    if (c == '\n') {
                        if (currentLine.length() == 0) {
                            ProcessRequest(client, request);
                            break;
                        } else {
                            currentLine = "";
                        }
                    } else if (c != '\r') {
                        currentLine += c;
                    }
                }
            }

            request = "";

            client.stop();
            LOG_D("Client disconnected.");
        }
    }
}

static void ProcessRequest(WiFiClient& client, String& request) {
    HttpRequest req = ParseRequest(request);

    // Request router
    if (strncmp(req.method, "POST", 4) == 0) {
        // TODO: handle POST requests
        return;
    } else if (strncmp(req.method, "GET", 3) == 0) {
        ServeFile(client, &req);
    } else {
        LOG_W("Unsupported HTTP method: %s", req.method);
        client.println(FPSTR(RESPONSE_NOT_FOUND));
    }
}

void ServeFile(WiFiClient& client, HttpRequest* req) {
    // Determine if the path already points to a file
    const char* lastSlash = strrchr(req->path, '/');
    const char* lastDot = strrchr(req->path, '.');

    // A path has an extension only if a dot exists after the last slash
    bool hasExtension = lastDot && (!lastSlash || lastDot > lastSlash);

    char finalPath[256] = {0};

    if (!hasExtension) {
        // Normalize trailing slash to avoid "/foo//index.html"
        if (req->path[strlen(req->path) - 1] == '/')
            snprintf(finalPath, sizeof(finalPath), "%sindex.html", req->path);
        else
            snprintf(finalPath, sizeof(finalPath), "%s/index.html", req->path);
    } else {
        strcpy(finalPath, req->path);
    }

    // Prepend UI base path
    char filePath[256] = {0};
    snprintf(filePath, sizeof(filePath), "%s%s", UI_BASE_PATH, finalPath);

    File file = SD.open(filePath, FILE_READ);

    if (!file) {
        LOG_W("Failed to open file %s for reading", filePath);
        client.println(FPSTR(RESPONSE_NOT_FOUND));
    } else {
        LOG_D("Serving file %s", filePath);
        client.print(FPSTR(RESPONSE_OK_HEADER));
        while (file.available()) {
            client.write(file.read());
        }
    }

    client.println();
    file.close();
}

}  // namespace WebServer