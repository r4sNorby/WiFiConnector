#include <M5Stack.h>
#include <Arduino.h>
#include <WiFi.h>
#include <FastLED.h>
#include "keyboard.h"

// LED
#define NUM_LEDS 10
#define DATA_PIN 15
CRGB leds[NUM_LEDS];

// Class instances
WiFiClient espClient;

// WiFi connection variables
const char *ssid = "Sde-Guest";
const char *password;

// FUNCTIONS
// Wifi
void setupWifi();
void scanForWifi();

// LEDs OFF
void setLEDStrip(int colour);

void setup()
{
  M5.begin();       // Init M5Stack.
  M5.Power.begin(); // Init power
  // M5.Lcd.setTextSize(2);

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); // GRB ordering is assumed
  FastLED.setBrightness(40);
  setLEDStrip(0x0);

  WiFi.mode(WIFI_STA);       // Set WiFi to station mode and disconnect from an AP
                             // if it was previously connected.
  WiFi.disconnect();         // Turn off all wifi connections.
  M5.Lcd.print("WIFI SCAN"); // Screen print string.
  delay(100);                // 100 ms delay.
}

void loop()
{
  M5.Lcd.setCursor(0, 0); // Set the cursor at (0,0).
  M5.Lcd.println("Press A to (re)scan");
  M5.update(); // Check the status of the key.
  if (M5.BtnA.isPressed())
  { // If button A is pressed.
    scanForWifi();
  }
  else if (M5.BtnB.isPressed())
  {
    M5.Lcd.clear();
    setupWifi();
  }
}

void scanForWifi()
{
  M5.Lcd.clear(); // Clear the screen.
  M5.Lcd.println("Scan start");
  int n = WiFi.scanNetworks(); // return the number of networks found.
  if (n == 0)
  { // If no network is found.
    M5.Lcd.println("No networks found");
  }
  else
  { // If have network is found.
    M5.Lcd.printf("Networks found:%d\n\n", n);
    int lineHeight = 62;
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found.
      if (i != 0)
      {
        lineHeight = lineHeight + 20;
      }
      M5.Lcd.printf("%d:", i + 1);
      // M5.Lcd.drawNumber(i + 1, 20, lineHeight);
      M5.Lcd.print(WiFi.SSID(i));
      M5.Lcd.printf("(%d)", WiFi.RSSI(i));
      M5.Lcd.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
      delay(10);
    }
  }
  delay(1000);
}

void setupWifi()
{
  Keyboard keyboard;
  password = keyboard.start();
  delay(10);
  M5.Lcd.printf("Connecting to %s", ssid);
  WiFi.mode(WIFI_STA);        // Set the mode to WiFi station mode.
  WiFi.begin(ssid, password); // Start Wifi connection.

  int wifiWaitTime = 0;
  int i = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    if (i > 4)
    {
      setLEDStrip(0x0);
      i = 0;
    }
    else
    {
      leds[i] = 0x0000FF;
      leds[i + 5] = 0x0000FF;
      i++;
      // Show color
      FastLED.show();
    }

    M5.Lcd.print(".");
    delay(500);
    wifiWaitTime++;

    if (wifiWaitTime >= 20)
    {
      setLEDStrip(0xFF0000);
      delay(1000);
      break;
    }
  }

  if (wifiWaitTime >= 15)
  {
    M5.Lcd.printf("\nFailed. Press Btn.B to try again\n");
    // Turn LEDs off
    setLEDStrip(0x0);
  }
  else
  {
    M5.Lcd.printf("\nSuccess\n");
    setLEDStrip(0x00FF00);
    delay(1000);
    // Turn LEDs off
    setLEDStrip(0x0);
  }
}

void setLEDStrip(int colour)
{
  for (int i = 0; i < 10; i++)
  {
    leds[i] = colour;
  }
  FastLED.show();
}