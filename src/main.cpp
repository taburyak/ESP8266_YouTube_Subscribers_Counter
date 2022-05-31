#include <Arduino.h>
#include <YoutubeApi.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include "SimpleTimer.h"
#include "ProvisioningService.h"

// For storing configurations
#include "FS.h"
#include <ArduinoJson.h>

// WiFiManager Libraries
#include <DNSServer.h>            //Local DNS Server used for redirecting all rs to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic

//Week Days
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

bool flagDot = false;

void setup() 
{
  Serial.begin(115200);

  ServiceProvisioning.begin();
}

void loop() 
{
  ServiceProvisioning.run();  

  time_t epochTime = timeClient.getEpochTime();
  Serial.print("Epoch Time: ");
  Serial.println(epochTime);
  
  String formattedTime = timeClient.getFormattedTime();
  Serial.print("Formatted Time: ");
  Serial.println(formattedTime);  

  int currentHour = timeClient.getHours();
  Serial.print("Hour: ");
  Serial.println(currentHour);  

  int currentMinute = timeClient.getMinutes();
  Serial.print("Minutes: ");
  Serial.println(currentMinute); 
   
  int currentSecond = timeClient.getSeconds();
  Serial.print("Seconds: ");
  Serial.println(currentSecond);  

  String weekDay = weekDays[timeClient.getDay()];
  Serial.print("Week Day: ");
  Serial.println(weekDay);    

  //Get a time structure
  struct tm *ptm = gmtime ((time_t *)&epochTime); 

  int monthDay = ptm->tm_mday;
  Serial.print("Month day: ");
  Serial.println(monthDay);

  int currentMonth = ptm->tm_mon+1;
  Serial.print("Month: ");
  Serial.println(currentMonth);

  String currentMonthName = months[currentMonth-1];
  Serial.print("Month name: ");
  Serial.println(currentMonthName);

  int currentYear = ptm->tm_year+1900;
  Serial.print("Year: ");
  Serial.println(currentYear);

  //Print complete date:
  String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
  Serial.print("Current date: ");
  Serial.println(currentDate);

  Serial.println("");

  display.clearDisplay();
  display.setCursor(10, 0);
  display.setTextSize(2);
  if(flagDot)
    display.printf("%02d:%02d", currentHour, currentMinute);
  else
    display.printf("%02d %02d", currentHour, currentMinute);
  display.setCursor(0, 17);
  display.setTextSize(1);
  display.print(monthDay);
  display.print(" ");
  display.print(currentMonthName);
  display.print(" ");
  display.println(currentYear);
  display.setCursor(20, 27);
  display.setTextSize(1);
  display.println(weekDay);
  display.display();
  flagDot = !flagDot;
  delay(1000);
}