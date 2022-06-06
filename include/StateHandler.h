#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>            //Local DNS Server used for redirecting all rs to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <Setings.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SimpleTimer.h>
#include <SecretDefinitions.h>
// #include <images.h>

// #include <YoutubeApi.h>
// #include <WiFiClientSecure.h>

// For storing configurations
// #include "FS.h"
#include <ArduinoJson.h>

// WiFiManager Libraries
// #include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic

// Replace with your network credentials
const char *ssid        = MY_SSID;
const char *password    = MY_PASSWORD;
const char *apiKey      = YOUTUBE_API_KEY;
const char *channelId   = YOUTUBE_CHANNEL_ID;

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

void displayYouTubeSubscriberCount();

void enterInitialPeriph()
{
    tft.begin();
    tft.setRotation(2);
    tft.fillScreen(ILI9341_BLACK);
    // tft.drawRGBBitmap(45, 160, hotWether, 150, 150);

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

    displayYouTubeSubscriberCount();

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
    displayYouTubeSubscriberCount();

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

//--------------------------------------------------------------------------------------------------
// displayYouTubeSubscriberCount()
//    Displays subscriber count, total number of public videos, and viewer count
//    using the globally defined youtube_channel_id and youtube_api_key vars
//--------------------------------------------------------------------------------------------------
void displayYouTubeSubscriberCount()
{
    // const int json_memory_buffer = JSON_MEMORY_BUFFER;
    int youtube_subscriber_count = 0;
    int youtube_view_count = 0;
    int youtube_video_count = 0;
  
    WiFiClientSecure client;
    client.setInsecure();      // as long as it is SSL we are good, not checking actual ssl key
    HTTPClient https;
    https.useHTTP10(true);    // required for deserializeJson (https://arduinojson.org/v6/how-to/use-arduinojson-with-httpclient/)

    String restful_url = "https://www.googleapis.com/youtube/v3/channels?part=statistics&id=%youtube_channel_id%&key=%api_key%";

    String url = restful_url;
    url.replace("%youtube_channel_id%",channelId);
    url.replace("%api_key%",apiKey);

    Serial.println("displayYouTubeSubscriberCount()");
    Serial.print("calling url: ");
    Serial.println(url);
   
    if ( https.begin(client, url.c_str() )) 
    {  
        int httpCode = https.GET();
        Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
        
        if (httpCode > 0) 
        {
            if (httpCode == 200 ) 
            {
                DynamicJsonDocument doc(JSON_MEMORY_BUFFER);
                DeserializationError error = deserializeJson(doc, https.getStream());
                if (error) 
                {
                    Serial.print("deserializeJson() failed: ");
                    Serial.println(error.f_str());
                    youtube_subscriber_count = -1;
                    youtube_view_count = -1;
                    youtube_video_count = -1;
                }
                else 
                {
                    Serial.println(https.getString());
                    youtube_subscriber_count = doc["items"][0]["statistics"]["subscriberCount"].as<int>();
                    youtube_view_count = doc["items"][0]["statistics"]["viewCount"].as<int>();
                    youtube_video_count = doc["items"][0]["statistics"]["videoCount"].as<int>();
                }
            }
        } 
    }

    https.end();

    char display_line[80];
    // int youtube_logo_width = 250;
    // int youtube_logo_height = 56;
    // int horizontal_center = (300-youtube_logo_width)/2;
    int starting_y_for_text = 180;
    int vertical_spacing = 4;
    int line_number = 0;
    int16_t tbx, tby; uint16_t tbw, tbh; // boundary box window
    uint16_t x, y;

    tft.setTextColor(ILI9341_LIGHTGREY);
    tft.setTextSize(2);

    // youtube logo graphic
    // tft.fillScreen(ILI9341_BLACK);
    // tft.drawBitmap(horizontal_center, 50, youtube_logo, youtube_logo_width, youtube_logo_height, ILI9341_BLACK);

    // subscribers
    sprintf(display_line, "Subscribers");
    tft.getTextBounds(display_line, 0, 0, &tbx, &tby, &tbw, &tbh); // it works for origin 0, 0, fortunately (negative tby!)
    x = ((tft.width() - tbw) / 2) - tbx;
    y = starting_y_for_text;
    tft.setCursor(x, y); // set the postition to start printing text
    tft.print(display_line); // print some text
    ++line_number;

    sprintf(display_line, "%d", youtube_subscriber_count);
    tft.getTextBounds(display_line, 0, 0, &tbx, &tby, &tbw, &tbh); // it works for origin 0, 0, fortunately (negative tby!)
    x = ((tft.width() - tbw) / 2) - tbx;
    y = starting_y_for_text+((tbh+vertical_spacing)*line_number);    // tbh height of font + a few pixels
    tft.setCursor(x, y); // set the postition to start printing text
    tft.print(display_line); // print some text
    line_number += 1;

    // view count
    sprintf(display_line, "View Count");
    tft.getTextBounds(display_line, 0, 0, &tbx, &tby, &tbw, &tbh); // it works for origin 0, 0, fortunately (negative tby!)
    x = ((tft.width() - tbw) / 2) - tbx;
    y = starting_y_for_text+((tbh+vertical_spacing)*line_number);
    tft.setCursor(x, y); // set the postition to start printing text
    tft.print(display_line); // print some text
    ++line_number;

    sprintf(display_line, "%d", youtube_view_count);
    tft.getTextBounds(display_line, 0, 0, &tbx, &tby, &tbw, &tbh); // it works for origin 0, 0, fortunately (negative tby!)
    x = ((tft.width() - tbw) / 2) - tbx;
    y = starting_y_for_text+((tbh+vertical_spacing)*line_number);    // tbh height of font + a few pixels
    tft.setCursor(x, y); // set the postition to start printing text
    tft.print(display_line); // print some text
    line_number += 1;

    // video count
    sprintf(display_line, "Video Count");
    tft.getTextBounds(display_line, 0, 0, &tbx, &tby, &tbw, &tbh); // it works for origin 0, 0, fortunately (negative tby!)
    x = ((tft.width() - tbw) / 2) - tbx;
    y = starting_y_for_text+((tbh+vertical_spacing)*line_number);
    tft.setCursor(x, y); // set the postition to start printing text
    tft.print(display_line); // print some text
    ++line_number;

    sprintf(display_line, "%d", youtube_video_count);
    tft.getTextBounds(display_line, 0, 0, &tbx, &tby, &tbw, &tbh); // it works for origin 0, 0, fortunately (negative tby!)
    x = ((tft.width() - tbw) / 2) - tbx;
    y = starting_y_for_text+((tbh+vertical_spacing)*line_number);    // tbh height of font + a few pixels
    tft.setCursor(x, y); // set the postition to start printing text
    tft.print(display_line); // print some text
    ++line_number;
}
