//By Terrien-DJV5
//Taskbar.h
//Date Created: jun 15 2023 3:27PM

#ifndef MY_TASKBAR_H
#define MY_TASKBAR_H
#include <Arduino.h>


void drawBatteryPowerIcon(Adafruit_SH1106 &display, unsigned int xPos, unsigned int yPos, byte batteryPercent = 0, bool chargingFlag = false, byte rotation = 0);
void drawCableIcon(Adafruit_SH1106 &display, unsigned int xPos, unsigned int yPos, byte cableStatus = 0, byte rotation = 0);
void drawTaskbar(Adafruit_SH1106 &display, int frame_location_offset, byte rotation = 1, bool displayFlag = false);

#endif
