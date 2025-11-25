#include "WebServer.h"

namespace WebServer {

// Parse the request line: "GET /index.html?foo=1 HTTP/1.1"
HttpRequest ParseRequest(const String& request) {
    HttpRequest req{};
    char fullPath[256] = {0};

    sscanf(request.c_str(), "%7s %255[^ ]", req.method, fullPath);

    // Split path and query
    char* qMark = strchr(fullPath, '?');
    char query[256] = {0};

    if (qMark) {
        size_t pathLen = qMark - fullPath;
        if (pathLen > sizeof(req.path) - 1)
            pathLen = sizeof(req.path) - 1;
        strncpy(req.path, fullPath, pathLen);
        req.path[pathLen] = 0;

        strncpy(query, qMark + 1, sizeof(query) - 1);
        query[sizeof(query) - 1] = 0;
    } else {
        strncpy(req.path, fullPath, sizeof(req.path) - 1);
        req.path[sizeof(req.path) - 1] = 0;
    }

    // Parse query parameters
    req.queryParams = ParseQuery(query);

    return req;
}

// Parse a query string like "foo=1&bar=2" into a map
std::map<String, String> ParseQuery(const char* query) {
    std::map<String, String> params;
    if (!query)
        return params;

    char buffer[256];
    strncpy(buffer, query, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = 0;

    char* token = strtok(buffer, "&");
    while (token) {
        char* eq = strchr(token, '=');
        if (eq) {
            *eq = 0;  // terminate key
            String key = token;
            String value = eq + 1;
            params[key] = value;
        }
        token = strtok(nullptr, "&");
    }
    return params;
}

}  // namespace WebServer