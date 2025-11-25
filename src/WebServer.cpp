#include <WebServer.h>

#define REQUEST_TIMEOUT_MS 2000

static void WebServer_Task(void *);
static void WebServer_ProcessRequest(WiFiClient &client, String &request);

void WebServer_Start()
{
    xTaskCreatePinnedToCore(
        WebServer_Task,
        "WebServer",
        6000,
        NULL,
        0,
        NULL,
        0);
}

static void WebServer_Task(void *)
{
    LOG_D("Web Server started");

    WiFiServer server(80);
    server.begin();

    unsigned long currentTime, previousTime;
    String request;

    for (;;)
    {
        WiFiClient client = server.available();

        if (client)
        {
            currentTime = millis();
            previousTime = currentTime;

            LOG_D("New Client.");
            String currentLine = "";

            while (client.connected() && currentTime - previousTime <= REQUEST_TIMEOUT_MS)
            {
                currentTime = millis();
                if (client.available())
                {
                    char c = client.read();
                    request += c;

                    if (c == '\n')
                    {
                        if (currentLine.length() == 0)
                        {
                            WebServer_ProcessRequest(client, request);
                            break;
                        }
                        else
                        {
                            currentLine = "";
                        }
                    }
                    else if (c != '\r')
                    {
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

static void WebServer_ProcessRequest(WiFiClient &client, String &request)
{
    LOG_D("Processing request:\n%s", request.c_str());

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();

    client.println("<!DOCTYPE html><html>");
    client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
    client.println("</head>");

    client.println("<body><h1>ESP32 Web Server</h1></body></html>");

    client.println();
}