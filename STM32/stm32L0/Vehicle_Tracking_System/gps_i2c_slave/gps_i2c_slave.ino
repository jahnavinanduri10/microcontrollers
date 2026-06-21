//esp32 gPS + I2C slave
#include <Wire.h>
#include <TinyGPS++.h>
 
#define SLAVE_ADDR 0x08
 
TinyGPSPlus gps;
HardwareSerial gpsSerial(2);
 
// Fixed buffer
char gpsData[40] = "NO GPS FIX";
 
void requestEvent()
{
    Wire.write((uint8_t*)gpsData, strlen(gpsData));
 
    Serial.println("Master Requested Data");
    Serial.print("Sent: ");
    Serial.println(gpsData);
    Serial.println();
}
 
void setup()
{
    Serial.begin(115200);
 
    // GPS UART
    gpsSerial.begin(9600, SERIAL_8N1, 16, 17);
 
    // ESP32 as I2C Slave
    Wire.begin(SLAVE_ADDR);
 
    // Register callback
    Wire.onRequest(requestEvent);
 
    Serial.println("ESP32 GPS I2C Slave Started");
}
 
void loop()
{
    // Read GPS continuously
    while (gpsSerial.available())
    {
        char c = gpsSerial.read();
        gps.encode(c);
    }
 
    // GPS VALID
    if (gps.location.isUpdated())
    {
        float latitude = gps.location.lat();
        float longitude = gps.location.lng();
 
        // Store into fixed buffer
        snprintf(gpsData, sizeof(gpsData),
                 "%.6f,%.6f",
                 latitude,
                 longitude);
 
        Serial.println("GPS Updated:");
        Serial.println(gpsData);
 
        Serial.print("Satellites: ");
        Serial.println(gps.satellites.value());
 
        Serial.println();
    }
 
    // Searching
    if (!gps.location.isValid())
    {
        static unsigned long lastMsg = 0;
 
        if (millis() - lastMsg > 2000)
        {
            lastMsg = millis();
 
            strcpy(gpsData, "NO GPS FIX");
 
            Serial.println("Searching for satellites...");
            Serial.print("Visible Satellites: ");
            Serial.println(gps.satellites.value());
 
            Serial.println();
        }
    }
}