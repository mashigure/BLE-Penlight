// BLE Concert Pen Light using XIAO-WAND (Central)

#include <ArduinoBLE.h>
#include "Button.h"
#include "LEDBar.h"

#define XIAOWAND_MODULE_XIAO_BLE

// Global variables
// Arduino BLEのサンプルプログラムの設定値そのままやけどええのんか？
const char *LOCALNAME = "PenLight";
const char *LEDSERV = "19B10000-E8F2-537E-4F6C-D104768A1214";
const char *LEDCHAR = "19B10001-E8F2-537E-4F6C-D104768A1214";
const char *BTNCHAR = "19B10002-E8F2-537E-4F6C-D104768A1214";

BLEService ledService(LEDSERV);
BLEByteCharacteristic LedCh(LEDCHAR, BLERead | BLEWrite);
BLEByteCharacteristic BtnCh(BTNCHAR, BLERead | BLENotify);
LEDBar led;
Button btn;


// initialising
void setup()
{
    BLE.begin();
    xiaowand_power_begin();
    led.on();

    BLE.setLocalName(LOCALNAME);
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
    xiaowand_polling();
}


// ボタン長押しで起動した時に行う処理はここに書く
void xiaowand_startup()
{
}


// メインのloop()処理部分をここに書く
void xiaowand_loop()
{
    // remote controlled mode
    remoteControlledMode_loop();

    // stand alone mode
    btn.update();
    led.cyclicProcess(btn.getNo(), btn.getBrightness());
}


// 電源OFFする時に必要な処理はここに書く
void xiaowand_shutdown()
{
    led.off();
}


// main loop for remote controlled mode
void remoteControlledMode_loop()
{
    int btn_peripheral = btn.getNo();
    int btn_central = btn_peripheral;

    BLEDevice central = BLE.central();

    while (central && central.connected())
    {
        btn.update();
        btn_peripheral = btn.getNo();

        // Power OFF by long press
        if (xiaowand_check_shutdown())
        {
            central.disconnect();
            return;
        }

        // send
        if (btn_central != btn_peripheral)
        {
            BtnCh.writeValue((byte)btn_peripheral);
        }

        // receive
        if (LedCh.written())
        {
            btn_central = (int)LedCh.value();
            btn.setNo(btn_central);
        }

        // control LEDs and wait
        led.cyclicProcess(btn_central, btn.getBrightness());
    }
}
