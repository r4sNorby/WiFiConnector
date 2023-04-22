#include <M5Stack.h>
#include "wificonnector.h"

void setup()
{
    M5.begin(); // Init M5Stack.
    M5.Speaker.begin();
    M5.Lcd.println("Please press Btn.A start connecting");
}

void loop()
{
    if (M5.BtnA.wasPressed())
    {
        WiFiConnector connector;
        connector.scanForWiFi();
    }
    delay(1);
    M5.update();
}