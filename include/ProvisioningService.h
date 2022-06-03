#pragma once

#include <Arduino.h>
#include "Setings.h"
#include "ServiceState.h"
#include "StateHandler.h"


#define DEBUG_PRINT Serial.println

inline void ServiceState::set(State m)
{
  if (state != m) 
  {
    DEBUG_PRINT(String(StateStr[state]) + " => " + StateStr[m]);
    state = m;
  }
}

class Provisioning {

public:
  void begin()
  {
    DEBUG_PRINT("");
    DEBUG_PRINT("*------------------------------------*");
    DEBUG_PRINT("* ESP8266 YouTube subscriber counter *");
    DEBUG_PRINT("*------------------------------------*");

    DEBUG_PRINT("");
    DEBUG_PRINT("Hardware v" + String(BOARD_HARDWARE_VERSION));
    DEBUG_PRINT("Firmware v" + String(BOARD_FIRMWARE_VERSION));

    // indicator_init();
    // button_init();
    // config_init();

    ServiceState::set(MODE_INITIAL_PERIPH);
    
    // if (configStore.flagConfig) {
    //   ServiceState::set(MODE_CONNECTING_NET);
    // } else {
    //   ServiceState::set(MODE_WAIT_CONFIG);
    // }
  }

  void run() 
  {
    switch (ServiceState::get()) 
    {
      case MODE_WAIT_CONFIG:
      case MODE_INITIAL_PERIPH:       enterInitialPeriph(); break;
      // case MODE_CONFIGURING:       enterConfigMode();    break;
      case MODE_CONNECTING_NET:       enterConnectNet();    break;
      case MODE_CONNECTING_CLOUD:     enterConnectCloud();  break;
      case MODE_RUNNING:              enterRun();           break;
      case MODE_DISPLAY:              enterDisplay();       break;
      case MODE_REFRESH_DATA:         enterRefreshData();   break;
      // case MODE_OTA_UPGRADE:       enterOTA();           break;
      // case MODE_SWITCH_TO_STA:     enterSwitchToSTA();   break;
      // case MODE_RESET_CONFIG:      enterResetConfig();   break;
      // case MODE_NO_WIFI:           enterNoWiFiMode();    break;
      // case MODE_NO_CLOUD:          enterNoCloudMode();   break;
      case MODE_IDLE:                 enterIdle();          break;
      default:                        enterError();         break;
    }
  }

};

Provisioning ServiceProvisioning;
