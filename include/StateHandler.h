#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>            //Local DNS Server used for redirecting all rs to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <Wire.h>
#include <Setings.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SimpleTimer.h>
#include <SecretDefinitions.h>
#include <images.h>

// #include <YoutubeApi.h>
// #include <WiFiClientSecure.h>

// For storing configurations
// #include "FS.h"
// #include <ArduinoJson.h>

// WiFiManager Libraries
// #include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic

// Replace with your network credentials
const char *ssid        = MY_SSID;
const char *password    = MY_PASSWORD;
char apiKey[45]         = MY_API_KEY;
char channelId[30]      = MY_CHANNEL_ID;

bool flagDot = false;

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
//Week Days
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//Month names
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

SimpleTimer timer;

void timerOnceSecond();
void timerRefreshData();

void enterInitialPeriph()
{
    tft.begin();
    tft.setRotation(2);
    tft.fillScreen(ILI9341_BLACK);
    tft.drawRGBBitmap(45, 160, hotWether, 150, 150);

    ServiceState::set(MODE_CONNECTING_NET);

    timer.setInterval(1000U, timerOnceSecond);
    timer.setInterval(1000U * 60U, timerRefreshData);
}

void enterConnectNet()
{
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Whoo! Connecting!");

    ServiceState::set(MODE_CONNECTING_CLOUD);
}

void enterConnectCloud()
{
    // Initialize a NTPClient to get time
    timeClient.begin();
    timeClient.setTimeOffset(3600 * TIME_ZONE);
    timeClient.update();

    ServiceState::set(MODE_RUNNING);
}

void enterRun()
{
    timer.run();
}

void enterDisplay()
{
    int16_t x, y;
    uint16_t w, h;

    time_t epochTime = timeClient.getEpochTime();
    struct tm *ptm = gmtime ((time_t *)&epochTime);
    
    int currentMonth = ptm->tm_mon+1;
    String currentMonthName = months[currentMonth-1];    

    String bufStr;
    
    char bufChar[6];
    
    if(flagDot)
    {
        sprintf(bufChar, "%02d:%02d", timeClient.getHours(), timeClient.getMinutes());            
    }
        
    else
    {
        sprintf(bufChar, "%02d %02d", timeClient.getHours(), timeClient.getMinutes());
    }
    
    bufStr = String(bufChar);

    tft.setTextSize(TFT_CLOCK_SIZE);
    tft.getTextBounds(bufStr, 0, 0, &x, &y, &w, &h);
    tft.setTextColor(TFT_CLOCK_COLOR, TFT_BACKGRAUND_COLOR);
    tft.setCursor((ILI9341_TFTWIDTH / 2) - (w / 2), TFT_CLOCK_Y);
    tft.print(bufStr);

    sprintf(bufChar, "%02d", ptm->tm_mday);

    bufStr = "   " + String(bufChar) + " " + currentMonthName + " " + String(ptm->tm_year+1900) + "   ";

    tft.setTextSize(TFT_DATE_SIZE);
    tft.getTextBounds(bufStr, 0, 0, &x, &y, &w, &h);
    tft.setTextColor(TFT_DATE_COLOR, TFT_BACKGRAUND_COLOR);
    tft.setCursor((ILI9341_TFTWIDTH / 2) - (w / 2), TFT_DATE_Y);
    tft.print(bufStr);
    
    bufStr = "  " + weekDays[timeClient.getDay()] + "  ";

    tft.setTextSize(TFT_WEEKDAY_SIZE);
    tft.getTextBounds(bufStr, 0, 0, &x, &y, &w, &h);
    
    if(timeClient.getDay() == 0 || timeClient.getDay() == 6)
    {
        tft.setTextColor(TFT_WEEKENDS_COLOR, TFT_BACKGRAUND_COLOR);
    }
    else
    {
        tft.setTextColor(TFT_WEEKDAYS_COLOR, TFT_BACKGRAUND_COLOR);
    }
    
    tft.setCursor((ILI9341_TFTWIDTH / 2) - (w / 2), TFT_WEEKDAY_Y);
    tft.print(bufStr);
    
    ServiceState::set(MODE_RUNNING);
}

void enterRefreshData()
{
    timeClient.update();

    ServiceState::set(MODE_RUNNING);
}

void enterIdle()
{

}

void enterError()
{
    
}

void timerOnceSecond()
{
    flagDot = !flagDot;
    ServiceState::set(MODE_DISPLAY);
}

void timerRefreshData()
{
    ServiceState::set(MODE_REFRESH_DATA);
}
