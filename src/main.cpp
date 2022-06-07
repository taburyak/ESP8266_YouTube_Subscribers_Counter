#include <Arduino.h>
#include "ProvisioningService.h"

void setup() 
{
  Serial.begin(115200);

  ServiceProvisioning.begin();
}

void loop() 
{
  ServiceProvisioning.run();
}