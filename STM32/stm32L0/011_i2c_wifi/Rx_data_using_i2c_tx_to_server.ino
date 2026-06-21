//receive data from stm32 and send to server

#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define SLAVE_ADDR 0x08

const char* ssid = "vamshi";
const char* password = "11111111";

String apiKey = "4Z9CZUR13Z7F8BYI";

String receivedData = "";

// =========================
// I2C Receive Callback
// =========================
void receiveEvent(int howMany)
{
    receivedData = "";

    while(Wire.available())
    {
        char c = Wire.read();
        receivedData += c;
    }

    Serial.print("Received I2C Data: ");
    Serial.println(receivedData);

    sendToThingSpeak(receivedData);
}

// =========================
// Setup
// =========================
void setup()
{
    Serial.begin(115200);

    // ESP32 as I2C SLAVE
    // SDA = GPIO21
    // SCL = GPIO22
    Wire.begin(SLAVE_ADDR);

    // Register callback
    Wire.onReceive(receiveEvent);

    Serial.println("ESP32 I2C Slave Started");

    // WiFi Connection
    WiFi.begin(ssid, password);

    Serial.print("Connecting to WiFi");

    while(WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi Connected");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

// =========================
// Main Loop
// =========================
void loop()
{
    // Nothing needed here
}

// =========================
// Send Data to ThingSpeak
// =========================
void sendToThingSpeak(String msg)
{
    if(WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;

        // Clean message
        msg.replace(" ", "%20");
        msg.replace("\n", "");
        msg.replace("\r", "");

        String url =
        "https://api.thingspeak.com/update?api_key="
        + apiKey +
        "&status=" + msg;

        Serial.println(url);

        http.begin(url);

        int httpCode = http.GET();

        Serial.print("HTTP Response Code: ");
        Serial.println(httpCode);

        http.end();
    }
    else
    {
        Serial.println("WiFi Disconnected");
    }
}
