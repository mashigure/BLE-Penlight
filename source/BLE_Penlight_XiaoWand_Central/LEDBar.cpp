// LED Control for Penlight

#include "LEDBar.h"
#include "Button.h"


LEDBar::LEDBar()
    :colorNo(0),
     rainbowNo(0)
{
    wait = new WaitCounter(loopDelay);
    pixels = new Adafruit_NeoPixel(ledNum, pin, NEO_GRB + NEO_KHZ800);
    pixels->begin();
}


LEDBar::~LEDBar()
{
    delete wait;
    delete pixels;
}


void LEDBar::cyclicProcess(int no, unsigned int brightness)
{
    if (no != colorNo)
    {
        colorNo = no;
        rainbowNo = 0;
    }

    SetColor(GetColor(), brightness);

    while (!wait->isCounted());
    wait->reset(loopDelay);
}


void LEDBar::on()
{
    for (int i=0; i<8; i++)
    {
        cyclicProcess(0, 0);
    }
}


void LEDBar::off()
{
    for (int i=0; i<8; i++)
    {
        cyclicProcess(-1, 0);
    }
}


void LEDBar::SetColor(unsigned int c, unsigned int brightness)
{
    for (int i=0; i<7; i++)
    {
        LedColors[7-i] = LedColors[6-i];
    }

    LedColors[0] = CalcBrightness(c, brightness);


    for (int i=0; i<8; i++)
    {
        pixels->setPixelColor(i   , LedColors[i]);
        pixels->setPixelColor(15-i, LedColors[i]);
        pixels->setPixelColor(i+16, LedColors[i]);
    }

    pixels->show();
}

unsigned int LEDBar::CalcBrightness(unsigned int c, unsigned int brightness)
{
    unsigned int r = (c & 0xFF0000) >> 16;
    unsigned int g = (c & 0x00FF00) >> 8;
    unsigned int b = (c & 0x0000FF);

    r = (r * (brightness + 1) / (Button::MAX_BRIGHTNESS + 1)) & 0xFF;
    g = (g * (brightness + 1) / (Button::MAX_BRIGHTNESS + 1)) & 0xFF;
    b = (b * (brightness + 1) / (Button::MAX_BRIGHTNESS + 1)) & 0xFF;

    return ((r << 16) + (g << 8) + b);
}


unsigned int LEDBar::GetColor()
{
    switch (colorNo)
    {
        case  0: return 0xFF2000;
        case  1: return 0xFF2747;
        case  2: return 0x00FF60;
        case  3: return 0xFF0000;
        case  4: return 0x0020FF;
        case  5: return 0xF0F0FF;
        case  6: return 0xFFFF00;
        case  7: return 0x4000FF;
        case  8: return 0xFF0060;
        case  9: return GetRainbowColor();
        default: return 0x000000;
    }
}


unsigned int LEDBar::GetRainbowColor()
{
    rainbowNo = (rainbowNo + 1) % 42;

    switch (rainbowNo)
    {
        case  0:
        case  1:
        case  2: return 0xFF0000;
        case  3: return 0xFF0F00;
        case  4: return 0xFF1F00;
        case  5: return 0xFF3F00;
        case  6: return 0xFF7F00;
        case  7:
        case  8:
        case  9: return 0xFFFF00;
        case 10: return 0x7FFF00;
        case 11: return 0x3FFF00;
        case 12: return 0x1FFF00;
        case 13: return 0x0FFF00;
        case 14:
        case 15:
        case 16: return 0x00FF00;
        case 17: return 0x00FF0F;
        case 18: return 0x00FF1F;
        case 19: return 0x00FF3F;
        case 20: return 0x00FF7F;
        case 21:
        case 22:
        case 23: return 0x00FFFF;
        case 24: return 0x007FFF;
        case 25: return 0x003FFF;
        case 26: return 0x001FFF;
        case 27: return 0x000FFF;
        case 28:
        case 29:
        case 30: return 0x0000FF;
        case 31: return 0x0F00FF;
        case 32: return 0x1F00FF;
        case 33: return 0x3F00FF;
        case 34: return 0x7F00FF;
        case 35:
        case 36:
        case 37: return 0xFF00FF;
        case 38: return 0xFF007F;
        case 39: return 0xFF003F;
        case 40: return 0xFF001F;
        case 41: return 0xFF000F;
        default: return 0x000000;
    }
}
