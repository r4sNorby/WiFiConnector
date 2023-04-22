#include <M5Stack.h>
#include <Arduino.h>
#include <WiFi.h>
#include <FastLED.h>
#include "keyboard.h"
#include "selector.h"

// LED
#define NUM_LEDS 10
#define DATA_PIN 15
CRGB leds[NUM_LEDS];

// Class instances
WiFiClient espClient;

// WiFi connection variables
char *chosenNetwork;
char *password;

// FUNCTIONS
// Wifi
void setupWifi();
char *scanForWifi();

// LED function
void setLEDStrip(int colour);

void setup()
{
	M5.begin(); // Init M5Stack.
	M5.Speaker.begin();

	FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); // GRB ordering is assumed
	FastLED.setBrightness(40);
	setLEDStrip(0x0);

	WiFi.mode(WIFI_STA);		 // Set WiFi to station mode and disconnect from an AP
								 // if it was previously connected.
	WiFi.disconnect();			 // Turn off all wifi connections.
	M5.Lcd.println("WIFI SCAN"); // Screen print string.
	delay(100);					 // 100 ms delay.
	M5.Lcd.println("Please press Btn.A to (re)scan");
}

void loop()
{
	// M5.Lcd.setCursor(0, 20); // Set the cursor at (x, y).
	if (M5.BtnA.wasPressed())
	{
		Selector selector;

		// char networks[] = scanForWifi();

		M5.Lcd.clear(); // Clear the screen.
		M5.Lcd.println("scan start");
		int n = WiFi.scanNetworks(); // return the number of networks found.

		char networks[n][33];

		if (n == 0)
		{ // If no network is found.
			M5.Lcd.println("no networks found");
		}
		else
		{ // If have network is found.
			M5.Lcd.printf("networks found:%d\n\n", n);
			for (int i = 0; i < n; ++i)
			{
				// Add ssid's to array
				for (int j = 0; j < 33; ++j)
				{
					networks[i][j] = WiFi.SSID(i)[j];
					M5.Lcd.print(WiFi.SSID(i)[j]);
				}
			}
		}

		chosenNetwork = selector.start(networks, n);
		M5.Lcd.clear();
		M5.Lcd.setCursor(0, 0); // Set the cursor at (x, y).
		M5.Lcd.println(chosenNetwork);
	}
	else if (M5.BtnB.wasPressed())
	{
		M5.Speaker.tone(661, 100);
		if (chosenNetwork != NULL)
		{
			M5.Lcd.clear();
			setupWifi();
		}
	}
	delay(1);
	M5.update(); // Check the status of the key.
}

void setupWifi()
{
	Keyboard keyboard;
	password = keyboard.start();
	delay(10);
	M5.Lcd.printf("Connecting to %s", chosenNetwork);
	WiFi.mode(WIFI_STA);				 // Set the mode to WiFi station mode.
	WiFi.begin(chosenNetwork, password); // Start Wifi connection.

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

// char scanForWifi()
// {
// 	return networks;
// }