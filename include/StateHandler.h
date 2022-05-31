#pragma once

#include <Arduino.h>
#include <Setings.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Adafruit_PCD8544.h>

// Replace with your network credentials
const char *ssid        = MY_SSID;
const char *password    = MY_PASSWORD;
char apiKey[45]         = MY_API_KEY;
char channelId[30]      = MY_CHANNEL_ID;

Adafruit_PCD8544 display = Adafruit_PCD8544(LCD_CLK_PIN, LCD_MOSI_PIN, LCD_DC_PIN, LCD_CS_PIN, LCD_RST_PIN);

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

void enterInitialPeriph()
{
    display.begin();

    // you can change the contrast around to adapt the display
    // for the best viewing!
    display.setContrast(25);
    display.setRotation(2);
    display.display(); // show splashscreen

    ServiceState::set(MODE_CONNECTING_NET);
}

void enterConnectNet()
{
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(".");
    }

    ServiceState::set(MODE_CONNECTING_CLOUD);
}

void enterConnectCloud()
{
    // Initialize a NTPClient to get time
    timeClient.begin();
    timeClient.setTimeOffset(3600 * 3);
    timeClient.update();

    ServiceState::set(MODE_RUNNING);
}

void enterRun()
{

}

void enterIdle()
{

}

void enterError()
{
    
}