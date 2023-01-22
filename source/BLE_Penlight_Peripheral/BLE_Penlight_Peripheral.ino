// BLE Penlight (Peripheral)

#include <ArduinoBLE.h>
#include "Button.h"
#include "LEDBar.h"

// Global variables
// Arduino BLEのサンプルプログラムの設定値そのままやけどええのんか？
BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLEByteCharacteristic LedCh("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEByteCharacteristic BtnCh("19B10002-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);
LEDBar led;
Button btn;


// initialising
void setup()
{
    BLE.begin();
    BLE.setLocalName("PenLight");
    BLE.setAdvertisedService(ledService);

    ledService.addCharacteristic(LedCh);
    ledService.addCharacteristic(BtnCh);
    BLE.addService(ledService);

    LedCh.writeValue(0);
    BtnCh.writeValue(0);

    BLE.advertise();
}


// main loop
void loop()
{
    static int no = 0;

    if (btn.isPowerON())
    {
        // remote controlled mode
        remoteControlledMode_loop(no);
    }

    // stand alone mode
    btn.update();
    no = btn.getNo();
    led.cyclicProcess(no);
}


// main loop for remote controlled mode
void remoteControlledMode_loop(int no)
{
    BLEDevice central = BLE.central();

    while (central && central.connected())
    {
        int last_no = btn.getNo();
        btn.update();
        int updated_no = btn.getNo();

        // Power OFF by long press
        if (!btn.isPowerON())
        {
            central.disconnect();
            return;
        }

        // send
        if (last_no != updated_no)
        {
            BtnCh.writeValue((byte)updated_no);
        }

        // receive
        if (LedCh.written())
        {
            no = (int)LedCh.value();
            btn.setNo(no);
        }

        // control LEDs and wait
        led.cyclicProcess(no);
    }
}
