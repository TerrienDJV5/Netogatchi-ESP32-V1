//By Terrien-DJV5
//Taskbar.h
//Date Created: jun 15 2023 3:27PM

#ifndef MY_TASKBAR_H
#define MY_TASKBAR_H
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
class Taskbar {
  private:
    Adafruit_SH1106 *display;
    byte *batteryPercentP;
    byte batteryPercent;
    bool *chargingFlagP;
    bool chargingFlag;
    byte *cableStatusP;
    byte cableStatus;
    
    
  public:
    Taskbar( Adafruit_SH1106 &displayL );
    void init();
    void syncPointer_BatteryPercent(byte batteryPercent);
    void syncPointer_ChargingFlag(bool chargingFlag);
    void syncPointer_CableStatus(byte cableStatus);
    
    void drawBatteryPowerIcon(unsigned int xPos, unsigned int yPos, byte batteryPercent = 0, bool chargingFlag = false, byte rotation = 0);
    void drawCableIcon(unsigned int xPos, unsigned int yPos, byte cableStatus = 0, byte rotation = 0);
    void drawTaskbar(int frame_location_offset, byte rotation = 1, bool displayFlag = false);
    
    
};
#endif
