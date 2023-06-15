//By Terrien-DJV5
//Taskbar.cpp
//Date Created: jun 15 2023 3:27PM


#include "Taskbar.h"
#define taskbarIconCount 16
#define showMissingIconIfEmpty false


static const unsigned char PROGMEM icon8x8_filled_bmp[] =
{
  B11111111,
  B11111111,
  B11111111,
  B11111111,
  B11111111,
  B11111111,
  B11111111,
  B11111111
};



static const unsigned char PROGMEM icon8x8_missing_icon_bmp[] =
{
  B00000111,
  B01111011,
  B11000101,
  B01100010,
  B01010010,
  B01001010,
  B01111110,
  B00000001,
};

static const unsigned char PROGMEM icon8x8_Battery_bmp[6][8] =
{
  {
    B00111100,
    B01000010,
    B01000010,
    B01000010,
    B01000010,
    B01000010,
    B01000010,
    B01111110
  },
  {
    B00000000,
    B00000000,
    B00011000,
    B00110000,
    B00111100,
    B00001100,
    B00011000,
    B00000000
  },
  {
    B00000000,
    B00000000,
    B00001000,
    B00010000,
    B00111100,
    B00001000,
    B00010000,
    B00000000
  },
  {
    B00111100,
    B01100110,
    B11001111,
    B10011101,
    B10111001,
    B11110011,
    B01100110,
    B00111100
  },
  {
    B00000000,
    B00100100,
    B00100100,
    B01111110,
    B01111110,
    B00111100,
    B00011000,
    B00011000
  },
  {
    B00100100,
    B01100100,
    B11100100,
    B00100100,
    B00100100,
    B00100111,
    B00100110,
    B00100100
  },
};


static const unsigned char PROGMEM iconLocationsRXY[4][16][2] = {
  {{0, 0}, {8, 0}, {16, 0}, {24, 0}, {32, 0}, {40, 0}, {48, 0}, {56, 0}, {64, 0}, {72, 0}, {80, 0}, {88, 0}, {96, 0}, {104, 0}, {112, 0}, {120, 0}},
  {{120, 0}, {120, 8}, {120, 16}, {120, 24}, {120, 32}, {120, 40}, {120, 48}, {120, 56}, {255, 255}, {255, 255}, {255, 255}, {255, 255}, {255, 255}, {255, 255}, {255, 255}, {255, 255}},
  {{0, 56}, {0, 56}, {0, 56}, {0, 56}, {0, 56}, {0, 56}, {0, 56}, {0, 56}, {0, 64}, {0, 72}, {0, 80}, {0, 88}, {0, 96}, {0, 104}, {0, 112}, {0, 120}},
  {{0, 0}, {0, 8}, {0, 16}, {0, 24}, {0, 32}, {0, 40}, {0, 48}, {0, 56}, {255, 255}, {255, 255}, {255, 255}, {255, 255}, {255, 255}, {255, 255}, {255, 255}, {255, 255}}
};



Taskbar::Taskbar( Adafruit_SH1106 &displayL ) {
  this->display = &displayL;
  init();
}

void Taskbar::init(){
  ;
}

void Taskbar::syncPointer_BatteryPercent(byte batteryPercent){
  this->batteryPercentP = &batteryPercent;
  this->batteryPercent = batteryPercent;
}
void Taskbar::syncPointer_ChargingFlag(bool chargingFlag){
  this->chargingFlagP = &chargingFlag;
  this->chargingFlag = chargingFlag;
}
void Taskbar::syncPointer_CableStatus(byte cableStatus){
  this->cableStatusP = &cableStatus;
  this->cableStatus = cableStatus;
}



