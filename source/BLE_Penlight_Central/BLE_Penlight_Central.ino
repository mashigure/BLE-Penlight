// BLE Concert Pen Light (Central)

#include <ArduinoBLE.h>
#include "Button.h"
#include "LEDBar.h"

// Global variables
LEDBar led;
Button btn;
bool isScanning = false;
const char *UUID = "19B10000-E8F2-537E-4F6C-D104768A1214";


// initialising
void setup()
{
    BLE.begin();
}


// main loop
void loop()
{
    if (btn.isPowerON())
    {
        // remote controlling mode
        remoteControllingMode_loop();
    }

    // stand alone mode
    btn.update();
    led.cyclicProcess(btn.getNo());
}


// main loop for remote controlling mode
void remoteControllingMode_loop()
{
    int btn_no = btn.getNo();
    int led_no = btn_no;

    startScan();

    BLEDevice peripheral = BLE.available();

    if (!peripheral || (peripheral.localName() != "PenLight"))
    {
        return;
    }

    stopScan();

    if (!peripheral.connect())
    {
        return;
    }

    if (!peripheral.discoverAttributes())
    {
        peripheral.disconnect();
        return;
    }

    BLECharacteristic ledCh = peripheral.characteristic("19B10001-E8F2-537E-4F6C-D104768A1214");
    BLECharacteristic btnCh = peripheral.characteristic("19B10002-E8F2-537E-4F6C-D104768A1214");

    if (!ledCh || !btnCh || !ledCh.canWrite() || !btnCh.canSubscribe() || !btnCh.subscribe())
    {
        peripheral.disconnect();
        return;
    }

    ledCh.writeValue((byte)led_no);

    while (peripheral.connected())
    {
        // receive
        if (btnCh.valueUpdated())
        {
            byte value = 0;
            btnCh.readValue(value);
            led_no = (int)value;
            ledCh.writeValue((byte)led_no);
            btn.setNo(led_no);
        }

        btn.update();
        btn_no = btn.getNo();

        // Power OFF by long press
        if (!btn.isPowerON())
        {
            peripheral.disconnect();
            return;
        }

        // send
        if (led_no != btn_no)
        {
            led_no = btn_no;
            ledCh.writeValue((byte)led_no);
        }

        // control LEDs and wait
        led.cyclicProcess(led_no);
    }
}


void startScan()
{
    if (!isScanning)
    {
        BLE.scanForUuid(UUID);
        isScanning = true;
    }
}


void stopScan()
{
    if (isScanning)
    {
        BLE.stopScan();
        isScanning = false;
    }
}
