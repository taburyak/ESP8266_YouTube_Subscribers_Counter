#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>            //Local DNS Server used for redirecting all rs to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <SPI.h>
#include <Wire.h>
#include <Setings.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SimpleTimer.h>

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

void enterInitialPeriph()
{
    tft.begin();
    tft.setRotation(2);
    tft.fillScreen(ILI9341_BLACK);

    ServiceState::set(MODE_CONNECTING_NET);

    timer.setInterval(1000, timerOnceSecond);
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
    timeClient.setTimeOffset(3600 * 3);
    timeClient.update();

    ServiceState::set(MODE_RUNNING);
}

void enterRun()
{
    timer.run();
}

void enterDisplay()
{
    time_t epochTime = timeClient.getEpochTime();
    struct tm *ptm = gmtime ((time_t *)&epochTime);

    tft.setTextSize(4);
    tft.setCursor(30, 0);
    tft.setTextColor(ILI9341_BLACK);
    tft.print("     ");
    tft.setCursor(30, 0);
    tft.setTextColor(ILI9341_WHITE);
    if(flagDot)
        tft.printf("%02d:%02d", timeClient.getHours(), timeClient.getMinutes());
    else
        tft.printf("%02d %02d", timeClient.getHours(), timeClient.getMinutes());

    // display.clearDisplay();
    // display.setCursor(10, 0);
    // display.setTextSize(2);
    // if(flagDot)
    //     display.printf("%02d:%02d", timeClient.getHours(), timeClient.getMinutes());
    // else
    //     display.printf("%02d %02d", timeClient.getHours(), timeClient.getMinutes());
    // display.setCursor(10, 17);
    // display.setTextSize(1);
    
    // display.printf("%02d/%02d/%4d", ptm->tm_mday, ptm->tm_mon+1, ptm->tm_year+1900);
    // display.setCursor(10, 27);
    // display.setTextSize(1);
    // display.println(weekDays[timeClient.getDay()]);
    // display.display();
    
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

//---------------------------------------------------------
  // time_t epochTime = timeClient.getEpochTime();
  // Serial.print("Epoch Time: ");
  // Serial.println(epochTime);
  
  // String formattedTime = timeClient.getFormattedTime();
  // Serial.print("Formatted Time: ");
  // Serial.println(formattedTime);  

  // int currentHour = timeClient.getHours();
  // Serial.print("Hour: ");
  // Serial.println(currentHour);  

  // int currentMinute = timeClient.getMinutes();
  // Serial.print("Minutes: ");
  // Serial.println(currentMinute); 
   
  // int currentSecond = timeClient.getSeconds();
  // Serial.print("Seconds: ");
  // Serial.println(currentSecond);  

  // String weekDay = weekDays[timeClient.getDay()];
  // Serial.print("Week Day: ");
  // Serial.println(weekDay);    

  // //Get a time structure
  // struct tm *ptm = gmtime ((time_t *)&epochTime); 

  // int monthDay = ptm->tm_mday;
  // Serial.print("Month day: ");
  // Serial.println(monthDay);

  // int currentMonth = ptm->tm_mon+1;
  // Serial.print("Month: ");
  // Serial.println(currentMonth);

  // String currentMonthName = months[currentMonth-1];
  // Serial.print("Month name: ");
  // Serial.println(currentMonthName);

  // int currentYear = ptm->tm_year+1900;
  // Serial.print("Year: ");
  // Serial.println(currentYear);

  // //Print complete date:
  // String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
  // Serial.print("Current date: ");
  // Serial.println(currentDate);

  // Serial.println("");