//void Taskbar::drawBatteryPowerIcon(unsigned int xPos, unsigned int yPos, byte batteryPercent = 0, bool chargingFlag = false, byte rotation = 0) {
void Taskbar::drawBatteryPowerIcon(unsigned int xPos, unsigned int yPos, byte batteryPercent, bool chargingFlag, byte rotation) {
  //add rotation later
  //batteryPercent is out of 100
  byte batteryIconSelect = map(batteryPercent, 0, 101, 0, 7);


  if (batteryPercent == 0) { //if 0 the battery is disconnected
    display->drawBitmap(xPos, yPos, icon8x8_Battery_bmp[0], 8, 8, WHITE);//Empty Battery Symbol
    display->drawBitmap(xPos, yPos, icon8x8_Battery_bmp[3], 8, 8, WHITE);//circleWithACross
    return ;
  }

  switch (batteryIconSelect) {
    case (0):
      display->drawBitmap(xPos, yPos, icon8x8_Battery_bmp[0], 8, 8, WHITE);//Empty Battery Symbol
      if (chargingFlag == true) {
        display->drawBitmap(xPos, yPos, icon8x8_Battery_bmp[2], 8, 8, WHITE);//Thin Ligthing Symbol
      }
      break;
    case (6):
      display->drawBitmap(xPos, yPos, icon8x8_Battery_bmp[0], 8, 8, WHITE);//Empty Battery Symbol
      display->fillRect(xPos + 1, yPos + 1, 6, 6, WHITE);
      if (chargingFlag == true) {
        display->drawBitmap(xPos, yPos, icon8x8_Battery_bmp[1], 8, 8, BLACK);//Thick Ligthing Symbol
      }
      break;
    default:
      display->drawBitmap(xPos, yPos, icon8x8_Battery_bmp[0], 8, 8, WHITE);//Empty Battery Symbol
      display->fillRect(xPos + 1, yPos + 1 + (6 - batteryIconSelect), 6, batteryIconSelect, WHITE);
      display->drawBitmap(xPos, yPos, icon8x8_Battery_bmp[1], 8, 8, BLACK);//Thick Ligthing Symbol
      if (chargingFlag == true) {
        display->drawBitmap(xPos, yPos, icon8x8_Battery_bmp[2], 8, 8 - batteryIconSelect - 1, WHITE); //Thin Ligthing Symbol
      }
      break;
  }
}

//void Taskbar::drawCableIcon(unsigned int xPos, unsigned int yPos, byte cableStatus = 0, byte rotation = 0) {
void Taskbar::drawCableIcon(unsigned int xPos, unsigned int yPos, byte cableStatus, byte rotation) {
  //add rotation later
  display->drawBitmap(xPos, yPos, icon8x8_Battery_bmp[5], 8, 8, WHITE);
  display->fillRect(xPos, yPos, 4, 8, BLACK);
  display->drawBitmap(xPos, yPos, icon8x8_Battery_bmp[4], 4, 8, WHITE);


}







