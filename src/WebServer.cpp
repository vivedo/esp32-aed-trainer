#include "WebServer.h"

#include <Arduino.h>
#include <SD.h>
#include <WiFi.h>

#include <map>

#include "Logger.h"
#include "WebServerConstants.h"

#define REQUEST_TIMEOUT_MS 2000

static void WebServer_Task(void *);
static void WebServer_ProcessRequest(WiFiClient &client, String &request);
static void WebServer_ServeFile(WiFiClient &client, const char *path);

static char UI_BASE_PATH[] = "/ui";

void WebServer_Start() { xTaskCreatePinnedToCore(WebServer_Task, "WebServer", 6000, NULL, 0, NULL, 0); }

static void WebServer_Task(void *) {
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
                            WebServer_ProcessRequest(client, request);
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

static void WebServer_ProcessRequest(WiFiClient &client, String &request) {
    // Parse the request line
    char method[8] = {0};
    char fullPath[256] = {0};
    sscanf(request.c_str(), "%7s %255[^ ]", method, fullPath);

    // Separate path and query parameters
    char path[256] = {0};
    char query[256] = {0};

    char *qMark = strchr(fullPath, '?');
    if (qMark) {
        size_t pathLen = qMark - fullPath;
        if (pathLen > sizeof(path) - 1) pathLen = sizeof(path) - 1;
        strncpy(path, fullPath, pathLen);
        path[pathLen] = 0;

        strncpy(query, qMark + 1, sizeof(query) - 1);
        query[sizeof(query) - 1] = 0;
    } else {
        strncpy(path, fullPath, sizeof(path) - 1);
        path[sizeof(path) - 1] = 0;
    }

    // Parse query parameters into a map
    std::map<String, String> queryParams;
    char *token = strtok(query, "&");
    while (token) {
        char *eq = strchr(token, '=');
        if (eq) {
            *eq = 0;
            String key = token;
            String value = eq + 1;
            queryParams[key] = value;
        }
        token = strtok(nullptr, "&");
    }

    // Request router
    if (strncmp(method, "POST", 4) == 0) {
        // TODO: handle POST requests
        return;
    } else if (strncmp(method, "GET", 3) == 0) {
        WebServer_ServeFile(client, path);
    } else {
        LOG_W("Unsupported HTTP method: %s", method);
        client.println(FPSTR(WS_RESPONSE_NOT_FOUND));
    }
}

void WebServer_ServeFile(WiFiClient &client, const char *path) {
    // Determine if the path already points to a file
    const char *lastSlash = strrchr(path, '/');
    const char *lastDot = strrchr(path, '.');

    // A path has an extension only if a dot exists after the last slash
    bool hasExtension = lastDot && (!lastSlash || lastDot > lastSlash);

    char finalPath[256] = {0};

    if (!hasExtension) {
        // Normalize trailing slash to avoid "/foo//index.html"
        if (path[strlen(path) - 1] == '/')
            snprintf(finalPath, sizeof(finalPath), "%sindex.html", path);
        else
            snprintf(finalPath, sizeof(finalPath), "%s/index.html", path);
    } else {
        strcpy(finalPath, path);
    }

    // Prepend UI base path
    char filePath[256] = {0};
    snprintf(filePath, sizeof(filePath), "%s%s", UI_BASE_PATH, finalPath);

    File file = SD.open(filePath, FILE_READ);

    if (!file) {
        LOG_W("Failed to open file %s for reading", filePath);
        client.println(FPSTR(WS_RESPONSE_NOT_FOUND));
    } else {
        LOG_D("Serving file %s", filePath);
        client.print(FPSTR(WS_RESPONSE_OK_HEADER));
        while (file.available()) {
            client.write(file.read());
        }
    }

    client.println();
    file.close();
}