#pragma once

#define BOARD_FIRMWARE_VERSION  "0.0.4"
#define BOARD_HARDWARE_VERSION  "1.0.0"

#define TFT_CS                  15
#define TFT_DC                  5

#define TIME_ZONE               (2U)

#define TFT_BACKGRAUND_COLOR    (ILI9341_BLACK)
#define TFT_CLOCK_COLOR         (ILI9341_YELLOW)
#define TFT_CLOCK_Y             (30U)
#define TFT_CLOCK_SIZE          (5U)
#define TFT_DATE_COLOR          (ILI9341_GREEN)
#define TFT_DATE_Y              (TFT_CLOCK_Y + 55U)
#define TFT_DATE_SIZE           (2U)
#define TFT_WEEKDAYS_COLOR      (ILI9341_PURPLE)
#define TFT_WEEKENDS_COLOR      (ILI9341_RED)
#define TFT_WEEKDAY_Y           (TFT_DATE_Y + 30U)
#define TFT_WEEKDAY_SIZE        (3U)

#define JSON_MEMORY_BUFFER      (1024*2) // now much memory to give to the decoding of the json response
