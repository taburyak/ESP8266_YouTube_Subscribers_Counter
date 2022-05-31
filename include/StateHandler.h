#pragma once

#include <Arduino.h>
#include <Setings.h>
#include <Adafruit_PCD8544.h>

Adafruit_PCD8544 display = Adafruit_PCD8544(LCD_CLK_PIN, LCD_MOSI_PIN, LCD_DC_PIN, LCD_CS_PIN, LCD_RST_PIN);

void enterInitialPeriph()
{
    display.begin();

    // you can change the contrast around to adapt the display
    // for the best viewing!
    display.setContrast(25);
    display.setRotation(2);
    display.display(); // show splashscreen

    ServiceState::set(MODE_RUNNING);
}

void enterRunLoop()
{

}

void enterIdle()
{

}

void enterError()
{
    
}