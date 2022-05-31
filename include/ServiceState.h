#pragma once

enum State {
  MODE_WAIT_CONFIG,
  MODE_INITIAL_PERIPH,
  MODE_CONFIGURING,
  MODE_CONNECTING_NET,
  MODE_CONNECTING_CLOUD,
  MODE_RUNNING,
  MODE_OTA_UPGRADE,
  MODE_SWITCH_TO_STA,
  MODE_RESET_CONFIG,
  MODE_NO_WIFI,
  MODE_NO_CLOUD,
  MODE_IDLE,
  MODE_ERROR,
  MODE_MAX_VALUE
};

const char* StateStr[MODE_MAX_VALUE] = {
  "WAIT_CONFIG",
  "MODE_INITIAL_PERIPH",
  "CONFIGURING",
  "CONNECTING_NET",
  "CONNECTING_CLOUD",
  "RUNNING",
  "OTA_UPGRADE",
  "SWITCH_TO_STA",
  "RESET_CONFIG",
  "NO_WIFI",
  "NO_CLOUD",
  "MODE_IDLE",
  "ERROR"
};

namespace ServiceState
{
  volatile State state;

  State get()        { return state; }
  bool  is (State m) { return (state == m); }
  void  set(State m);
};