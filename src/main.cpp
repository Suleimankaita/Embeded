#include <Arduino.h>

#define LED_PIN 4
#define EXT_LED 2

void setup()
{
    pinMode(EXT_LED, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
}

void loop()
{

    digitalWrite(EXT_LED, HIGH);
    digitalWrite(LED_PIN, HIGH);
    delay(1000);

    digitalWrite(EXT_LED, LOW);
    digitalWrite(LED_PIN, LOW);
    delay(1000);
}
