#line 1 "/home/hacktopubuntu/Documents/GitHub/Netogatchi-ESP32-V1/Software/tamagotchi_V0.0.0_for_Hardware_V1/Test_Adafruit_SH1106.h"
//By Terrien-DJV5
//Test_Adafruit_SH1106.h
//Date Created: may 17 2023 1:18PM



#ifndef TEST_ADAFRUIT_SH1106_H
#define TEST_ADAFRUIT_SH1106_H
#include <Arduino.h>



void setupTest(Adafruit_SH1106 &display);
void testdrawbitmap(Adafruit_SH1106 &display, const uint8_t *bitmap, uint8_t w, uint8_t h);
void testdrawchar(Adafruit_SH1106 &display);
void testdrawcircle(Adafruit_SH1106 &display);
void testfillrect(Adafruit_SH1106 &display);
void testdrawtriangle(Adafruit_SH1106 &display);
void testfilltriangle(Adafruit_SH1106 &display);
void testdrawroundrect(Adafruit_SH1106 &display);
void testfillroundrect(Adafruit_SH1106 &display);
void testdrawrect(Adafruit_SH1106 &display);
void testdrawline(Adafruit_SH1106 &display);
void testscrolltext(Adafruit_SH1106 &display);

#endif
