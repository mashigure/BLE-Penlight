// LED Control for Penlight

#ifndef LEDBAR_H
#define LEDBAR_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "WaitCounter.h"


// LEDs control
class LEDBar
{
private:
    const int pin = 6;
    const int ledNum = 24;
    const int loopDelay = 50;

    int colorNo;
    int rainbowNo;
    unsigned int LedColors[8];

    WaitCounter *wait;
    Adafruit_NeoPixel *pixels;

public:
    LEDBar();
    ~LEDBar();
    void cyclicProcess(int no);

private:
    void SetColor(unsigned int c);
    unsigned int GetColor();
    unsigned int GetRainbowColor();
};


#endif // LEDBAR_H
