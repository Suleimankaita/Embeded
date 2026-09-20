#include <Arduino.h>
#include "FlashLight.h"

int  LED_PIN= 4;
int  LED_PINs=6;
int EXT_LED= 2;

void setup()
{
  setupFlash(EXT_LED, LED_PIN, LED_PINs);
}

void loop()
{

    flashLight(LED_PIN,EXT_LED,LED_PINs);
  
}
