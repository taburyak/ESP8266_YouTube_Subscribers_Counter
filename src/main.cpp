#include <Arduino.h>
#include <YoutubeApi.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <NTPClient.h>
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

#define LCD_CLK_PIN   14
#define LCD_MISO_PIN  12
#define LCD_MOSI_PIN  13
#define LCD_DC_PIN    5
#define LCD_CS_PIN    15
#define LCD_RST_PIN   4

Adafruit_PCD8544 display = Adafruit_PCD8544(LCD_CLK_PIN, LCD_MOSI_PIN, LCD_DC_PIN, LCD_CS_PIN, LCD_RST_PIN);

// Replace with your network credentials
const char *ssid     = "TP-LINK_BFA2";
const char *password = "8YU8QF7mM4";
char apiKey[45]  = "AIzaSyCx06fJXr32wQO5O78J4IUYKOSzkN7TiM4";
char channelId[30] = "UCHMrCiNnJB6PrRraDDZEyZQ";

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//Week Days
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

bool flagDot = false;

void setup() 
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("*------------------------------------*");
  Serial.println("* ESP8266 YouTube subscriber counter *");
  Serial.println("*------------------------------------*");
  
  display.begin();
  // init done

  // you can change the contrast around to adapt the display
  // for the best viewing!
  display.setContrast(25);
  display.setRotation(2);
  display.display(); // show splashscreen

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }

  // Initialize a NTPClient to get time
  timeClient.begin();
  timeClient.setTimeOffset(3600 * 3);
  timeClient.update();
}

void loop() 
{
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
  display.setCursor(5, 17);
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