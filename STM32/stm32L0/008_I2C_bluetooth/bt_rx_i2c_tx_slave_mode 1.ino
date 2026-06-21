//esp32 act as slave device and 
//it receives data from user via bluetooth 
//and transmit to stm32 using i2c protocol

#include <Wire.h>
#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

#define SLAVE_ADDR 0x08

// Stores latest valid message
String btMessage = "NO DATA";

// Temporary buffer while receiving
String tempBuffer = "";

void setup()
{
    Serial.begin(115200);

    // Start Bluetooth
    SerialBT.begin("ESP32_BT");

    // Start I2C Slave
    Wire.begin(SLAVE_ADDR);

    // Callback for STM32 request
    Wire.onRequest(sendData);

    Serial.println("ESP32 Bluetooth + I2C Slave Ready");
}

void loop()
{
    while (SerialBT.available())
    {
        char c = SerialBT.read();

        // Message completed
        if (c == '\n')
        {
            // Overwrite old message
            btMessage = tempBuffer;

            Serial.println("New Data Stored:");
            Serial.println(btMessage);

            // Clear temp buffer
            tempBuffer = "";
        }
        else
        {
            tempBuffer += c;
        }
    }
}

// Called when STM32 master requests data
void sendData()
{
    Serial.println("STM32 Requested Data");

    Wire.write(btMessage.c_str());

    // Null terminator
    Wire.write('\0');
}