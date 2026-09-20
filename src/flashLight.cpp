#include "FlashLight.h"
#include <Arduino.h>

 void flashLight(int EXT_LED,int LED_PIN,int LED_PINs){
    digitalWrite(EXT_LED, HIGH);
    delay(100);
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PINs, HIGH);
    delay(1000);

    digitalWrite(EXT_LED, LOW);
        delay(100);
        
        digitalWrite(LED_PIN, LOW);
        delay(100);
    digitalWrite(LED_PINs, LOW);
    delay(1000);
 };

void setupFlash(int EXT_LED,int LED_PIN,int LED_PINs){
   pinMode(EXT_LED, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    pinMode(LED_PINs, OUTPUT);
};
 