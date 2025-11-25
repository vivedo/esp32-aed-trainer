#pragma once

#include <Arduino.h>
#include <pgmspace.h>

#include <map>

#define WS_HTTP_RESPONSE_HEADER(code, reason) \
    "HTTP/1.1 " #code " " reason              \
    "\r\n"                                    \
    "Content-Type: text/html\r\n"             \
    "Connection: close\r\n\r\n"

#define WS_HTTP_ERROR(code, reason)                       \
    WS_HTTP_RESPONSE_HEADER(code, reason)                 \
    "<!DOCTYPE html><html><head><title>" #code " " reason \
    "</title></head>"                                     \
    "<body><center><h1>" #code " " reason                 \
    "</h1></center>"                                      \
    "<hr><center>AEDTrainer</center></body></html>"

namespace WebServer {

struct HttpRequest {
    char method[8];                        // GET, POST, etc.
    char path[256];                        // path without query
    std::map<String, String> queryParams;  // parsed query string
};

void Start();

std::map<String, String> ParseQuery(const char* query);
HttpRequest ParseRequest(const String& request);

// Response headers
const char RESPONSE_OK_HEADER[] PROGMEM = WS_HTTP_RESPONSE_HEADER(200, "OK");

// Full HTTP error responses
const char RESPONSE_NOT_FOUND[] PROGMEM = WS_HTTP_ERROR(404, "Not Found");
const char RESPONSE_METHOD_NOT_ALLOWED[] PROGMEM = WS_HTTP_ERROR(405, "Method Not Allowed");

}  // namespace WebServer