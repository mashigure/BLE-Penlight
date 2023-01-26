// Button of Penlight

#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include "WaitCounter.h"

class Button
{
private:
    WaitCounter *counter;
    int no;
    const int pin = 5;
    const int longPushTime = 5000;

public:
    Button()
    {
        counter = new WaitCounter(longPushTime);
        pinMode(pin, INPUT_PULLUP);
        no = 0;
    }

    int getNo()
    {
        return no;
    }

    void setNo(int value)
    {
        no = value;
    }

    bool isPowerON()
    {
        return (0 <= no);
    }

    void update()
    {
        static int last_sw = HIGH;

        int sw = digitalRead(pin);

        if ((last_sw == LOW) && (sw == HIGH) && !counter->isCounted())
        {
            no = (no + 1) % 10;
        }

        if ((last_sw == HIGH) && (sw == LOW))
        {
            counter->reset(longPushTime);
        }

        last_sw = sw;

        // Long press
        if ((sw == LOW) && counter->isCounted())
        {
            no = -1;
        }
    }
};


#endif // BUTTON_H