//void Taskbar::drawTaskbar(int frame_location_offset, byte rotation = 1, bool displayFlag = false) {
void Taskbar::drawTaskbar(int frame_location_offset, byte rotation, bool displayFlag) {
  static byte lastRotation;
  static int iconLocationsXY[taskbarIconCount][2];
  static byte visibleIconCont;
  if (rotation != lastRotation) {
    //iconLocations
    for (uint8_t index = 0; index < taskbarIconCount; index++) {
      switch (rotation) {
        case (0)://Top
          iconLocationsXY[ index ][0] = 0 + 8 * index;
          iconLocationsXY[ index ][1] = 0;//0-frame_location_offset
          visibleIconCont = 16;
          break;
        case (1): //Right
          iconLocationsXY[ index ][0] = 120;//120+frame_location_offset
          iconLocationsXY[ index ][1] = 0 + 8 * index;
          visibleIconCont = 8;
          break;
        case (2): //Bottom
          iconLocationsXY[ index ][0] = 0 + 8 * index;
          iconLocationsXY[ index ][1] = 56;//56+frame_location_offset
          visibleIconCont = 16;
          break;
        case (3): //Left
          iconLocationsXY[ index ][0] = 0;//0-frame_location_offset
          iconLocationsXY[ index ][1] = 0 + 8 * index;
          visibleIconCont = 8;
          break;
        default:
          //taskbar is hidden
          visibleIconCont = 0;
          return ;
          break;
      }
    }
    //iconLocationsXY[16][2] = iconLocationsRXY[rotation][16][2];
    lastRotation = rotation;
  } else {

  }
  int iconXPOS;
  int iconYPOS;
  byte batteryIconIndex = 0;
  byte cableIconIndex = 0;
  for (uint8_t iconIndex = 0; iconIndex < taskbarIconCount; iconIndex++) {
    iconXPOS = iconLocationsRXY[rotation][ iconIndex ][0];
    iconYPOS = iconLocationsRXY[rotation][ iconIndex ][1];
    switch (rotation) {
      case (0)://Top
        iconYPOS = iconYPOS - frame_location_offset;
        break;
      case (1): //Right
        iconXPOS = iconXPOS + frame_location_offset;
        break;
      case (2): //Bottom
        iconYPOS = iconYPOS + frame_location_offset;
        break;
      case (3): //Left
        iconXPOS = iconXPOS - frame_location_offset;
        break;
      default:
        //taskbar is hidden
        break;
    }
    iconLocationsXY[ iconIndex ][0] = iconXPOS;
    iconLocationsXY[ iconIndex ][1] = iconYPOS;
  }

  switch (rotation) {
    case (0)://Top
      //batteryIcon
      batteryIconIndex = 7;
      //cableIcon
      cableIconIndex = 6;
      display->fillRect(0 - 1, iconLocationsXY[0][1] - 1, 8 * visibleIconCont + 2, 10, BLACK);
      display->drawRect(0 - 1, iconLocationsXY[0][1] - 1, 8 * visibleIconCont + 2, 10, WHITE);
      display->drawLine(0, iconLocationsXY[0][1] + 8, 8 * visibleIconCont, iconLocationsXY[0][1] + 8, WHITE);

      break;
    case (1)://Right
      //batteryIcon
      batteryIconIndex = 0;
      //cableIcon
      cableIconIndex = 1;
      display->fillRect(iconLocationsXY[0][0] - 1, 0 - 1, 10, 8 * visibleIconCont + 2, BLACK);
      display->drawRect(iconLocationsXY[0][0] - 1, 0 - 1, 10, 8 * visibleIconCont + 2, WHITE);
      break;
    case (2)://Bottom
      //batteryIcon
      batteryIconIndex = 7;
      //cableIcon
      cableIconIndex = 6;
      display->fillRect(0 - 1, iconLocationsXY[0][1] - 1, 8 * visibleIconCont + 2, 10, BLACK);
      display->drawRect(0 - 1, iconLocationsXY[0][1] - 1, 8 * visibleIconCont + 2, 10, WHITE);
      break;
    case (3)://Left
      //batteryIcon
      batteryIconIndex = 0;
      //cableIcon
      cableIconIndex = 1;
      display->fillRect(iconLocationsXY[0][0] - 1, 0 - 1, 10, 8 * visibleIconCont + 2, BLACK);
      display->drawRect(iconLocationsXY[0][0] - 1, 0 - 1, 10, 8 * visibleIconCont + 2, WHITE);
      break;
    default:
      //taskbar is hidden
      return ;
      break;
  }

  for (uint8_t iconIndex = 0; iconIndex < taskbarIconCount; iconIndex++) {
    iconXPOS = iconLocationsXY[ iconIndex ][0];
    iconYPOS = iconLocationsXY[ iconIndex ][1];

    if (batteryIconIndex == iconIndex) {
      drawBatteryPowerIcon(iconXPOS, iconYPOS, batteryPercent, chargingFlag);
      //display->setTextSize(1);
      //display->setTextColor(WHITE);
      //display->print("batteryPercent:"); display->println(batteryPercent);
      continue;
    }
    if (cableIconIndex == iconIndex) {
      drawCableIcon(iconXPOS, iconYPOS, cableStatus);
      continue;
    }
    if (showMissingIconIfEmpty == true) {
      display->drawBitmap(iconXPOS, iconYPOS, icon8x8_missing_icon_bmp, 8, 8, WHITE);
      if (displayFlag) {
        display->display();
      };
    }
  }

  if (displayFlag) {
    display->display();
  };
}
