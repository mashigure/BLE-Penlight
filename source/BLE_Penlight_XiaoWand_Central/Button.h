// Buttons of Penlight

#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

extern bool xiaowand_check_release(void);

class Button
{
public:
    static const int MAX_BRIGHTNESS = 2;

private:
    int color_no;
    int brtns_lv;
    const int pin_brtns_btn = 5;

public:
    Button():
        color_no(0),
        brtns_lv(MAX_BRIGHTNESS)
    {
        pinMode(pin_brtns_btn, INPUT_PULLUP);
    }

    int getNo()
    {
        return color_no;
    }

    void setNo(int value)
    {
        color_no = value;
    }

    int getBrightness()
    {
        return brtns_lv;
    }

    void update()
    {
        updateColor();
        updateBrightness();
    }

private:
    void updateColor()
    {
        static int last_sw = HIGH;
        bool result = false;

        int sw = digitalRead(pin_brtns_btn);

        if ((last_sw == HIGH) && (sw == LOW))
        {
            color_no = (color_no + 1) % 10;
        }

        last_sw = sw;
    }

    void updateBrightness()
    {
        if (xiaowand_check_release())
        {
            brtns_lv = (brtns_lv + 1) % (MAX_BRIGHTNESS + 1);
        }
    }
};


#endif // BUTTON_H
