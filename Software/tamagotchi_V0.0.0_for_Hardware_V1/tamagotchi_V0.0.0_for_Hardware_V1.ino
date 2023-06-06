//TeegyBigz and TerrienDJV5
//tamagotchi_V0.0.0 for Hardware V1
//ESP-Wroom-32

//DOIT ESP32 DEVKIT V1
//https://lastminuteengineers.com/esp32-pinout-reference/

//https://circuits4you.com/2018/12/31/esp32-hardware-serial2-example/



//https://github.com/nhatuan84/esp32-sh1106-oled/blob/master/Adafruit_SH1106.cpp

//https://linuxhint.com/remove-arduino-library/#b2

//https://forum.arduino.cc/t/how-to-pass-an-object-or-reference-to-a-function/340185/2

//https://randomnerdtutorials.com/esp32-flash-memory/
//https://forum.arduino.cc/t/solved-getting-array-size/506976

//https://rishabhdevyadav.medium.com/create-your-own-arduino-library-h-and-cpp-files-62ab456453e0

//https://www.tutorialspoint.com/c_standard_library/string_h.htm
//https://www.tutorialspoint.com/cprogramming/c_pointers.htm

//https://linuxhint.com/variable-scope-arduino/

//
//https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/
//

/* Making ESP32 have filesystem
   https://github.com/me-no-dev/arduino-esp32fs-plugin/releases/
   https://randomnerdtutorials.com/install-esp32-filesystem-uploader-arduino-ide/
*/


//images
//used for converting images to monochrome BMPs
//https://image.online-convert.com/convert-to-bmp
//https://javl.github.io/image2cpp/


//helpful:
//https://roboticsbackend.com/arduino-object-oriented-programming-oop/
//https://dronebotworkshop.com/shift-registers/
//https://forum.arduino.cc/t/static-array-of-strings/411408/6

//https://github.com/arkhipenko/Dictionary

//https://cplusplus.com/reference/cstdlib/itoa/
//https://www.badprog.com/c-type-what-are-uint8-t-uint16-t-uint32-t-and-uint64-t

//https://docs.arduino.cc/learn/programming/memory-guide

//https://www.tutorialspoint.com/structs-in-arduino-program

//https://randomnerdtutorials.com/esp32-dual-core-arduino-ide/
//https://savjee.be/videos/programming-esp32-with-arduino/using-rtc-memory/


//Check this out?
//https://github.com/olikraus/u8g2


//Define Simple Functions
#define ARRAY_LEN(array) ((sizeof(array))/(sizeof(array[0])))



/*
   Dictionary
   https://github.com/arkhipenko/Dictionary
*/

//#define _DICT_CRC_ 16
#define _DICT_CRC_ 32
//#define _DICT_CRC_ 64

#define _DICT_PACK_STRUCTURES
//#define _DICT_COMPRESS_SHOCO
//#define _DICT_COMPRESS_SMAZ
#define _DICT_USE_PSRAM  // for ESP32 only


#ifdef _DICT_USE_PSRAM
#define KEYS  10000
#define TESTS 10000
#else
#define KEYS  2000
#define TESTS 10000
#endif

#define _DICT_KEYLEN 64
#define _DICT_VALLEN 256

//#define _DEBUG_
//#define _TEST_

#include <Dictionary.h>


/*
   Dictionary
*/

//Feature Enables (compiler Addons)
#define FeatureEnable_BT_Serial false
#define FeatureEnable_WiFi_Serial true //Option not implemented (enabled or disabled)
#define FeatureEnable_UART_Serial true //Option not implemented (enabled or disabled)

#define FeatureEnable_SH1106_Display true //Option not Fully implemented (enabled or disabled)
#define FeatureEnable_PiSo_Control true //Option not implemented (enabled or disabled)

#define FeatureEnable_IR_RXTX_Control false



//storageDevice modes
#define STORAGE_SELECT_SD 0
#define STORAGE_SELECT_SPIFFS 1
#define STORAGE_SELECT_SDFAT 2
#define STORAGE_SELECT_FFAT 3

#define STORAGE_INCLUDE_SD false
#define STORAGE_INCLUDE_SPIFFS true
#define STORAGE_INCLUDE_SDFAT false
#define STORAGE_INCLUDE_FFAT false

#define STORAGE_DEVICE_DEFAULT 1







//Library startups
#include "Arduino.h"

#include "FS.h"
#include "SPIFFS.h"
//https://www.tutorialspoint.com/esp32_for_iot/esp32_for_iot_spiffs_storage.htm


//Json Support
//<ArduinoJson.h> By Benoit BLANCHON
#include <ArduinoJson.h> //https://arduinojson.org/



//WiFi Support
#include "WiFi.h"
#include <esp_wifi.h>
//https://www.mischianti.org/2021/03/06/esp32-practical-power-saving-manage-wifi-and-cpu-1/


#include "WiFiType.h"

//StaticJsonDocument<512> saved_wifi_json;


int wifi_status = WL_IDLE_STATUS;


//https://www.megunolink.com/articles/wireless/talk-esp32-over-wifi/
WiFiServer SerialWiFiserver(23);

// Initialize the client library
WiFiClient SerialWiFiclient;




//other Stuff Support

#include "driver/adc.h"
#include "driver/dac.h"//https://docs.espressif.com/projects/esp-idf/en/v4.3/esp32s2/api-reference/peripherals/dac.html

#include <Preferences.h> //https://espressif-docs.readthedocs-hosted.com/projects/arduino-esp32/en/latest/api/preferences.html
Preferences preferences;



#include <SPI.h>
#include <Wire.h>



#if (FeatureEnable_SH1106_Display==true)
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>



#include "BMIMGmanipulate.h"
BMIMGmanipulate bitmapIMGmanipulate0(false);


//#define _TEST_ADAFRUIT_SH1106
#ifdef _TEST_ADAFRUIT_SH1106
#include "Test_Adafruit_SH1106.h"
#endif

#endif



#if (FeatureEnable_PiSo_Control==true)
#include "ButtonPISO.h"

#endif


#if (FeatureEnable_BT_Serial==true)
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error("Bluetooth is not enabled! Please run `make menuconfig` to and enable it")
#endif
BluetoothSerial SerialBT;
#endif






#if (FeatureEnable_IR_RXTX_Control==true)
/*
   Specify which protocol(s) should be used for decoding.
   If no protocol is defined, all protocols (except Bang&Olufsen) are active.
   This must be done before the #include <IRremote.hpp>
*/
//#define DECODE_DENON        // Includes Sharp
//#define DECODE_JVC
//#define DECODE_KASEIKYO
#define DECODE_PANASONIC    // alias for DECODE_KASEIKYO
//#define DECODE_LG
//#define DECODE_NEC          // Includes Apple and Onkyo
//#define DECODE_SAMSUNG
//#define DECODE_SONY
//#define DECODE_RC5
//#define DECODE_RC6

//#define DECODE_BOSEWAVE
//#define DECODE_LEGO_PF
//#define DECODE_MAGIQUEST
//#define DECODE_WHYNTER
//#define DECODE_FAST

//#define DECODE_DISTANCE_WIDTH // Universal decoder for pulse distance width protocols
//#define DECODE_HASH         // special decoder for all protocols

//#define DECODE_BEO          // This protocol must always be enabled manually, i.e. it is NOT enabled if no protocol is defined. It prevents decoding of SONY!

//#define DEBUG               // Activate this for lots of lovely debug output from the decoders.

//#define RAW_BUFFER_LENGTH  180  // Default is 112 if DECODE_MAGIQUEST is enabled, otherwise 100.

/*
   This include defines the actual pin number for pins like IR_RECEIVE_PIN, IR_SEND_PIN for many different boards and architectures
*/
//#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp> // include the library
//https://github.com/Arduino-IRremote/Arduino-IRremote

#if !defined(RAW_BUFFER_LENGTH)
#  if RAMEND <= 0x4FF || RAMSIZE < 0x4FF
#define RAW_BUFFER_LENGTH  120
#  elif RAMEND <= 0xAFF || RAMSIZE < 0xAFF // 0xAFF for LEONARDO
#define RAW_BUFFER_LENGTH  400 // 600 is too much here, because we have additional uint8_t rawCode[RAW_BUFFER_LENGTH];
#  else
#define RAW_BUFFER_LENGTH  750
#  endif
#endif

#define DISABLE_CODE_FOR_RECEIVER // Disables restarting receiver after each send. Saves 450 bytes program memory and 269 bytes RAM if receiving functions are not used.
//#define SEND_PWM_BY_TIMER         // Disable carrier PWM generation in software and use (restricted) hardware PWM.
//#define USE_NO_SEND_PWM           // Use no carrier PWM, just simulate an active low receiver signal. Overrides SEND_PWM_BY_TIMER definition
#endif


/*
  define pin Input Variables
*/

//IR Pins
#define IR_TX_Pin 27 //GPIO 27
#define IR_SEND_PIN 27
#define IR_RX_Pin 39 //GPIO 39



//MIC Pin
#define MIC_Pin 34 //GPIO 34


//Buzzer Boi Pin
#define buzzerPin 25//GPIO 25 //DAC1

//BatterySensePin
#define batteryVoltageDivider 36//GPIO 36 //ADC1_CH0



// Define Connections to 74HC165
// PL pin 1
#define BUTTONPISO_PL 32//GPIO 32 //PISO Load
// Q7 pin 7
#define BUTTONPISO_DataIn 35//GPIO 35 //PISO Data
// CP pin 2
#define BUTTONPISO_Clk 33//GPIO 33 //PISO Clock


//I2C Display
#define OLED_SDA 21 //I2C SDA
#define OLED_SCL 22 //I2C SCL

//UART
//U0UXD is generally used to communicate with the ESP32 for programming and during reset/boot.
#define UART0_TX_Pin 1//Used for Programing Board
#define UART0_RX_Pin 3//Used for Programing Board

//U1UXD is unused and can be used for your projects. Some boards use this port for SPI Flash access though!
#define UART1_TX_Pin 10
#define UART1_RX_Pin 9

//U2UXD is unused and can be used for your projects.
#define UART2_TX_Pin 17
#define UART2_RX_Pin 16

//SPI Bus
#define SPI_MoSi_Pin 23
#define SPI_MiSo_Pin 19
#define SPI_SCK_Pin 18
#define SPI_SS_Pin 5



//Devices Setup
#if (FeatureEnable_PiSo_Control==true)
ButtonPISO buttonpiso1(BUTTONPISO_DataIn, BUTTONPISO_Clk, BUTTONPISO_PL);
#endif


Adafruit_SH1106 display(OLED_SDA, OLED_SCL);


//http://electronics4dogs.blogspot.com/2011/01/arduino-predefined-constants.html
#if defined(ESP32)
//#include <HardwareSerial.h>
//HardwareSerial ExtraSerial0(0);//ESP32Serial0
//HardwareSerial ExtraSerial1(1);//ESP32Serial1
//HardwareSerial ExtraSerial2(2);//ESP32Serial2

#endif




#define ENABLE_RSSI true

#include "LoRa_E220.h"
//https://github.com/xreef/EByte_LoRa_E220_Series_Library


//LoRa_E220 e220ttl(&ExtraSerial2);  // e22 TX e22 RX
LoRa_E220 e220ttl(&Serial2);  // e22 TX e22 RX




//Devices Setup Complete

/*
  #include <pgmspace.h>
  #define buttonNameBufferSize 16
  const char buttonName_0[] PROGMEM = "UP";
  const char buttonName_1[] PROGMEM = "Start";
  const char buttonName_2[] PROGMEM = "Select";
  const char buttonName_3[] PROGMEM = "A";
  const char buttonName_4[] PROGMEM = "LEFT";
  const char buttonName_5[] PROGMEM = "DOWN";
  const char buttonName_6[] PROGMEM = "RIGHT";
  const char buttonName_7[] PROGMEM = "B";

  // Then set up a table to refer to your strings.
  const char* const button_Names_Byjunction[] PROGMEM = {buttonName_0, buttonName_1, buttonName_2, buttonName_3, buttonName_4, buttonName_5, buttonName_6, buttonName_7};

  char buttonNameBuffer[buttonNameBufferSize];    // make sure this is large enough for the largest string it must hold
  //*/

#define buttonPISOcount 8

//Byjunction
char buttonNameString[buttonPISOcount][7] =
{
  "UP",
  "Start",
  "Select",
  "A",
  "LEFT",
  "DOWN",
  "RIGHT",
  "B"
};

char* returnButtonName(byte buttonID, byte opMode = 1) {
  /* Using the string table in program memory requires the use of special functions to retrieve the data.
       The strcpy_P function copies a string from program space to a string in RAM ("buffer").
       Make sure your receiving string in RAM  is large enough to hold whatever
       you are retrieving from program space. */
  /*
    if (opMode == 0) {
    strcpy_P(buttonNameBuffer, (char*)pgm_read_word(&(button_Names_Byjunction[ buttonID ]))); // Necessary casts and dereferencing, just copy.
    return buttonNameBuffer;
    }
  */
  if (opMode == 1) {
    return buttonNameString[buttonID];
  }
}
unsigned int returnButtonID(char* buttonName) {
  for (unsigned int i = 0; i < ARRAY_LEN(buttonNameString); i++) {
    if (buttonNameString[i] == buttonName) {
      return i;
    }
  }

}



#define maxDebugCommandScan 1024

#define maxDebugCommandLength 256
//https://www.arduino.cc/reference/en/language/variables/utilities/progmem/
#include <pgmspace.h>
const char debugCommand_0[] PROGMEM = "";
const char debugCommand_1[] PROGMEM = "";
const char debugCommand_2[] PROGMEM = "";
const char debugCommand_3[] PROGMEM = "";
const char debugCommand_4[] PROGMEM = "";
const char debugCommand_5[] PROGMEM = "";
const char debugCommand_6[] PROGMEM = "";
const char debugCommand_7[] PROGMEM = "";
const char debugCommand_8[] PROGMEM = "";
const char debugCommand_9[] PROGMEM = "";
const char debugCommand_10[] PROGMEM = "";
const char debugCommand_11[] PROGMEM = "";
const char debugCommand_12[] PROGMEM = "";
const char debugCommand_13[] PROGMEM = "";
const char debugCommand_14[] PROGMEM = "";
const char debugCommand_15[] PROGMEM = "";
const char debugCommand_16[] PROGMEM = "";
const char debugCommand_17[] PROGMEM = "";
const char debugCommand_18[] PROGMEM = "";
const char debugCommand_19[] PROGMEM = "";
const char debugCommand_20[] PROGMEM = "";
const char debugCommand_21[] PROGMEM = "";
const char debugCommand_22[] PROGMEM = "";
const char debugCommand_23[] PROGMEM = "";
const char debugCommand_24[] PROGMEM = "";
const char debugCommand_25[] PROGMEM = "";
const char debugCommand_26[] PROGMEM = "";
const char debugCommand_27[] PROGMEM = "";
const char debugCommand_28[] PROGMEM = "";
const char debugCommand_29[] PROGMEM = "";
const char debugCommand_30[] PROGMEM = "";
const char debugCommand_31[] PROGMEM = "";

// Then set up a table to refer to your strings.
const char* const debugCommand_List[] PROGMEM = {debugCommand_0, debugCommand_1, debugCommand_2, debugCommand_3, debugCommand_4, debugCommand_5, debugCommand_6, debugCommand_7, debugCommand_8, debugCommand_9, debugCommand_10, debugCommand_11, debugCommand_12, debugCommand_13, debugCommand_14, debugCommand_15, debugCommand_16, debugCommand_17, debugCommand_18, debugCommand_19, debugCommand_20, debugCommand_21, debugCommand_22, debugCommand_23, debugCommand_24, debugCommand_25, debugCommand_26, debugCommand_27, debugCommand_28, debugCommand_29, debugCommand_30, debugCommand_31};

char debugCommandBuffer[maxDebugCommandLength];    // make sure this is large enough for the largest string it must hold







#if (FeatureEnable_SH1106_Display==true)
#if (SH1106_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SH1106.h!");
#endif
#endif



//Debug Based
byte showDebugMenu = 0;//0 = no debug menu, 1= first debug menu, etc
bool disableButtonPISO_update = false;

//System Variables
unsigned int currentMenuID = 0;
unsigned int previousMenuID = 0;
unsigned long frameCountVariable;//This variable stores the number of frames that have been displayed since the program started.
static unsigned char* batteryIconBigImage; //batteryBig_bitmapimg.dat
static unsigned int batteryIconBigWidth;
static unsigned int batteryIconBigHeight;
int cursorXPos = 0;
int cursorYPos = 0;
long int frameStartTime;// micros()
long int frameEndTime;// micros()
double system_Frame_FPS;


//selection menu //currentMenuID==1
static int sm_selectedID = 0;
static char smOptionName_List[][32 + 1] = {
  "Main Menu",
  "Selection Menu",
  "LoRa Menu Unfinished",
  "Wifi Menu Unfinished",
  "BT Menu Unfinished",
  "IR Menu Unfinished",
  "Sensor Menu Unfinished",
  "Mic/Biz Menu Unfinished",
  "SPIFFS File System Unfinished",
  "SD File System Menu Unfinished",
  "Op10",
  "Op11",
  "Op12",
  "Op13",
  "Op14",
  "Op15",
};


//Wifi Menu //currentMenuID==3
static unsigned long lastScanTimer;


//SPIFFs menu //currentMenuID==8



//character variables
const unsigned int characterPixelWidth = 32;//4 bytes
const unsigned int characterPixelHeight = 56;
const unsigned int characterFrameByteSize = ((characterPixelWidth % 8 + characterPixelWidth) >> 3) * characterPixelHeight;
const unsigned int characterBufferFrameCount = 32;
static unsigned char visableCharacterBuffer[ characterBufferFrameCount ][ characterFrameByteSize ];




//battery Variabls
static unsigned int batteryXpos = 99;
static unsigned int batteryYpos = 1;
static volatile byte batteryPercent = 100;//out of 100
static volatile double batteryVoltage = 0;
static volatile bool chargingFlag = false;

//cable Icon Variables
byte cableStatus = 0;

//user Settings
byte taskbarRotation = 1;//1
int frame_location_offset = 16;





/*

   Sprites Sheet and Tiles Sheet

*/


static const unsigned char PROGMEM rotateImageTestIcon[] =
{
  B11111111, B11111111, B00000000, B00000000,
  B10000000, B00000001, B00000000, B00000000,
  B10000000, B00000001, B00000000, B00000000,
  B10000000, B00000001, B00000000, B00000000,
  B10000000, B00000001, B00000000, B00000000,
  B10000000, B00000001, B00000000, B00000000,
  B10000000, B00000001, B00000000, B00000000,
  B10000000, B00000001, B00000000, B00000000,
  B10000000, B11111111, B00000000, B00000000,
  B10000000, B10000001, B00000000, B00000000,
  B10000000, B10000001, B00000000, B00000000,
  B10000000, B10000001, B00000000, B00000000,
  B10000000, B10000001, B00000000, B00000000,
  B10000000, B10000001, B00000000, B00000000,
  B10000000, B10000001, B00000000, B00000000,
  B11111111, B11111111, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000001,
  B00000000, B00000000, B00000000, B00000001,
  B00000000, B00000000, B00000000, B00000001,
  B00000000, B00000000, B00000000, B00000001,
  B00000000, B00000000, B00000000, B00000001,
  B00000000, B00000000, B00000000, B00000001,
  B00000000, B00000000, B00000000, B00000001,
  B00000000, B00000000, B00000000, B00000001,
  B00000000, B00000000, B00000000, B00000001,
  B00000000, B00000000, B00000000, B11111111,
  B00000000, B00000000, B00000000, B11111111,
  B00000000, B00000000, B00000000, B11111111,
  B00000000, B00000000, B00000000, B11111111,
  B00000000, B00000000, B00000000, B11111111,
  B00000000, B00000000, B00000000, B11111111,
  B00000000, B00000000, B11111111, B11111111
};



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


void drawBatteryPowerIcon(Adafruit_SH1106 &currentDisplay, unsigned int xPos, unsigned int yPos, byte batteryPercent = 0, bool chargingFlag = false, byte rotation = 0) {
  //add rotation later
  //batteryPercent is out of 100
  byte batteryIconSelect = map(batteryPercent, 0, 101, 0, 7);


  if (batteryPercent == 0) { //if 0 the battery is disconnected
    currentDisplay.drawBitmap(xPos, yPos, icon8x8_Battery_bmp[0], 8, 8, WHITE);//Empty Battery Symbol
    currentDisplay.drawBitmap(xPos, yPos, icon8x8_Battery_bmp[3], 8, 8, WHITE);//circleWithACross
    return ;
  }

  switch (batteryIconSelect) {
    case (0):
      currentDisplay.drawBitmap(xPos, yPos, icon8x8_Battery_bmp[0], 8, 8, WHITE);//Empty Battery Symbol
      if (chargingFlag == true) {
        currentDisplay.drawBitmap(xPos, yPos, icon8x8_Battery_bmp[2], 8, 8, WHITE);//Thin Ligthing Symbol
      }
      break;
    case (6):
      currentDisplay.drawBitmap(xPos, yPos, icon8x8_Battery_bmp[0], 8, 8, WHITE);//Empty Battery Symbol
      currentDisplay.fillRect(xPos + 1, yPos + 1, 6, 6, WHITE);
      if (chargingFlag == true) {
        currentDisplay.drawBitmap(xPos, yPos, icon8x8_Battery_bmp[1], 8, 8, BLACK);//Thick Ligthing Symbol
      }
      break;
    default:
      currentDisplay.drawBitmap(xPos, yPos, icon8x8_Battery_bmp[0], 8, 8, WHITE);//Empty Battery Symbol
      currentDisplay.fillRect(xPos + 1, yPos + 1 + (6 - batteryIconSelect), 6, batteryIconSelect, WHITE);
      currentDisplay.drawBitmap(xPos, yPos, icon8x8_Battery_bmp[1], 8, 8, BLACK);//Thick Ligthing Symbol
      if (chargingFlag == true) {
        currentDisplay.drawBitmap(xPos, yPos, icon8x8_Battery_bmp[2], 8, 8 - batteryIconSelect - 1, WHITE); //Thin Ligthing Symbol
      }
      break;
  }
}

void drawCableIcon(Adafruit_SH1106 &display, unsigned int xPos, unsigned int yPos, byte cableStatus = 0, byte rotation = 0) {
  //add rotation later
  display.drawBitmap(xPos, yPos, icon8x8_Battery_bmp[5], 8, 8, WHITE);
  display.fillRect(xPos, yPos, 4, 8, BLACK);
  display.drawBitmap(xPos, yPos, icon8x8_Battery_bmp[4], 4, 8, WHITE);


}





static const unsigned char PROGMEM iconLocationsRXY[4][16][2] = {
  {{0, 0}, {8, 0}, {16, 0}, {24, 0}, {32, 0}, {40, 0}, {48, 0}, {56, 0}, {64, 0}, {72, 0}, {80, 0}, {88, 0}, {96, 0}, {104, 0}, {112, 0}, {120, 0}},
  {{120, 0}, {120, 8}, {120, 16}, {120, 24}, {120, 32}, {120, 40}, {120, 48}, {120, 56}, {255, 255}, {255, 255}, {255, 255}, {255, 255}, {255, 255}, {255, 255}, {255, 255}, {255, 255}},
  {{0, 56}, {0, 56}, {0, 56}, {0, 56}, {0, 56}, {0, 56}, {0, 56}, {0, 56}, {0, 64}, {0, 72}, {0, 80}, {0, 88}, {0, 96}, {0, 104}, {0, 112}, {0, 120}},
  {{0, 0}, {0, 8}, {0, 16}, {0, 24}, {0, 32}, {0, 40}, {0, 48}, {0, 56}, {255, 255}, {255, 255}, {255, 255}, {255, 255}, {255, 255}, {255, 255}, {255, 255}, {255, 255}}
};


#define taskbarIconCount 16
#define showMissingIconIfEmpty false
void drawTaskbar(Adafruit_SH1106 &display, int frame_location_offset, byte rotation = 1, bool displayFlag = false) {
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
      display.fillRect(0 - 1, iconLocationsXY[0][1] - 1, 8 * visibleIconCont + 2, 10, BLACK);
      display.drawRect(0 - 1, iconLocationsXY[0][1] - 1, 8 * visibleIconCont + 2, 10, WHITE);
      display.drawLine(0, iconLocationsXY[0][1] + 8, 8 * visibleIconCont, iconLocationsXY[0][1] + 8, WHITE);

      break;
    case (1)://Right
      //batteryIcon
      batteryIconIndex = 0;
      //cableIcon
      cableIconIndex = 1;
      display.fillRect(iconLocationsXY[0][0] - 1, 0 - 1, 10, 8 * visibleIconCont + 2, BLACK);
      display.drawRect(iconLocationsXY[0][0] - 1, 0 - 1, 10, 8 * visibleIconCont + 2, WHITE);
      break;
    case (2)://Bottom
      //batteryIcon
      batteryIconIndex = 7;
      //cableIcon
      cableIconIndex = 6;
      display.fillRect(0 - 1, iconLocationsXY[0][1] - 1, 8 * visibleIconCont + 2, 10, BLACK);
      display.drawRect(0 - 1, iconLocationsXY[0][1] - 1, 8 * visibleIconCont + 2, 10, WHITE);
      break;
    case (3)://Left
      //batteryIcon
      batteryIconIndex = 0;
      //cableIcon
      cableIconIndex = 1;
      display.fillRect(iconLocationsXY[0][0] - 1, 0 - 1, 10, 8 * visibleIconCont + 2, BLACK);
      display.drawRect(iconLocationsXY[0][0] - 1, 0 - 1, 10, 8 * visibleIconCont + 2, WHITE);
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
      drawBatteryPowerIcon(display, iconXPOS, iconYPOS, batteryPercent, chargingFlag);
      //display.setTextSize(1);
      //display.setTextColor(WHITE);
      //display.print("batteryPercent:"); display.println(batteryPercent);
      continue;
    }
    if (cableIconIndex == iconIndex) {
      drawCableIcon(display, iconXPOS, iconYPOS, cableStatus);
      continue;
    }
    if (showMissingIconIfEmpty == true) {
      display.drawBitmap(iconXPOS, iconYPOS, icon8x8_missing_icon_bmp, 8, 8, WHITE);
      if (displayFlag) {
        display.display();
      };
    }
  }

  if (displayFlag) {
    display.display();
  };
}



/*
 * Struct Images and Animations: Begin
 */
typedef struct {
  //https://www.tutorialspoint.com/structs-in-arduino-program
  char imageName[32] = "IMAGE";//max name length = 31 charaters
  uint16_t imageByteLength;
  uint16_t widthPx;//in pixels
  uint16_t heightPx;//in pixels
  unsigned char* image;//size is imageByteLength
} IMGbitmapStruct;



void load_struct_bitmapIMG_dat_File(File &fileBMP, IMGbitmapStruct &bitmapIMG);
void display_struct_bitmapIMG(Adafruit_SH1106 &display, IMGbitmapStruct &bitmapIMG, int locx, int locy);









/* Sorce: https://forum.arduino.cc/t/passing-struct-info-as-function-parameters/37330/2
 *  Sorce: https://forum.arduino.cc/t/passing-struct-info-as-function-parameters/37330/7
 * The "&" effectively means that the original will be operated on rather than a copy that only exists in the scope of the function.
 * References are a less powerful but far easier to use derivative of pointers, Google should find more about them if you want to know.
*/
void load_struct_bitmapIMG_dat_File(File &fileBMP, IMGbitmapStruct &bitmapIMG) {
  if (!fileBMP) {
    Serial.println("Failed to open file for reading");
    return;
  }
  //→ used for struct pointer
  //Serial.println(fileBMP.seek(0));
  bitmapIMG.imageByteLength = ((fileBMP.read()) << 8) | (fileBMP.read());
  bitmapIMG.widthPx = ((fileBMP.read()) << 8) | (fileBMP.read());
  bitmapIMG.widthPx++;
  bitmapIMG.imageByteLength -= 2;
  bitmapIMG.heightPx = ((fileBMP.read()) << 8) | (fileBMP.read());
  bitmapIMG.heightPx++;
  bitmapIMG.imageByteLength -= 2;
  unsigned char* imageTemp;
  imageTemp = (unsigned char*)malloc(bitmapIMG.imageByteLength + 1);
  bitmapIMG.image = (unsigned char*)malloc(bitmapIMG.imageByteLength);
  fileBMP.read(imageTemp, bitmapIMG.imageByteLength);
  for (int index=0; index < bitmapIMG.imageByteLength; index++){
    bitmapIMG.image[index] = imageTemp[index];
  }
  free(imageTemp);
}


void display_struct_bitmapIMG(Adafruit_SH1106 &display, IMGbitmapStruct &bitmapIMG, int locx, int locy) {
  display.drawBitmap(locx, locy, bitmapIMG.image, bitmapIMG.widthPx, bitmapIMG.heightPx, WHITE);
}




typedef struct {
  uint16_t locxPxOffset=0;//X Offset in pixels
  uint16_t locyPxOffset=0;//Y Offset in pixels
  uint16_t widthPx;//in pixels(Global)
  uint16_t heightPx;//in pixels(Global)
  
  //subframeDataRotation is used if "rotationSFCompression" is Enabled
  uint8_t subframeDataRotation=0;//data rotated Clockwise 4 phases
  /* subframeDataRotation
   * 0 == 0 CW rotations Needed
   * 1 == 3 CW rotations Needed
   * 2 == 2 CW rotations Needed
   * 3 == 1 CW rotations Needed
   */
  unsigned int frameStaydelayms = 100;// units = milliseconds
  uint16_t frameByteLength;//frame data byteSize
  unsigned char* frameDataPool;//size is gifByteLength
  unsigned long lastMillisecTime = 0;
} GIFframebitmapStruct;


typedef struct {
  //https://www.tutorialspoint.com/structs-in-arduino-program
  char imageName[32] = "GIF";//max name length = 31 charaters
  uint16_t gifByteLength;//total image byteSize
  uint16_t frameCount;
  GIFframebitmapStruct *subFramePointers;//[frameCount]
  uint16_t globalsubimageByteLength;
  uint16_t widthGPx;//in pixels(Global)
  uint16_t heightGPx;//in pixels(Global)
  
  byte subframeCompressionFlags;//size is frameCount //8 flags each frame
  bool cropSFCompression = false;
  bool rotationSFCompression = false;

  uint16_t* subimageByteLength; //size is frameCount
  
  //locxPxOffset, locyPxOffset, widthPx and HeightPx are used if "cropSFCompression" is Enabled
  uint16_t* locxPxOffset;//size is frameCount //X Offset in pixels
  uint16_t* locyPxOffset;//size is frameCount //Y Offset in pixels
  uint16_t* widthPx;//size is frameCount //in pixels(Sub Image)
  uint16_t* heightPx;//size is frameCount //in pixels(Sub Image)
  
  
  //subframeDataRotation is used if "rotationSFCompression" is Enabled
  uint8_t* subframeDataRotation;//size is frameCount //data rotated Clockwise 4 phases
  /* subframeDataRotation
   * 0 == 0 CW rotations Needed
   * 1 == 3 CW rotations Needed
   * 2 == 2 CW rotations Needed
   * 3 == 1 CW rotations Needed
   */
  uint16_t* subimageByteOffset; //size is frameCount
  unsigned int* frameStaydelayms;//size is frameCount// units = milliseconds
  unsigned char* frameDataPool;//size is gifByteLength
  unsigned long lastMillisecTime = 0;
} GIFbitmapStruct;


void load_struct_bitmapGIF_dat_File(File &file, GIFbitmapStruct &bitmapGIF);
void display_struct_bitmapGIF(Adafruit_SH1106 &display, GIFbitmapStruct &bitmapGIF, int frame, int locx, int locy);



void load_struct_bitmapGIF_dat_File(File &file, GIFbitmapStruct &bitmapGIF) {
  Serial.println("load_struct_bitmapGIF_dat_File");
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }
  printFreeHeap(Serial);
  unsigned long filesize = file.size();
  //→ used for struct pointer
  //check first 2 bytes, if they are not 0 then its the old formating
  bool formatCheck;
  char* checkHeaderFormat;
  checkHeaderFormat = (char*)malloc(7);
  memset(checkHeaderFormat, '\0', 7);
  file.seek(0);
  checkHeaderFormat[0] = (char)file.peek();
  file.seek(1);
  checkHeaderFormat[1] = (char)file.peek();
  file.seek(2);
  checkHeaderFormat[2] = (char)file.peek();
  file.seek(3);
  checkHeaderFormat[3] = (char)file.peek();
  file.seek(4);
  checkHeaderFormat[4] = (char)file.peek();
  file.seek(5);
  checkHeaderFormat[5] = (char)file.peek();
  file.seek(0);
  if (strncpy(checkHeaderFormat,"datgif", 6)==0){
    formatCheck = true;
  }else{
    formatCheck = false;
  }
  
  
  unsigned char* imageTemp;
  
  unsigned int gifdataReadIndex = 0;
  unsigned int imagedataSize = 0;
  Serial.println("load_struct_bitmapGIF_dat_File -Check -00");
  printFreeHeap(Serial);
  if (formatCheck==false){
    Serial.println("load_struct_bitmapGIF_dat_File -Check -01");
    //old format
    //2 bytes - subframeDataSize (includes subframe variables)
    //2 bytes - subframeWidth         (if "enabled") (cropSFCompression OR rotationSFCompression)
    //2 bytes - subframeHeight        (if "enabled") (cropSFCompression OR rotationSFCompression)
    //# bytes - frameData (byte size = subframeDataSize)
    //repeat until end of File
    unsigned int frameIndex = 0;
    
    unsigned int frameDataSize;
    file.seek(0);
    frameDataSize = ((file.read()) << 8) | (file.read());
    file.seek(0);
    
    printFreeHeap(Serial);
    bitmapGIF.frameCount = int(filesize / (frameDataSize+2));
    //bitmapGIF.frameDataPool = (unsigned char*)malloc(bitmapGIF.frameCount*(frameDataSize-2-2));
    Serial.print("bitmapGIF.frameDataPool: ");Serial.println( bitmapGIF.frameCount*(frameDataSize-2-2) );
    
    
    bitmapGIF.subFramePointers = (GIFframebitmapStruct*)malloc(bitmapGIF.frameCount);
    
    
    //bitmapGIF.subimageByteOffset = (uint16_t*)malloc(bitmapGIF.frameCount);
    //bitmapGIF.frameStaydelayms = (unsigned int*)malloc(bitmapGIF.frameCount);
    bitmapGIF.subframeCompressionFlags = 0b00000000;
    Serial.println("load_struct_bitmapGIF_dat_File -Check -02");
    gifdataReadIndex = 0;
    bitmapGIF.gifByteLength = 0;
    frameIndex = 0;
    while (file.available()) {
      Serial.print("load_struct_bitmapGIF_dat_File -Check -03 -");Serial.println(frameIndex);
      bitmapGIF.subFramePointers[ frameIndex ].frameByteLength = ((file.read()) << 8) | (file.read());
      bitmapGIF.subFramePointers[ frameIndex ].widthPx = ((file.read()) << 8) | (file.read());
      bitmapGIF.subFramePointers[ frameIndex ].widthPx++;
      bitmapGIF.subFramePointers[ frameIndex ].frameByteLength -= 2;
      bitmapGIF.subFramePointers[ frameIndex ].heightPx = ((file.read()) << 8) | (file.read());
      bitmapGIF.subFramePointers[ frameIndex ].heightPx++;
      bitmapGIF.subFramePointers[ frameIndex ].frameByteLength -= 2;
      Serial.println("load_struct_bitmapGIF_dat_File -Check -04");
      printFreeHeap(Serial);
      
      bitmapGIF.subFramePointers[ frameIndex ].frameDataPool = (unsigned char*)malloc(bitmapGIF.subFramePointers[ frameIndex ].frameByteLength);
      file.read(bitmapGIF.subFramePointers[ frameIndex ].frameDataPool, bitmapGIF.subFramePointers[ frameIndex ].frameByteLength);
      
      display.clearDisplay();
      display.drawBitmap(0,0, bitmapGIF.subFramePointers[ frameIndex ].frameDataPool, bitmapGIF.subFramePointers[ frameIndex ].widthPx, bitmapGIF.subFramePointers[ frameIndex ].heightPx, WHITE);
      display.display();
      
      Serial.println("load_struct_bitmapGIF_dat_File -Check -05");
      
      printFreeHeap(Serial);
      bitmapGIF.subFramePointers[ frameIndex ].frameStaydelayms = 1;
      gifdataReadIndex += bitmapGIF.globalsubimageByteLength;
      bitmapGIF.gifByteLength += bitmapGIF.globalsubimageByteLength;
      
      frameIndex++;
    }
    frameIndex--;
    Serial.print("frameIndex: ");Serial.println(frameIndex);
  }
  if (formatCheck==true){
    /*needs to be tested*/
    //format of .datgif
    //6 bytes - Fileheader ("datgif")
    //2 bytes - FileheaderVersion ("00")
    //2 byte  - frameCount
    //1 byte  - subframe Compression Flags (8 flags) = {cropSFCompression, rotationSFCompression, unused, unused, unused, unused, unused, unused}
    //2 bytes - GlobalsubframeDataSize
    //2 bytes - globalframeWidth
    //2 bytes - globalframeHeight
    
    
    //subframes
    //2 bytes - subframeDataSize (if "enabled") (cropSFCompression OR rotationSFCompression)(includes subframe variables)
    //2 bytes - subframeWidth         (if "enabled") (cropSFCompression OR rotationSFCompression)
    //2 bytes - subframeHeight        (if "enabled") (cropSFCompression OR rotationSFCompression)
    //2 bytes - subframeLocOffsetX    (if "enabled") (cropSFCompression)
    //2 bytes - subframeLocOffsetY    (if "enabled") (cropSFCompression)
    //1 byte  - subframeDataRotation  (if "enabled") (rotationSFCompression)
    //2 bytes - subframeDelay
    //# bytes - subframeData
    
    file.read((unsigned char*)checkHeaderFormat, 6);
    char* dataFileVersionString;
    dataFileVersionString = (char*)malloc(3);
    memset(dataFileVersionString, '\0', 3);
    file.read((unsigned char*)dataFileVersionString, 2);
    unsigned long fileTempPostition = file.position();
    
    
    bitmapGIF.frameCount = ((file.read()) << 8) | (file.read());
    bitmapGIF.subframeCompressionFlags = file.read();
    bitmapGIF.cropSFCompression = (bitmapGIF.subframeCompressionFlags & 0b10000000)==0b10000000;
    bitmapGIF.rotationSFCompression = (bitmapGIF.subframeCompressionFlags & 0b01000000)==0b01000000;
    bitmapGIF.globalsubimageByteLength = ((file.read()) << 8) | (file.read());
    bitmapGIF.widthGPx = ((file.read()) << 8) | (file.read());
    bitmapGIF.heightGPx = ((file.read()) << 8) | (file.read());
    
    //allocate
    if (bitmapGIF.cropSFCompression | bitmapGIF.rotationSFCompression){
      bitmapGIF.widthPx = (uint16_t*)malloc(bitmapGIF.frameCount);
      bitmapGIF.heightPx = (uint16_t*)malloc(bitmapGIF.frameCount);
      bitmapGIF.subimageByteLength = (uint16_t*)malloc(bitmapGIF.frameCount);
    }
    if (bitmapGIF.cropSFCompression){
      bitmapGIF.locxPxOffset = (uint16_t*)malloc(bitmapGIF.frameCount);
      bitmapGIF.locyPxOffset = (uint16_t*)malloc(bitmapGIF.frameCount);
    }
    if (bitmapGIF.rotationSFCompression){
      bitmapGIF.subframeDataRotation = (uint8_t*)malloc(bitmapGIF.frameCount);
    }
    bitmapGIF.subimageByteOffset = (uint16_t*)malloc(bitmapGIF.frameCount);
    bitmapGIF.frameStaydelayms = (unsigned int*)malloc(bitmapGIF.frameCount);
    
    
    gifdataReadIndex = 0;
    imagedataSize = 0;
    for (int frameIndex = 0; frameIndex < bitmapGIF.frameCount; frameIndex++) {
      //subimageByteLength, subimageByteOffset, frameStaydelayms, locxPxOffset, locyPxOffset, widthPx, heightPx, subframeDataRotation
      bitmapGIF.subimageByteOffset[ frameIndex ] = gifdataReadIndex;

      if (bitmapGIF.cropSFCompression | bitmapGIF.rotationSFCompression){
        bitmapGIF.subimageByteLength[ frameIndex ] = ((file.read()) << 8) | (file.read());
        bitmapGIF.widthPx[ frameIndex ] = ((file.read()) << 8) | (file.read());
        bitmapGIF.subimageByteLength[ frameIndex ] -= 2;
        bitmapGIF.heightPx[ frameIndex ] = ((file.read()) << 8) | (file.read());
        bitmapGIF.subimageByteLength[ frameIndex ] -= 2;
      }
      if (bitmapGIF.cropSFCompression){
        bitmapGIF.locxPxOffset[ frameIndex ] = ((file.read()) << 8) | (file.read());
        bitmapGIF.subimageByteLength[ frameIndex ] -= 2;
        bitmapGIF.locyPxOffset[ frameIndex ] = ((file.read()) << 8) | (file.read());
        bitmapGIF.subimageByteLength[ frameIndex ] -= 2;
      }
      if (bitmapGIF.rotationSFCompression){
        bitmapGIF.subframeDataRotation[ frameIndex ] = file.read();
        bitmapGIF.subimageByteLength[ frameIndex ] -= 1;
      }
      bitmapGIF.frameStaydelayms[ frameIndex ] = ((file.read()) << 8) | (file.read());
      if (bitmapGIF.cropSFCompression | bitmapGIF.rotationSFCompression){
        bitmapGIF.subimageByteLength[ frameIndex ] -= 2;
        
        gifdataReadIndex += bitmapGIF.subimageByteLength[ frameIndex ];
        imagedataSize += bitmapGIF.subimageByteLength[ frameIndex ];
        file.read( &bitmapGIF.frameDataPool[ bitmapGIF.subimageByteOffset[ frameIndex ] ], bitmapGIF.subimageByteLength[ frameIndex ] );
      }else {
        bitmapGIF.globalsubimageByteLength -= 2;
        
        gifdataReadIndex += bitmapGIF.globalsubimageByteLength;
        imagedataSize += bitmapGIF.globalsubimageByteLength;
        file.read( &bitmapGIF.frameDataPool[ bitmapGIF.subimageByteOffset[ frameIndex ] ], bitmapGIF.globalsubimageByteLength );
      }
      
    }
    
    free(dataFileVersionString);
  }
  free(checkHeaderFormat);
  Serial.println("load_struct_bitmapGIF_dat_File: End");
}


void display_struct_bitmapGIF(Adafruit_SH1106 &display, GIFbitmapStruct &bitmapGIF, int frame, int locx, int locy) {
  Serial.println("display_struct_bitmapGIF");
  for(byte index = frame; index <= frame; index++){
    printFreeHeap(Serial);
    Serial.print("index: ");Serial.println(index);
    Serial.print("subimageByteOffset: ");Serial.println(bitmapGIF.subimageByteOffset[ index ]);
    Serial.print("frameCount: ");Serial.println(bitmapGIF.frameCount);
    Serial.print("gifByteLength: ");Serial.println(bitmapGIF.gifByteLength);
    
    if (bitmapGIF.cropSFCompression | bitmapGIF.rotationSFCompression){
      unsigned char* imageTemp;
      imageTemp = (unsigned char*)malloc(bitmapGIF.subimageByteLength[ index ]);
      Serial.print("subimageByteLength: ");Serial.println(bitmapGIF.subimageByteLength[ index ]);
      memcpy(imageTemp, &bitmapGIF.frameDataPool[ bitmapGIF.subimageByteOffset[ index ] ], bitmapGIF.subimageByteLength[ index ]);
      if (bitmapGIF.cropSFCompression){
        display.drawBitmap(locx+bitmapGIF.locxPxOffset[ index ], locy+bitmapGIF.locyPxOffset[ index ], imageTemp, bitmapGIF.widthPx[ index ], bitmapGIF.heightPx[ index ], WHITE);
      }else if (bitmapGIF.rotationSFCompression){
        //add rotate
        display.drawBitmap(locx, locy, imageTemp, bitmapGIF.widthPx[ index ], bitmapGIF.heightPx[ index ], WHITE);
      }
      free(imageTemp);
    }else{
      Serial.println("Old Format");
      //Serial.println(bitmapGIF.globalsubimageByteLength);
      //Serial.println(ESP.getMaxAllocHeap());
      /*
      uint32_t getHeapSize(); //total heap size
      uint32_t getFreeHeap(); //available heap
      uint32_t getMinFreeHeap(); //lowest level of free heap since boot
      uint32_t getMaxAllocHeap(); //largest block of heap that can be allocated at once
      */
      printFreeHeap(Serial);
      display.drawBitmap(bitmapGIF.subFramePointers[ index ].locxPxOffset, bitmapGIF.subFramePointers[ index ].locyPxOffset, bitmapGIF.subFramePointers[ index ].frameDataPool, bitmapGIF.subFramePointers[ index ].widthPx, bitmapGIF.subFramePointers[ index ].heightPx, WHITE);
    }
    printFreeHeap(Serial);
  }
  Serial.println("display_struct_bitmapGIF: End");
}



/*
 * Struct Images and Animations: End
 */















void load_bitmapdat_File(unsigned char*&, unsigned int&, unsigned int&, unsigned int&, File &);

void load_bitmapdat_File(unsigned char* &imageloadlocation, unsigned int &imageDataLength, unsigned int &widthPointer, unsigned int &heightPointer, File &fileBMP) {
  if (!fileBMP) {
    Serial.println("Failed to open file for reading");
    return;
  }
  Serial.printf("Address stored in widthPointer variable: %x\n", &widthPointer  );
  Serial.printf("Value of widthPointer variable: %d\n", widthPointer );
  Serial.printf("Address stored in heightPointer variable: %x\n", &heightPointer  );
  Serial.printf("Value of heightPointer variable: %d\n", heightPointer );
  unsigned int frameDataSize;
  frameDataSize = ((fileBMP.read()) << 8) | (fileBMP.read());
  unsigned int frameWidthSize;
  frameWidthSize = ((fileBMP.read()) << 8) | (fileBMP.read()); frameWidthSize++;
  frameDataSize -= 2;
  unsigned int frameHeightSize;
  frameHeightSize = ((fileBMP.read()) << 8) | (fileBMP.read()); frameHeightSize++;
  frameDataSize -= 2;
  //unsigned char* fileImageDataBuffer;
  //fileImageDataBuffer = (unsigned char*)malloc(frameDataSize+1);
  imageloadlocation = (unsigned char*)malloc(frameDataSize + 1);
  fileBMP.read(imageloadlocation, frameDataSize);
  widthPointer = frameWidthSize;
  heightPointer = frameHeightSize;
  imageDataLength = frameDataSize;

  //display.drawBitmap(0, 0, fileImageDataBuffer, frameWidthSize, frameHeightSize, WHITE);
}





void readbitmapdatFile(Adafruit_SH1106 &display, File &fileBMP) {
  if (!fileBMP) {
    Serial.println("Failed to open file for reading");
    return;
  }
  Serial.println("File Content:");
  while (fileBMP.available()) {
    unsigned int frameDataSize;
    frameDataSize = ((fileBMP.read()) << 8) | (fileBMP.read());
    unsigned int frameWidthSize;
    frameWidthSize = ((fileBMP.read()) << 8) | (fileBMP.read()); frameWidthSize++;
    frameDataSize -= 2;
    unsigned int frameHeightSize;
    frameHeightSize = ((fileBMP.read()) << 8) | (fileBMP.read()); frameHeightSize++;
    frameDataSize -= 2;
    unsigned char* fileImageDataBuffer;
    fileImageDataBuffer = (unsigned char*)malloc(frameDataSize + 1);
    //Serial.println(fileBMP.available());Serial.println(frameDataSize);
    fileBMP.read(fileImageDataBuffer, frameDataSize);
    
    display.clearDisplay();
    display.drawBitmap(0, 0, fileImageDataBuffer, frameWidthSize, frameHeightSize, WHITE);
    display.display();
    free(fileImageDataBuffer);
  }
}








/* Baud-rates available: 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 57600, or 115200, 256000, 512000, 962100

    Protocols available:
   SERIAL_5N1   5-bit No parity 1 stop bit
   SERIAL_6N1   6-bit No parity 1 stop bit
   SERIAL_7N1   7-bit No parity 1 stop bit
   SERIAL_8N1 (the default) 8-bit No parity 1 stop bit
   SERIAL_5N2   5-bit No parity 2 stop bits
   SERIAL_6N2   6-bit No parity 2 stop bits
   SERIAL_7N2   7-bit No parity 2 stop bits
   SERIAL_8N2   8-bit No parity 2 stop bits
   SERIAL_5E1   5-bit Even parity 1 stop bit
   SERIAL_6E1   6-bit Even parity 1 stop bit
   SERIAL_7E1   7-bit Even parity 1 stop bit
   SERIAL_8E1   8-bit Even parity 1 stop bit
   SERIAL_5E2   5-bit Even parity 2 stop bit
   SERIAL_6E2   6-bit Even parity 2 stop bit
   SERIAL_7E2   7-bit Even parity 2 stop bit
   SERIAL_8E2   8-bit Even parity 2 stop bit
   SERIAL_5O1   5-bit Odd  parity 1 stop bit
   SERIAL_6O1   6-bit Odd  parity 1 stop bit
   SERIAL_7O1   7-bit Odd  parity 1 stop bit
   SERIAL_8O1   8-bit Odd  parity 1 stop bit
   SERIAL_5O2   5-bit Odd  parity 2 stop bit
   SERIAL_6O2   6-bit Odd  parity 2 stop bit
   SERIAL_7O2   7-bit Odd  parity 2 stop bit
   SERIAL_8O2   8-bit Odd  parity 2 stop bit
*/

void disableWiFi();
void enableWiFi();



//https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/wifi-security.html
struct WiFiconfig {
  //six characters are not allowed (SSID): ?, ", $, [, \, ], and +
  char ssid[33];//max length 32 //Min Length 2
  char passphrase[64];//max length 63
};



const char *filenameWiFi = "/Wifi_Connections.txt";
WiFiconfig wificonfig;

/*
#define STORAGE_SELECT_SD 0
#define STORAGE_SELECT_SPIFFS 1
#define STORAGE_SELECT_SDFAT 2
#define STORAGE_SELECT_FFAT 3

#define STORAGE_INCLUDE_SD false
#define STORAGE_INCLUDE_SPIFFS true
#define STORAGE_INCLUDE_SDFAT false
#define STORAGE_INCLUDE_FFAT false
*/

// Loads the configuration from a file
void loadWiFiConfiguration(const char *filename, WiFiconfig &wificonfig, byte storageDevice = STORAGE_DEVICE_DEFAULT) {
  // Open file for reading
  File file;
  #if (STORAGE_INCLUDE_SD)
  if (storageDevice == STORAGE_SELECT_SD){
    file = SD.open(filename);
  }
  #endif
  #if (STORAGE_INCLUDE_SPIFFS)
  if (storageDevice == STORAGE_SELECT_SPIFFS){
    file = SPIFFS.open(filename);
  }
  #endif
  
  
  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use https://arduinojson.org/v6/assistant to compute the capacity.
  StaticJsonDocument<512> doc;

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, file);
  if (error)
    Serial.println(F("Failed to read file, using default configuration"));

  // Copy values from the JsonDocument to the Config
  strlcpy(wificonfig.passphrase,                  // <- destination
          doc["passphrase"] | "",  // <- source
          sizeof(wificonfig.passphrase));         // <- destination's capacity
  strlcpy(wificonfig.ssid,                  // <- destination
          doc["ssid"] | "",  // <- source
          sizeof(wificonfig.ssid));         // <- destination's capacity

  // Close the file (Curiously, File's destructor doesn't close the file)
  file.close();
}


// Saves the configuration to a file
void saveWiFiConfiguration(const char *filename, const WiFiconfig &wificonfig, byte storageDevice = STORAGE_DEVICE_DEFAULT) {
  // Delete existing file, otherwise the configuration is appended to the file
  #if (STORAGE_INCLUDE_SD)
  if (storageDevice == STORAGE_SELECT_SD){
    SD.remove(filename);
  }
  #endif
  #if (STORAGE_INCLUDE_SPIFFS)
  if (storageDevice == STORAGE_SELECT_SPIFFS){
    SPIFFS.remove(filename);
  }
  #endif
  
  // Open file for writing
  File file;
  #if (STORAGE_INCLUDE_SD)
  if (storageDevice == STORAGE_SELECT_SD){
    file = SD.open(filename, FILE_WRITE);
  }
  #endif
  #if (STORAGE_INCLUDE_SPIFFS)
  if (storageDevice == STORAGE_SELECT_SPIFFS){
    file = SPIFFS.open(filename, FILE_WRITE);
  }
  #endif

  
  if (!file) {
    Serial.println(F("Failed to create file"));
    return;
  }

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use https://arduinojson.org/assistant to compute the capacity.
  StaticJsonDocument<256> doc;

  // Set the values in the document
  doc["ssid"] = wificonfig.ssid;
  doc["passphrase"] = wificonfig.passphrase;

  // Serialize JSON to file
  if (serializeJson(doc, file) == 0) {
    Serial.println(F("Failed to write to file"));
  }

  // Close the file
  file.close();
}

// Prints the content of a file to the Serial
void printWiFiFile(const char *filename, byte storageDevice = STORAGE_DEVICE_DEFAULT) {
  // Open file for reading
  File file;
  #if (STORAGE_INCLUDE_SD)
  if (storageDevice == STORAGE_SELECT_SD){
    file = SD.open(filename);
  }
  #endif
  #if (STORAGE_INCLUDE_SPIFFS)
  if (storageDevice == STORAGE_SELECT_SPIFFS){
    file = SPIFFS.open(filename);
  }
  #endif
  
  if (!file) {
    Serial.println(F("Failed to read file"));
    return;
  }

  // Extract each characters by one by one
  while (file.available()) {
    Serial.print((char)file.read());
  }
  Serial.println();

  // Close the file
  file.close();
}










// Our configuration structure.
//
// Never use a JsonDocument to store the configuration!
// A JsonDocument is *not* a permanent storage; it's only a temporary storage
// used during the serialization phase. See:
// https://arduinojson.org/v6/faq/why-must-i-create-a-separate-config-object/
struct Config {
  char hostname[64];
  int port;
};

const char *filename = "/config.txt";  // <- SD library uses 8.3 filenames
Config config;                         // <- global configuration object

/*
#define STORAGE_SELECT_SD 0
#define STORAGE_SELECT_SPIFFS 1
#define STORAGE_SELECT_SDFAT 2
#define STORAGE_SELECT_FFAT 3

#define STORAGE_INCLUDE_SD false
#define STORAGE_INCLUDE_SPIFFS true
#define STORAGE_INCLUDE_SDFAT false
#define STORAGE_INCLUDE_FFAT false
*/

// Loads the configuration from a file
void loadConfiguration(const char *filename, Config &config, byte storageDevice = STORAGE_DEVICE_DEFAULT) {
  // Open file for reading
  File file;
  #if (STORAGE_INCLUDE_SD)
  if (storageDevice == STORAGE_SELECT_SD){
    file = SD.open(filename);
  }
  #endif
  #if (STORAGE_INCLUDE_SPIFFS)
  if (storageDevice == STORAGE_SELECT_SPIFFS){
    file = SPIFFS.open(filename);
  }
  #endif
  
  
  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use https://arduinojson.org/v6/assistant to compute the capacity.
  StaticJsonDocument<512> doc;

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, file);
  if (error)
    Serial.println(F("Failed to read file, using default configuration"));

  // Copy values from the JsonDocument to the Config
  config.port = doc["port"] | 2731;
  strlcpy(config.hostname,                  // <- destination
          doc["hostname"] | "example.com",  // <- source
          sizeof(config.hostname));         // <- destination's capacity

  // Close the file (Curiously, File's destructor doesn't close the file)
  file.close();
}


// Saves the configuration to a file
void saveConfiguration(const char *filename, const Config &config, byte storageDevice = STORAGE_DEVICE_DEFAULT) {
  // Delete existing file, otherwise the configuration is appended to the file
  #if (STORAGE_INCLUDE_SD)
  if (storageDevice == STORAGE_SELECT_SD){
    SD.remove(filename);
  }
  #endif
  #if (STORAGE_INCLUDE_SPIFFS)
  if (storageDevice == STORAGE_SELECT_SPIFFS){
    SPIFFS.remove(filename);
  }
  #endif
  
  // Open file for writing
  File file;
  #if (STORAGE_INCLUDE_SD)
  if (storageDevice == STORAGE_SELECT_SD){
    file = SD.open(filename, FILE_WRITE);
  }
  #endif
  #if (STORAGE_INCLUDE_SPIFFS)
  if (storageDevice == STORAGE_SELECT_SPIFFS){
    file = SPIFFS.open(filename, FILE_WRITE);
  }
  #endif

  
  if (!file) {
    Serial.println(F("Failed to create file"));
    return;
  }

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use https://arduinojson.org/assistant to compute the capacity.
  StaticJsonDocument<256> doc;

  // Set the values in the document
  doc["hostname"] = config.hostname;
  doc["port"] = config.port;

  // Serialize JSON to file
  if (serializeJson(doc, file) == 0) {
    Serial.println(F("Failed to write to file"));
  }

  // Close the file
  file.close();
}

// Prints the content of a file to the Serial
void printFile(const char *filename, byte storageDevice = STORAGE_DEVICE_DEFAULT) {
  // Open file for reading
  File file;
  #if (STORAGE_INCLUDE_SD)
  if (storageDevice == STORAGE_SELECT_SD){
    file = SD.open(filename);
  }
  #endif
  #if (STORAGE_INCLUDE_SPIFFS)
  if (storageDevice == STORAGE_SELECT_SPIFFS){
    file = SPIFFS.open(filename);
  }
  #endif
  
  if (!file) {
    Serial.println(F("Failed to read file"));
    return;
  }

  // Extract each characters by one by one
  while (file.available()) {
    Serial.print((char)file.read());
  }
  Serial.println();

  // Close the file
  file.close();
}







void OnWiFiEvent(WiFiEvent_t event)
{
  switch (event) {
    case SYSTEM_EVENT_STA_CONNECTED:
      Serial.println("ESP32 Connected to WiFi Network");
      break;
    case SYSTEM_EVENT_AP_START:
      Serial.println("ESP32 soft AP started");
      break;
    case SYSTEM_EVENT_AP_STACONNECTED:
      Serial.println("Station connected to ESP32 soft AP");
      break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
      Serial.println("Station disconnected from ESP32 soft AP");
      break;
    default: 
      Serial.printf("Got Event: %d\n", event);
      break;
  }
}


TaskHandle_t Task1;
TaskHandle_t Task2;

//https://forum.arduino.cc/t/how-can-i-do-dual-core-task-use-same-memory-esp32-rtos/702929/24?page=2 //https://youtu.be/ywbq1qR-fY0?t=1206
QueueHandle_t queue;



//https://docs.espressif.com/projects/esp-idf/en/v4.3/esp32s2/api-reference/peripherals/index.html
void setup()   {
  Serial.begin(115200);
  Serial.print("setup() running on core ");Serial.println(xPortGetCoreID());
  Serial.print("ESP.getFreeHeap():"); Serial.println(ESP.getFreeHeap());
  //https://espressif-docs.readthedocs-hosted.com/projects/arduino-esp32/en/latest/api/preferences.html
  //https://randomnerdtutorials.com/esp32-save-data-permanently-preferences/
  Serial.println( preferences.begin("boyKisser", false, "nvs"));
  preferences.end();
  //setup Pins

  #if (FeatureEnable_IR_RXTX_Control==true)
  //IR Pins
  //pinMode(IR_TX_Pin, OUTPUT);
  IrSender.begin(IR_TX_Pin, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN); // Specify send pin and enable feedback LED at default feedback LED pin

  //pinMode(IR_RX_Pin, INPUT);
  IrReceiver.begin(IR_RX_Pin, ENABLE_LED_FEEDBACK); // Start the receiver
  #endif
  
  //MIC Pin
  pinMode(MIC_Pin, INPUT);


  //Buzzer Boi Pin
  //https://github.com/pschatzmann/arduino-audio-tools
  pinMode(buzzerPin, INPUT);

  //BatterySensePin
  pinMode(batteryVoltageDivider, INPUT);

  e220ttl.begin();//LoRa Usage

  //Enable Devices
  adc_power_on();

  //https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/performance/size.html#idf-py-size
  Serial.print("ESP.getFreeHeap():"); Serial.println(ESP.getFreeHeap());
#if (FeatureEnable_BT_Serial==true)
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  Serial.print("ESP.getFreeHeap():"); Serial.println(ESP.getFreeHeap());
#endif



  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  File file;
  file = SPIFFS.open("/test_example.txt");
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.println("File Content:");
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();

  file = SPIFFS.open("/Wifi_Connections.txt");
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.println("File Content:");
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();





  /*
    Dictionary *dict = new Dictionary();//buttonNamesDict
    dict->insert("0", "key_Up");
    dict->insert("1", "key_Start");
    dict->insert("2", "key_Select");
    dict->insert("3", "key_A");
    dict->insert("4", "key_Left");
    dict->insert("5", "key_Down");
    dict->insert("6", "key_Right");
    dict->insert("7", "key_B");
  */


  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(200);

  // Clear the buffer.
  display.stopscroll();
  display.clearDisplay();



  //Text Image Read
  File fileBMP;
  Serial.print("ESP.getFreeHeap():"); Serial.println(ESP.getFreeHeap());

  /*
    static unsigned char animationFrameBuffer[1024*16];
    unsigned char* framepointerArray[22];

    fileBMP = SPIFFS.open("/boyKisserFaceGif_bitmapgif.dat");
    //loads file into large buffer then saves pointers and plays frames from pointers
    unsigned char* imageloadlocation;
    unsigned int imageDataLength = 0;
    unsigned int imageWidth = 0;
    unsigned int imageHeight = 0;
    Serial.println("File Content:");
    unsigned int currentFrame = 0;
    unsigned int currentAnimationDataLength = 0;

    while(fileBMP.available()){
    load_bitmapdat_File(imageloadlocation, imageDataLength, imageWidth, imageHeight, fileBMP);
    framepointerArray[currentFrame] = &animationFrameBuffer[ currentAnimationDataLength ];
    memcpy(framepointerArray[currentFrame], imageloadlocation, imageDataLength);
    currentAnimationDataLength += imageDataLength;
    Serial.println(currentFrame);
    currentFrame++;
    }

    fileBMP.close();

    long int t1 = micros();
    for (unsigned int index=0; index < 22; index++){
    display.clearDisplay();
    display.drawBitmap(0, 0, framepointerArray[index], imageWidth, imageHeight, WHITE);
    display.display();
    }
    long int t2 = micros();
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.print("Frames: "); display.print( 22 ); display.println();
    display.print("T-Time(ns): "); display.print( t2-t1 ); display.println("");
    display.print("FPS: "); display.print( (double)(22/((double)(t2-t1)/1000000)) ); display.println();
    //Serial.print("Time taken by the task: "); Serial.print(t2-t1); Serial.println(" microseconds(ns)");
    display.display();
    delay(1500);
  */
  Serial.print("ESP.getFreeHeap():"); Serial.println(ESP.getFreeHeap());
  {
  printFreeHeap(Serial);
  File fileBMP = SPIFFS.open("/boyKisserFaceGif_bitmapgif.dat");
  IMGbitmapStruct boyKisserTest;
  printFreeHeap(Serial);
  Serial.println("IMGbitmapStruct Test: Begin");
  while (fileBMP.available()) {
    load_struct_bitmapIMG_dat_File(fileBMP, boyKisserTest);
    display.clearDisplay();
    display_struct_bitmapIMG(display, boyKisserTest, 0, 0);
    display.display();
  }
  fileBMP.close();
  printFreeHeap(Serial);
  }
  delay(1000);
  /*
  { // Fix Me!
  GIFbitmapStruct boyKisserGIFTest;
  
  File fileStructBMP;
  fileStructBMP = SPIFFS.open("/boyKisserFaceGif_bitmapgif.dat");
  load_struct_bitmapGIF_dat_File(fileStructBMP, boyKisserGIFTest);
  Serial.println(fileStructBMP);
  fileStructBMP.close();
  printFreeHeap(Serial);
  
  for(byte frame = 0; frame < boyKisserGIFTest.frameCount; frame++){
    Serial.print("Frame: ");Serial.print(frame);Serial.print("/");Serial.println(boyKisserGIFTest.frameCount);
    display.clearDisplay();
    display_struct_bitmapGIF(display, boyKisserGIFTest, frame, 0, 0);
    display.display();
    Serial.print("Frame: ");Serial.print(frame);Serial.print("/");Serial.println(boyKisserGIFTest.frameCount);
  }
  //free(boyKisserGIFTest);
  Serial.println("IMGbitmapStruct Test: End");
  }
  //*/
  delay(5000);
  
  
  
  
  
  
  fileBMP = SPIFFS.open("/batteryBig_bitmapimg.dat");
  Serial.println("File Content:");
  while (fileBMP.available()) {
    unsigned char* imageloadlocation;
    unsigned int imageDataLength = 0;
    load_bitmapdat_File(imageloadlocation, imageDataLength, batteryIconBigWidth, batteryIconBigHeight, fileBMP);
    batteryIconBigImage = (unsigned char*)malloc(imageDataLength + 1);
    memcpy(&batteryIconBigImage[0], imageloadlocation, imageDataLength);
  }
  fileBMP.close();

  fileBMP = SPIFFS.open("/Neco-Arc_bitmapimg.dat");
  Serial.println("File Content:");
  while (fileBMP.available()) {
    unsigned char* imageloadlocation;
    unsigned int imageDataLength = 0;
    unsigned int imageWidth = 0;
    unsigned int imageHeight = 0;
    load_bitmapdat_File(imageloadlocation, imageDataLength, imageWidth, imageHeight, fileBMP);
    //adds to charater Buffer
    memcpy(&visableCharacterBuffer[ 0 ], imageloadlocation, imageDataLength);//4*56
  }
  fileBMP.close();

  Serial.print("ESP.getFreeHeap():"); Serial.println(ESP.getFreeHeap());



  fileBMP = SPIFFS.open("/boyKisserFaceGif_bitmapgif.dat");
  readbitmapdatFile(display, fileBMP);
  fileBMP.close();
  delay(500);
  //Neco-Arc_bitmapimg.dat
  fileBMP = SPIFFS.open("/Neco-Arc_bitmapimg.dat");
  readbitmapdatFile(display, fileBMP);
  fileBMP.close();
  delay(500);

  Serial.print("ESP.getFreeHeap():"); Serial.println(ESP.getFreeHeap());



  fileBMP = SPIFFS.open("/boyKisserFaceGif_bitmapgif.hex");
  if (!fileBMP) {
    Serial.println("Failed to open file for reading");
    return;
  }
  Serial.println("File Content:");
  while (fileBMP.available()) {
    char fileDataSizeBuffer[(2) * 2 + 1];
    char fileImageDataBuffer[2048];
    memset(fileDataSizeBuffer, '\0', (2) * 2 + 1);
    memset(fileImageDataBuffer, '\0', 2048);
    fileBMP.readBytes(fileDataSizeBuffer, (2) * 2);
    //Serial.write(fileDataSizeBuffer, strlen(fileDataSizeBuffer));
    //Serial.println();
    uint16_t frameDataSize;
    char *ptr;
    frameDataSize = strtol(fileDataSizeBuffer, &ptr, 16);
    fileBMP.readBytes(fileImageDataBuffer, (frameDataSize) * 2);
    //Serial.write(fileImageDataBuffer, strlen(fileImageDataBuffer));
    //Serial.println();
    char tempCharArray[3]; memset(tempCharArray, '\0', 3);
    strncpy(tempCharArray, &fileImageDataBuffer[(0 * 2) * 2], (2) * 2);
    uint16_t frameWidthSize = strtol(tempCharArray, &ptr, 16) + 1;
    strncpy(tempCharArray, &fileImageDataBuffer[(1 * 2) * 2], (2) * 2);
    uint16_t frameHeightSize = strtol(tempCharArray, &ptr, 16) + 1;
    display.clearDisplay();
    display.drawBitmap(0, 0, convertTextHextoCharArray( &fileImageDataBuffer[(2) * 2 + (2) * 2] ), frameWidthSize, frameHeightSize, WHITE); //Test Image Read
    display.display();
  }
  fileBMP.close();
  delay(500);

  Serial.print("ESP.getFreeHeap():"); Serial.println(ESP.getFreeHeap());


  //Neco-Arc_bitmapimg.txt
  fileBMP = SPIFFS.open("/Neco-Arc_bitmapimg.txt");
  if (!fileBMP) {
    Serial.println("Failed to open file for reading");
    return;
  }
  Serial.println("File Content:");
  while (fileBMP.available()) {
    char filereadBuffer[2048];
    char fileImageBuffer[2048];
    memset(filereadBuffer, '\0', 2048);
    memset(fileImageBuffer, '\0', 2048);
    fileBMP.readBytesUntil('\n', filereadBuffer, 2048);
    fileBMP.readBytesUntil('\n', fileImageBuffer, 2048);
    //Serial.write(filereadBuffer, strlen(filereadBuffer));
    //Serial.println();
    //Serial.write(fileImageBuffer, strlen(fileImageBuffer));
    //Serial.println();
    display.clearDisplay();
    display.drawBitmap(0, 0, convertTextHextoCharArray( fileImageBuffer ), 32, 56, WHITE);//Test Image Read
    display.display();
  }
  fileBMP.close();
  delay(500);

  Serial.print("ESP.getFreeHeap():"); Serial.println(ESP.getFreeHeap());


  //"/boyKisserFaceGif_bitmapgif.txt"
  fileBMP = SPIFFS.open("/boyKisserFaceGif_bitmapgif.txt");
  if (!fileBMP) {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.println("File Content:");
  while (fileBMP.available()) {
    char filereadBuffer[2048];
    char fileImageBuffer[2048];
    memset(filereadBuffer, '\0', 2048);
    memset(fileImageBuffer, '\0', 2048);
    fileBMP.readBytesUntil('\n', filereadBuffer, 2048);
    fileBMP.readBytesUntil('\n', fileImageBuffer, 2048);
    //Serial.write(filereadBuffer, strlen(filereadBuffer));
    //Serial.println();
    //Serial.write(fileImageBuffer, strlen(fileImageBuffer));
    //Serial.println();
    display.clearDisplay();
    display.drawBitmap(0, 0, convertTextHextoCharArray( fileImageBuffer ), 85, 64, WHITE);//Test Image Read
    ///*
    unsigned char* rotateImageTest;
    rotateImageTest = (unsigned char *) calloc(4 * 32, sizeof(unsigned char *)); //32*32
    bitmapIMGmanipulate0.rotateBitImageClockwise( rotateImageTest, (unsigned char*)rotateImageTestIcon, 32, 32);//rotates 90 deg clockwise //new Px Width and Height will be flipped
    display.drawBitmap(90, 0, rotateImageTest, 32, 32, WHITE);//Test Image Read
    //*/
    display.display();
    Serial.println("Displayed");
    //delay(2000);
    ///*
    unsigned char* betterRotateImageTest;
    betterRotateImageTest = (unsigned char *) calloc(16 * 128, sizeof(unsigned char *)); //85*64
    Serial.println("CheckPoint");
    bitmapIMGmanipulate0.rotateBitImageClockwise( betterRotateImageTest, convertTextHextoCharArray( fileImageBuffer ), 85, 64);//rotates 90 deg clockwise //new Px Width and Height will be flipped
    display.clearDisplay();
    display.drawBitmap(0, 0, betterRotateImageTest, 64, 85, WHITE);//Test Image Read
    display.display();
    free(betterRotateImageTest);
    //*/
    //delay(2000);
  }
  fileBMP.close();

  Serial.print("ESP.getFreeHeap():"); Serial.println(ESP.getFreeHeap());

  display.clearDisplay();

  //WiFi Setup
  WiFi.onEvent(OnWiFiEvent);
  
  //https://www.upesy.com/blogs/tutorials/how-to-connect-wifi-acces-point-with-esp32
  disableWiFi();
  
  enableWiFi();
  //wifiSerialSetup();


  //WiFi Setup Complete

#ifdef _TEST_ADAFRUIT_SH1106
  setupTest(disptestBitFramelay);
#endif

  // Should load default config if run for the first time
  Serial.println(F("Loading configuration..."));
  loadConfiguration(filename, config);

  // Create configuration file
  Serial.println(F("Saving configuration..."));
  saveConfiguration(filename, config);

  // Dump config file
  Serial.println(F("Print config file..."));
  printFile(filename);
  
  
  // Should load default config if run for the first time
  Serial.println(F("Loading WiFi configuration..."));
  loadWiFiConfiguration(filenameWiFi, wificonfig);

  // Create configuration file
  Serial.println(F("Saving WiFi configuration..."));
  saveWiFiConfiguration(filenameWiFi, wificonfig);

  // Dump config file
  Serial.println(F("Print WiFi config file..."));
  printWiFiFile(filenameWiFi);

  /*
   * Task Handles
   */
  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    1000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    1000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    2,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 1 */
  delay(500);
  
  Serial.print("ESP.getFreeHeap():"); Serial.println(ESP.getFreeHeap());
}



void Task1code( void * pvParameters ){
  Serial.print("Task1 running on core ");Serial.println(xPortGetCoreID());
  for (;;){
    delay(1000);
  }
}

void Task2code( void * pvParameters ){
  Serial.print("Task2 running on core ");Serial.println(xPortGetCoreID());
  for (;;){
    delay(2000);
  }
}





void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("loop() running on core ");Serial.println(xPortGetCoreID());
  //calculate FPS
  system_Frame_FPS = (double)(1 / ((double)(frameEndTime - frameStartTime) / 1000000));
  //\/ start Frame Here \/

  frameStartTime = micros();

  ///*
  #if (FeatureEnable_IR_RXTX_Control==true)
  if (IrReceiver.decode()) {
    Serial.print("IR_Receiver:");
    Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX); // Print "old" raw data
    // USE NEW 3.x FUNCTIONS
    IrReceiver.printIRResultShort(&Serial); // Print complete received data in one line
    IrReceiver.printIRSendUsage(&Serial);   // Print the statement required to send this data

    IrReceiver.resume(); // Enable receiving of the next value
  }
  #endif
  //*/




  display.clearDisplay();
  display.setCursor(0, 0);
  if (disableButtonPISO_update == false) {
    buttonpiso1.update();
  };
  printButtonPISODebug();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.print("RawButtons:"); display.println(buttonpiso1.getRAWState(), BIN);
  /*
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println("frameCountVariable:");
    display.print("->");display.println(frameCountVariable, DEC);
    //*/
  if (showDebugMenu == true) {
#if (FeatureEnable_BT_Serial==true)
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.print("BTserial.avail:"); display.println(SerialBT.available(), DEC);
#endif
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.print("Serial.avail:"); display.println(Serial.available(), DEC);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.print("CurrentMenuID:"); display.println(currentMenuID, DEC);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.print("Wifi_Status:"); display.println("UNKNOWN");
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.print("LoRa_Status:"); display.println("UNKNOWN");
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.print("Time_Status:"); display.println("UNKNOWN");
    //display.setTextSize(1);
    //display.setTextColor(WHITE);
    //display.print("tamagotchi_Status:"); display.println("UNKNOWN");
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.print("DeBug_Status:"); display.println(showDebugMenu);
  }


  //main Menu //currentMenuID==0
  if (currentMenuID == 0) {
    //
    display.drawRoundRect(0, 2 - 8, 128, 16 - 2 + 8, 4, WHITE);
    display.fillRect(0, 0, 48 + 8, 16, BLACK);
    display.drawLine(0, 7, 47, 7, WHITE);
    display.drawLine(47, 7, 47 + 8, 7 + 8, WHITE);
    display.drawBitmap(batteryXpos, batteryYpos, batteryIconBigImage, batteryIconBigWidth, batteryIconBigHeight, WHITE);
    display.fillRect(batteryXpos + 4, batteryYpos + 2, (double)((100 - batteryPercent) * (20 / 100)), 8, BLACK);
    //batteryPercent, chargingFlag
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.print("FPS~:"); display.print( system_Frame_FPS ); display.println();
    display.print("RawButtons:"); display.println(buttonpiso1.getRAWState(), BIN);
    display.print("battery%:"); display.print(batteryPercent); display.print(", "); display.print(batteryVoltage); display.println("V");
    display.print("ShowingVCB_frame:"); display.println(frameCountVariable % 32);
    display.drawBitmap(0, 8, visableCharacterBuffer[ frameCountVariable % 32 ], 32, 56, WHITE);

  }
  //selection menu //currentMenuID==1
  if (currentMenuID == 1) {
    int buttonDrawCount = 4;
    int buttonDrawWidth = display.width() - 2;
    int buttonDrawHeight = 16;//display.height()/4;
    int optionNameID = 0;
    char printOptionsBuffer[33];
    memset(printOptionsBuffer, '\0', 33);
    static int oN_IDDisplay_offset;

    if (buttonpiso1.isTapped(0) == true) {//UP
      sm_selectedID--;
      if (sm_selectedID + 1 == oN_IDDisplay_offset) {
        oN_IDDisplay_offset = sm_selectedID;
      };
    };
    if (buttonpiso1.isTapped(5) == true) {//DOWN
      sm_selectedID++;
      if ((sm_selectedID - oN_IDDisplay_offset) == buttonDrawCount) {
        oN_IDDisplay_offset = sm_selectedID - (buttonDrawCount - 1);
      };
    };
    if (buttonpiso1.isTapped(2) == true) {//Select
      currentMenuID = sm_selectedID;
    };



    for (int buttonDrawIndex = 0; buttonDrawIndex < buttonDrawCount; buttonDrawIndex++) {
      optionNameID = oN_IDDisplay_offset + buttonDrawIndex;
      display.setCursor(4, 4 + buttonDrawHeight * buttonDrawIndex);
      display.setTextSize(1.5);
      if ((sm_selectedID) != optionNameID) {
        display.drawRoundRect(0, buttonDrawHeight * buttonDrawIndex, buttonDrawWidth, buttonDrawHeight - 2, buttonDrawHeight / 4, WHITE);
        display.setTextColor(WHITE);
      } else {
        display.fillRoundRect(0, buttonDrawHeight * buttonDrawIndex, buttonDrawWidth, buttonDrawHeight - 2, buttonDrawHeight / 4, WHITE);
        display.setTextColor(BLACK);
      };
      strcpy(printOptionsBuffer, smOptionName_List[ optionNameID ]);
      display.print("Opt "); display.print(optionNameID); display.print(":"); display.println(printOptionsBuffer);
    }
  }
  //Lora menu //currentMenuID==2
  if (currentMenuID == 2) {
    display.setCursor(0, 1 * 8);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.print("LoRa Menu"); display.println();
    // If something available
    if (e220ttl.available() > 1) {
      Serial.println("Message received!");
      display.println("Message received!");

      // read the String message
#ifdef ENABLE_RSSI
      ResponseContainer rc = e220ttl.receiveMessageRSSI();
#else
      ResponseContainer rc = e220ttl.receiveMessage();
#endif
      // Is something goes wrong print error
      if (rc.status.code != 1) {
        Serial.println(rc.status.getResponseDescription());
        display.println(rc.status.getResponseDescription());
      } else {
        // Print the data received
        Serial.println(rc.status.getResponseDescription());
        Serial.println(rc.data);
        display.println(rc.status.getResponseDescription());
        display.println(rc.data);
#ifdef ENABLE_RSSI
        Serial.print("RSSI: "); Serial.println(rc.rssi, DEC);
        display.print("RSSI: "); display.println(rc.rssi, DEC);
#endif
      }
    }
    display.setCursor(0, 5 * 8);
  }


  //Wifi menu //currentMenuID==3
  if (currentMenuID == 3) {
    //https://randomnerdtutorials.com/esp32-useful-wi-fi-functions-arduino/
    display.setCursor(0, 1 * 8);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.print("WiFi Menu"); display.println();
    if (buttonpiso1.isTapped(3) == true) {//A
      if (WiFi.getMode() == WIFI_MODE_NULL) {
        enableWiFi();
        //wifiSerialSetup();
      }else{
        disableWiFi();
      }
    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
    }
    if (WiFi.status() != WL_CONNECTED) {
      if (WiFi.getMode() != WIFI_MODE_NULL) {
        int n;
        int showOnScreenConnection = 7;
        if ((millis() - lastScanTimer) > 60000) {
          n = WiFi.scanNetworks();
          lastScanTimer = millis();
        }
        display.print("Connections: "); display.println(n);
        for (int index = 0; index < n; index++) {
          display.print(WiFi.SSID(index)); display.print(" RRSI: "); display.println(WiFi.RSSI(index));
        }
      }
    }
    
    
    Serial.print("WiFi.getMode():");Serial.print(WiFi.getMode());Serial.print(", ");
    if (WiFi.getMode() == WIFI_MODE_NULL) {
      Serial.println("WiFi.getMode() == WIFI_MODE_NULL");
    };
    if (WiFi.getMode() == WIFI_MODE_STA) {
      Serial.println("WiFi.getMode() == WIFI_MODE_STA");
    };
    if (WiFi.getMode() == WIFI_MODE_AP) {
      Serial.println("WiFi.getMode() == WIFI_MODE_AP");
    };
    if (WiFi.getMode() == WIFI_MODE_APSTA) {
      Serial.println("WiFi.getMode() == WIFI_MODE_APSTA");
    };
    if (WiFi.getMode() == WIFI_MODE_MAX) {
      Serial.println("WiFi.getMode() == WIFI_MODE_MAX");
    };
    
    if (WiFi.getMode() == WIFI_MODE_NULL) {
      display.println("WiFi.getMode() == WIFI_MODE_NULL");
    };
    if (WiFi.getMode() == WIFI_MODE_STA) {
      display.println("WiFi.getMode() == WIFI_MODE_STA");
    };
    if (WiFi.getMode() == WIFI_MODE_AP) {
      display.println("WiFi.getMode() == WIFI_MODE_AP");
    };
    if (WiFi.getMode() == WIFI_MODE_APSTA) {
      display.println("WiFi.getMode() == WIFI_MODE_APSTA");
    };
    if (WiFi.getMode() == WIFI_MODE_MAX) {
      display.println("WiFi.getMode() == WIFI_MODE_MAX");
    };


  }

  //IR menu //currentMenuID==5
  if (currentMenuID == 5) {
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.print("IR Menu"); display.println();
    display.print("Functionality Not yet added!"); display.println();
    
  }



  //SPIFFs menu //currentMenuID==8
  if (currentMenuID == 8) {
    //https://techtutorialsx.com/2019/02/23/esp32-arduino-list-all-files-in-the-spiffs-file-system/
    display.setCursor(0, 1 * 8);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.print("SPIFFs Menu"); display.println();
    unsigned int fileCount = 0;
    int fileSelect = 0;
    unsigned int fileDisplayOffset = 0;
    unsigned int fileDisplayOffsetMax = 7;
    unsigned int currentNameSaveCount = 8;
    unsigned int extendNameSaveCountSize = 4;
    unsigned int maxFileNameLength = 32;
    char *savedFileNames;
    char *savedFileNamesExtendBuffer;
    savedFileNames = (char *)malloc( currentNameSaveCount * maxFileNameLength );
    memset(savedFileNames, '\0', currentNameSaveCount * maxFileNameLength);
    Serial.println("CheckPoint-00");
    File root = SPIFFS.open("/");
    Serial.println("CheckPoint-01");
    File file = root.openNextFile();
    Serial.println("CheckPoint-02");
    while (file) {
      //extend Main List Length
      Serial.println("Extending File Name List!");
      Serial.print("ESP.getFreeHeap():"); Serial.println(ESP.getFreeHeap());
      if (fileCount > currentNameSaveCount) {
        //allocate buffer
        //copy to buffer
        //extend main list
        //add items back to Main List
        //free up allocated buffer
        Serial.println("CheckPoint-03");
        savedFileNamesExtendBuffer = (char *)malloc( currentNameSaveCount * maxFileNameLength );
        Serial.println("CheckPoint-04");
        memcpy(savedFileNamesExtendBuffer, savedFileNames, currentNameSaveCount * maxFileNameLength);
        Serial.println("CheckPoint-05");
        free(savedFileNames);
        Serial.println("CheckPoint-06");
        /*
          for (int index = 0; index < currentNameSaveCount; index++){
          savedFileNamesExtendBuffer[ index ] = savedFileNames[ index ];
          }
          //*/
        currentNameSaveCount += extendNameSaveCountSize;
        savedFileNames = (char *)malloc( currentNameSaveCount * maxFileNameLength );
        Serial.println("CheckPoint-07");
        memset(savedFileNames, '\0', currentNameSaveCount * maxFileNameLength);
        Serial.println("CheckPoint-08");
        for (int index = 0; index < (currentNameSaveCount - extendNameSaveCountSize); index++) {
          savedFileNames[ index ] = savedFileNamesExtendBuffer[ index ];
        }
        Serial.println("CheckPoint-09");
        free(savedFileNamesExtendBuffer);//frees Memory
        Serial.println("CheckPoint-0a");
      }
      Serial.print("ESP.getFreeHeap():"); Serial.println(ESP.getFreeHeap());
      Serial.println("CheckPoint-0b");
      //add to main List
      strcpy(&savedFileNames[ fileCount * maxFileNameLength ], file.name()); //savedFileNames[ fileCount ] = file.name();
      Serial.println("CheckPoint-0c");
      Serial.print("FILE: ");
      Serial.println(file.name());

      fileCount++;
      file = root.openNextFile();
      Serial.println("CheckPoint-0d");
    }
    Serial.println("CheckPoint-0e");
    file.close();
    root.close();
    while (currentMenuID == 8) {
      Serial.println("CheckPoint-0f");
      display.clearDisplay();
      display.setCursor(0, 0);
      display.setTextSize(1);
      display.setTextColor(WHITE);

      if (disableButtonPISO_update == false) {
        buttonpiso1.update();
      };
      printButtonPISODebug();


      if (buttonpiso1.isTapped(0) == true) {//UP
        fileSelect--;
        if (fileSelect + 1 <= fileDisplayOffset) {
          fileDisplayOffset = fileSelect;
        }
      }
      if (buttonpiso1.isTapped(5) == true) {//DOWN
        fileSelect++;
        if ((fileSelect - fileDisplayOffset) >= fileDisplayOffsetMax) {
          fileDisplayOffset = fileSelect - (fileDisplayOffsetMax - 1);
        }
      }
      display.println(fileSelect);
      char printBuffer[32];
      for (int index = 0; index < fileDisplayOffsetMax; index++) {
        //
        display.setCursor(0, 8 * index);
        strncpy(printBuffer, &savedFileNames[ (fileDisplayOffset + index)*maxFileNameLength ], maxFileNameLength);
        if ((fileDisplayOffset + index) == fileSelect) {
          display.fillRect(0, 8 * (fileSelect - fileDisplayOffset), display.width(), 8, WHITE);
          display.setTextColor(BLACK);
          display.println(printBuffer);
        } else {
          display.setTextColor(WHITE);
          display.println(printBuffer);
        }
      }
      display.display();
      //
      if (buttonpiso1.isTapped(7) == true) {//B
        currentMenuID = 0;//previousMenuID;
        break;
      };
    }
    Serial.println("CheckPoint-10");
    //free(savedFileNames);//frees Memory
    Serial.println("CheckPoint-11");
  }



  if (buttonpiso1.isPressed(2) == true) {//Select
    frame_location_offset --; //frameCountVariable%10;
    if (frame_location_offset < (-0)) {
      frame_location_offset = (-0);
    };
    drawTaskbar(display, frame_location_offset, taskbarRotation);
    display.setCursor(4, 52);
    display.setTextSize(1.5);
    display.setTextColor(WHITE);
    display.println("TaskBarDrawn:");
  } else {
    frame_location_offset ++;
    if (frame_location_offset > (9)) {
      frame_location_offset = (9);
    };
    if (frame_location_offset < (9)) {
      drawTaskbar(display, frame_location_offset, taskbarRotation);
      display.setCursor(4, 52);
      display.setTextSize(1.5);
      display.setTextColor(WHITE);
      display.println("TaskBarDrawn:");
    }
  };
  if (buttonpiso1.isTapped(1) == true) {//start
    if (currentMenuID == 0) {
      previousMenuID = currentMenuID;
      currentMenuID = 1;
    } else if (currentMenuID == 1) {
      previousMenuID = currentMenuID;
      currentMenuID = 0;
    } else {
      previousMenuID = currentMenuID;
      currentMenuID = 1;
    }
  }
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.print("currentMenuID:"); display.println(currentMenuID);

  //display.print("CPU Freq: ");display.println(getCpuFrequencyMhz());





  


  //checkPoint
  //ToDo
  //make a file Viewer
  //make Functions for LoRa Data
  //WiFi
  //Buzzer
  //IR
  //Sensors
  //UI


  //Make Serial Debug command into Function in order for usage with both SerialBT and Serial
  serialDebugCommands(Serial);
#if (FeatureEnable_BT_Serial==true)
  serialDebugCommands(SerialBT);
#endif
  
  // listen for incoming clients
  if (!SerialWiFiclient.connected()) {
    SerialWiFiclient = SerialWiFiserver.available();
  }
  Serial.print("Client Connected:");Serial.print(SerialWiFiclient.connected());Serial.print(" Client available:");Serial.print(SerialWiFiclient.available());Serial.println();
  
  if (SerialWiFiclient) {
    if (SerialWiFiclient.connected()) {
      Serial.println("Connected to Serial client");
      serialDebugCommands(SerialWiFiclient);
    }
    // close the connection:
    //client.stop();
  }
  
  if (WiFi.getMode() != WIFI_MODE_NULL) {
    if (WiFi.getMode() != WIFI_MODE_AP) {
      Serial.print("Local IP address: ");Serial.println(WiFi.localIP());
    };
    if (WiFi.getMode() != WIFI_MODE_STA) {
      Serial.print("SoftAP IP address: ");Serial.println(WiFi.softAPIP());
    };
  }
  


  //Calculate Battery Percentage

  double vin = calculateVoltageDivider(100000, 100000, batteryVoltageDivider);
  double maxVin = 4.1;
  double minVin = 3.3;
  batteryPercent = (byte)mapdouble((double)vin, (double)minVin, (double)maxVin, (double)0.0, (double)100.0);
  batteryVoltage = vin;
  chargingFlag = false;
  //Serial.print("batteryPercent%:");Serial.println(batteryPercent);

  
  //Serial.print("ESP.getFreeHeap():");Serial.println(ESP.getFreeHeap());
  //display.print("ESP.getFreeHeap():"); display.println(ESP.getFreeHeap());
  printFreeHeap(Serial);
  display.print("FreeHeap:"); display.print(ESP.getFreeHeap()); display.print(" B"); display.println();
  

  //update some variables


  //update Frame
  display.display();
  //update frame count variable
  frameCountVariable++;
  //update frame End Time
  frameEndTime = micros();
}


void printFreeHeap(Stream &serialport) {
  serialport.print("ESP.getFreeHeap():");
  serialport.println(ESP.getFreeHeap());
}


#define MAXDEBUG_TARGETCOMMAND_LENGTH 64
#define MAXDEBUG_COMMANDINPUT_LENGTH 256
#define MAXDEBUG_SUBTARGETCOMMAND_LENGTH 256
#define MAXDEBUG_SUBCOMMANDINPUT_LENGTH 256
class DebugCommands {
  private:
    //
    
  public:
    DebugCommands();
    void init();
    void DebugCommands::serialDebugCommands(Stream &serialport);
    
};
DebugCommands::DebugCommands() {
  init();
}
void DebugCommands::init() {
  //init Buffers
  char targetCommand[ MAXDEBUG_TARGETCOMMAND_LENGTH ];
  char commandInputs[ MAXDEBUG_COMMANDINPUT_LENGTH ];
  char subTargetCommand[ MAXDEBUG_SUBTARGETCOMMAND_LENGTH ];
  char subCommandInputs[ MAXDEBUG_SUBCOMMANDINPUT_LENGTH ];
}
void DebugCommands::serialDebugCommands(Stream &serialport) {
  //
}



void serialDebugCommands(Stream &serialport)
{
  //https://forum.arduino.cc/t/can-you-pass-a-serial-port-as-a-parameter/658755
  //get serial debug command!
  if (serialport.available() != 0) {
    String debugCommandString = serialport.readString();  //read until timeout
    debugCommandString.trim();// remove any \r \n whitespace at the end of the String
    char debugCommand[ debugCommandString.length() + 1 ]; //maxDebugCommandScan
    debugCommandString.toCharArray(debugCommand, debugCommandString.length() + 1);
    //https://docs.arduino.cc/built-in-examples/communication/ASCIITable

    //Decode serial debug command!
    int debugCommandLength = strlen(debugCommand);
    //init Buffers
    char targetCommand[ MAXDEBUG_TARGETCOMMAND_LENGTH ];
    char commandInputs[ MAXDEBUG_COMMANDINPUT_LENGTH ];
    char subTargetCommand[ MAXDEBUG_SUBTARGETCOMMAND_LENGTH ];
    char subCommandInputs[ MAXDEBUG_SUBCOMMANDINPUT_LENGTH ];
    //
    /*
    char cacheCommand_0[64];
    char cacheCommand_1[64];
    char valueCommandCheckCache_0[64];
    char valueCommandCheckCache_1[64];
    char valueCommandCheckCache_2[64];
    char valueCommandCheckCache_3[64];
    int intDebugValue_0;
    int intDebugValue_1;
    int intDebugValue_2;
    int intDebugValue_3;
    //*/
    
    printCharArrayValue(serialport, debugCommand, "Debug Command");
    
    serial_WiFi_DebugCommands(serialport, debugCommand);
    serial_LoRa_DebugCommands(serialport, debugCommand);
    serial_ButtonPISO_DebugCommands(serialport, debugCommand);
    
    if (commandSelect(debugCommand, "set ")) {
      strcpy(targetCommand, "set ");
      strcpy(commandInputs, &debugCommand[strlen(targetCommand)]);
      printCharArrayValue(serialport, commandInputs, "commandInputs");
      if (commandSelect(commandInputs, "taskbarRotation ")) {
        //command format: set taskbarRotation #
        //command format: set taskbarRotation hidden
        strcpy(subTargetCommand, "taskbarRotation ");
        strcpy(subCommandInputs, &commandInputs[strlen(subTargetCommand)]);
        printCharArrayValue(serialport, subCommandInputs, "subCommandInputs");
        //https://linuxhint.com/arduino-atoi-function/
        taskbarRotation = atoi(subCommandInputs);//outputs 0 if 0 or is not a valid number
        if (taskbarRotation == 0) {
          if (strcmp(subCommandInputs, "hidden") == 0) {
            taskbarRotation = 0;//taskbar is hidden
          }
        }
        taskbarRotation = taskbarRotation - 1;
        if (taskbarRotation < 0) {
          taskbarRotation = 4;//taskbar is hidden
        }
      }
      if (commandSelect(commandInputs, "chargingFlag ")) {
        //command format: set chargingFlag #
        strcpy(subTargetCommand, "chargingFlag ");
        strcpy(subCommandInputs, &commandInputs[strlen(subTargetCommand)]);
        printCharArrayValue(serialport, subCommandInputs, "subCommandInputs");
        chargingFlag = atoi(subCommandInputs);
      }
      if (commandSelect(commandInputs, "DebugMenu ")) {
        //command format: set DebugMenu hidden
        //command format: set DebugMenu show
        //command format: set DebugMenu invisible
        //command format: set DebugMenu visible
        strcpy(subTargetCommand, "DebugMenu ");
        strcpy(subCommandInputs, &commandInputs[strlen(subTargetCommand)]);
        printCharArrayValue(serialport, subCommandInputs, "subCommandInputs");
        
        if (strcmp(subCommandInputs, "hidden") == 0) {
          showDebugMenu = 0;
        }
        if (strcmp(subCommandInputs, "show") == 0) {
          showDebugMenu = 1;
        }
        if (strcmp(subCommandInputs, "invisible") == 0) {
          showDebugMenu = 0;
        }
        if (strcmp(subCommandInputs, "visible") == 0) {
          showDebugMenu = 1;
        }
        if (showDebugMenu == 0) {
          serialPrintMessageFrom("System_Debug", "hiding debug menu");
        }
        if (showDebugMenu == 1) {
          serialPrintMessageFrom("System_Debug", "showing debug menu");
        }
      }
      if (commandSelect(commandInputs, "MenuID ")) {
        //command format: set MenuID #
        strcpy(subTargetCommand, "MenuID ");
        strcpy(subCommandInputs, &commandInputs[strlen(subTargetCommand)]);
        printCharArrayValue(serialport, subCommandInputs, "subCommandInputs");
        currentMenuID = atoi(subCommandInputs);
      }
      
      #if (FeatureEnable_IR_RXTX_Control==true)
      //https://github.com/Arduino-IRremote/Arduino-IRremote/blob/master/examples/SendDemo/SendDemo.ino
      if (commandSelect(commandInputs, "IRSender ")) {
        //command format: set IRSender sendValue
        strcpy(subTargetCommand, "IRSender ");
        strcpy(subCommandInputs, &commandInputs[strlen(subTargetCommand)]);
        printCharArrayValue(serialport, subCommandInputs, "subCommandInputs");
        
        char *ptr;
        unsigned int sendValue;//0xBD3D00802002

        sendValue = strtoul(subCommandInputs, &ptr, 16);
        serialport.println(sendValue, HEX);
        //IrSender.sendRaw(aIRDataToSend->rawCode, aIRDataToSend->rawCodeLength, 38);
        //const uint16_t irSignal[] = 0xBD3D00802002;//Power Button
        //Protocol=Panasonic Address=0x8 Command=0x3D Repeat gap=74500us Raw-Data=0xBD3D00802002 48 bits LSB first
        //IrSender.sendRaw(irSignal, sizeof(irSignal) / sizeof(irSignal[0]), 38);

        IrSender.sendNECRaw(sendValue, 1);
      }
      #endif

      
      //
      
    }
    
  }
}


void serial_ButtonPISO_DebugCommands(Stream &serialport, char *debugCommand)
{
  char targetCommand[ MAXDEBUG_TARGETCOMMAND_LENGTH ];
  char commandInputs[ MAXDEBUG_COMMANDINPUT_LENGTH ];
  char subTargetCommand[ MAXDEBUG_SUBTARGETCOMMAND_LENGTH ];
  char subCommandInputs[ MAXDEBUG_SUBCOMMANDINPUT_LENGTH ];
  
  char cacheCommand_0[64];
  char cacheCommand_1[64];
  char valueCommandCheckCache_0[64];
  char valueCommandCheckCache_1[64];
  char valueCommandCheckCache_2[64];
  char valueCommandCheckCache_3[64];
  int intDebugValue_0;
  int intDebugValue_1;
  int intDebugValue_2;
  int intDebugValue_3;
  
  printCharArrayValue(serialport, debugCommand, "Debug Command");
  
  if (commandSelect(debugCommand, "BPISO ")) {
    strcpy(targetCommand, "BPISO ");
    strcpy(commandInputs, &debugCommand[strlen(targetCommand)]);
    printCharArrayValue(serialport, commandInputs, "commandInputs");
    
    if (commandSelect(commandInputs, "auto_update ")) {
      //command format: BPISO auto_update enable
      //command format: BPISO auto_update disable
      strcpy(subTargetCommand, "ButtonPISO_update ");
      strcpy(subCommandInputs, &commandInputs[strlen(subTargetCommand)]);
      printCharArrayValue(serialport, subCommandInputs, "subCommandInputs");
      
      if (strcmp(subCommandInputs, "enable") == 0) {
        disableButtonPISO_update = false;
        serialPrintMessageFrom("System_Debug", "enabled ButtonPISO update");
      } else if (strcmp(subCommandInputs, "disable") == 0) {
        disableButtonPISO_update = true;
        serialPrintMessageFrom("System_Debug", "disabled ButtonPISO update");
      }
    }
    if (commandSelect(commandInputs, "setInput ")) {
      //command format: BPISO setInput buttonID/buttonName LOW
      //command format: BPISO setInput buttonID/buttonName HIGH
      strcpy(subTargetCommand, "ButtonPISO ");
      strcpy(subCommandInputs, &commandInputs[strlen(subTargetCommand)]);
      printCharArrayValue(serialport, subCommandInputs, "subCommandInputs");
      for (uint8_t buttonIndex = 0; buttonIndex < buttonPISOcount; buttonIndex++) {
        strcpy(valueCommandCheckCache_0, buttonNameString[buttonIndex]);
        strcat(valueCommandCheckCache_0, " ");
        itoa(buttonIndex, valueCommandCheckCache_1, 10);
        strcat(valueCommandCheckCache_1, " ");
        if (commandSelect(subCommandInputs, valueCommandCheckCache_0)) {
          intDebugValue_0 = buttonIndex;
          strcpy(cacheCommand_0, &subCommandInputs[strlen(valueCommandCheckCache_0)]);
          serialport.println("Name");
        } else if (commandSelect(subCommandInputs, valueCommandCheckCache_1)) {
          intDebugValue_0 = buttonIndex;
          strcpy(cacheCommand_0, &subCommandInputs[strlen(valueCommandCheckCache_1)]);
          serialport.println("ID");
        } else {
          continue;
        }
        serialport.println(cacheCommand_0);
        if (strcmp(cacheCommand_0, "HIGH") == 0) {
          buttonpiso1.overrideButtonState(intDebugValue_0, HIGH);
          serialPrintMessageFrom("System_Debug", "ButtonPISO State Changed");
        }
        if (strcmp(cacheCommand_0, "LOW") == 0) {
          buttonpiso1.overrideButtonState(intDebugValue_0, LOW);
          serialPrintMessageFrom("System_Debug", "ButtonPISO State Changed");
        }
      }
    }
    //
    
  }
}


void serial_LoRa_DebugCommands(Stream &serialport, char *debugCommand)
{
  char targetCommand[ MAXDEBUG_TARGETCOMMAND_LENGTH ];
  char commandInputs[ MAXDEBUG_COMMANDINPUT_LENGTH ];
  char subTargetCommand[ MAXDEBUG_SUBTARGETCOMMAND_LENGTH ];
  char subCommandInputs[ MAXDEBUG_SUBCOMMANDINPUT_LENGTH ];
  printCharArrayValue(serialport, debugCommand, "Debug Command");
  if (commandSelect(debugCommand, "control LoRa ")) {
    strcpy(targetCommand, "control LoRa ");
    strcpy(commandInputs, &debugCommand[strlen(targetCommand)]);
    printCharArrayValue(serialport, commandInputs, "commandInputs");
    //
  }
}

void serial_WiFi_DebugCommands(Stream &serialport, char *debugCommand)
{
  char targetCommand[ MAXDEBUG_TARGETCOMMAND_LENGTH ];
  char commandInputs[ MAXDEBUG_COMMANDINPUT_LENGTH ];
  char subTargetCommand[ MAXDEBUG_SUBTARGETCOMMAND_LENGTH ];
  char subCommandInputs[ MAXDEBUG_SUBCOMMANDINPUT_LENGTH ];
  printCharArrayValue(serialport, debugCommand, "Debug Command");
  if (commandSelect(debugCommand, "control WiFi ")) {
    strcpy(targetCommand, "control WiFi ");
    strcpy(commandInputs, &debugCommand[strlen(targetCommand)]);
    printCharArrayValue(serialport, commandInputs, "commandInputs");
    //https://www.mischianti.org/2021/03/06/esp32-practical-power-saving-manage-wifi-and-cpu-1/
    //https://randomnerdtutorials.com/esp32-useful-wi-fi-functions-arduino/
    //https://espressif-docs.readthedocs-hosted.com/projects/arduino-esp32/en/latest/api/wifi.html
    //https://reference.arduino.cc/reference/en/libraries/wifi/
    //https://randomnerdtutorials.com/esp32-useful-wi-fi-functions-arduino/
    if (commandSelect(commandInputs, "scanNetworks")) {
      //command format: control WiFi scanNetworks
      strcpy(subTargetCommand, "scanNetworks");
      strcpy(subCommandInputs, &commandInputs[strlen(subTargetCommand)]);
      printCharArrayValue(serialport, subCommandInputs, "subCommandInputs");
      int n = WiFi.scanNetworks();
      serialport.print("Connections: "); serialport.println(n);
      for (int index = 0; index < n; index++) {
        serialport.print(WiFi.SSID(index)); serialport.print(" RRSI: "); serialport.println(WiFi.RSSI(index));
      }
    }
    if (commandSelect(commandInputs, "showModes ")) {
      //command format: control WiFi showMode
      strcpy(subTargetCommand, "showMode ");
      strcpy(subCommandInputs, &commandInputs[strlen(subTargetCommand)]);
      printCharArrayValue(serialport, subCommandInputs, "subCommandInputs");
      serialport.println("WiFi.mode(WIFI_STA)  station mode: the ESP32 connects to an access point");
      serialport.println("WiFi.mode(WIFI_AP)  access point mode: stations can connect to the ESP32");
      serialport.println("WiFi.mode(WIFI_AP_STA)  access point and a station connected to another access point");
    }
    if (commandSelect(commandInputs, "setMode ")) {
      //command format: control WiFi setMode WIFI_STA
      //command format: control WiFi setMode WIFI_AP
      //command format: control WiFi setMode WIFI_AP_STA
      strcpy(subTargetCommand, "setMode ");
      strcpy(subCommandInputs, &commandInputs[strlen(subTargetCommand)]);
      printCharArrayValue(serialport, subCommandInputs, "subCommandInputs");
      if (strcmp(subCommandInputs, "WIFI_STA") == 0) {
        WiFi.mode(WIFI_STA);  //station mode: the ESP32 connects to an access point
      }
      if (strcmp(subCommandInputs, "WIFI_AP") == 0) {
        WiFi.mode(WIFI_AP);  //access point mode: stations can connect to the ESP32
      }
      if (strcmp(subCommandInputs, "WIFI_AP_STA") == 0) {
        WiFi.mode(WIFI_AP_STA);  //access point and a station connected to another access point
      }
    }
    if (commandSelect(commandInputs, "getMode ")) {
      //command format: control WiFi getMode
      strcpy(subTargetCommand, "getMode ");
      strcpy(subCommandInputs, &commandInputs[strlen(subTargetCommand)]);
      printCharArrayValue(serialport, subCommandInputs, "subCommandInputs");
      serialport.print("WiFi.getMode():");serialport.print(WiFi.getMode());serialport.print(", ");
      if (WiFi.getMode() == WIFI_MODE_NULL) {
        serialport.println("WiFi.getMode() == WIFI_MODE_NULL");
      };
      if (WiFi.getMode() == WIFI_MODE_STA) {
        serialport.println("WiFi.getMode() == WIFI_MODE_STA");
      };
      if (WiFi.getMode() == WIFI_MODE_AP) {
        serialport.println("WiFi.getMode() == WIFI_MODE_AP");
      };
      if (WiFi.getMode() == WIFI_MODE_APSTA) {
        serialport.println("WiFi.getMode() == WIFI_MODE_APSTA");
      };
      if (WiFi.getMode() == WIFI_MODE_MAX) {
        serialport.println("WiFi.getMode() == WIFI_MODE_MAX");
      };
    }
    if (commandSelect(commandInputs, "connect ")) {
      //command format: control WiFi connect "SSID" "PassWord"
      strcpy(subTargetCommand, "connect ");
      strcpy(subCommandInputs, &commandInputs[strlen(subTargetCommand)]);
      printCharArrayValue(serialport, subCommandInputs, "subCommandInputs");
      WiFi.disconnect(false);  // Reconnect the network
      char STA_SSID[33];
      char STA_PASS[64];
      //The SSID can be any alphanumeric, case-sensitive entry from 2 to 32 characters. The printable characters plus the space (ASCII 0x20) are allowed, but these six characters are not: ?, ", $, [, \, ], and +.
      //WEP - Maximum key length is 16 characters.
      //WPA-PSK/WPA2-PSK - Maximum key length is 63 characters.
      WiFi.begin(STA_SSID, STA_PASS);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        serialport.print(".");
      }
      serialport.println("");
      serialport.println("WiFi connected");
      serialport.println("IP address: ");
      serialport.println(WiFi.localIP());
      serialport.println("Wifi connected!");
    }
    if (commandSelect(commandInputs, "disconnect")) {
      //command format: control WiFi disconnect
      strcpy(subTargetCommand, "connect ");
      strcpy(subCommandInputs, &commandInputs[strlen(subTargetCommand)]);
      printCharArrayValue(serialport, subCommandInputs, "subCommandInputs");
      WiFi.disconnect(true);  // Disconnect from the network
      serialport.println("Wifi Disconnected!");
    }
  }
  
}


bool commandSelect(char* commandCheck, char* commandSelect) {
  return (strncmp(commandCheck, commandSelect, strlen(commandSelect)) == 0);
}

void serialPrintMessageFrom(char *messageSorce, char *message) {
  Serial.print(messageSorce);
  Serial.print(": ");
  Serial.print(message);
  Serial.println();
}


void printCharArrayValue(Stream &serialport, char *charArrayValue, char *charArrayName)
{
  //using this function when possible can save Program Space
  serialport.print(charArrayName);
  serialport.print(":");
  serialport.print(char(34));
  serialport.print(charArrayValue);
  serialport.print(char(34));
  serialport.println();
}








//https://www.mischianti.org/2021/03/06/esp32-practical-power-saving-manage-wifi-and-cpu-1/
void disableWiFi() {
  WiFi.disconnect(true);  // Disconnect from the network
  WiFi.mode(WIFI_OFF);    // Switch WiFi off
}

void enableWiFi() {
  WiFi.disconnect(false);  // Reconnect the network
  if (WiFi.getMode() == WIFI_MODE_NULL) {
    WiFi.mode(WIFI_STA);
  }else if (WiFi.getMode() == WIFI_MODE_AP){
    WiFi.mode(WIFI_AP_STA);
  }
  
  Serial.println("START WIFI");
  //WiFi.begin(STA_SSID, STA_PASS);
  wifi_status = WiFi.begin("TP-Link_0F3D", "Jerkface13597603");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("WiFi Failed!\n");
    return;
  }
  
  
  SerialWiFiserver.begin();
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


//https://techtutorialsx.com/2021/01/04/esp32-soft-ap-and-station-modes/
void disableWiFiSoftAP(){
  if (WiFi.getMode() == WIFI_MODE_AP) {
    WiFi.mode(WIFI_OFF);
  }else if (WiFi.getMode() == WIFI_MODE_APSTA){
    WiFi.mode(WIFI_STA);
  }
}

void enableWiFiSoftAP(char* soft_ap_ssid, char* soft_ap_password){
  if (WiFi.getMode() == WIFI_MODE_NULL) {
    WiFi.mode(WIFI_AP);
  }else if (WiFi.getMode() == WIFI_MODE_STA){
    WiFi.mode(WIFI_AP_STA);
  }
  WiFi.softAP(soft_ap_ssid, soft_ap_password);
}





void printButtonPISODebug() {
  //delay(100);
  if (buttonpiso1.isPressed(0) == true) {//UP
    Serial.println("Button 0 was Pressed!");
  };
  if (buttonpiso1.isPressed(1) == true) {//Start
    Serial.println("Button 1 was Pressed!");
  };
  if (buttonpiso1.isPressed(2) == true) {//Select
    Serial.println("Button 2 was Pressed!");
  };
  if (buttonpiso1.isPressed(3) == true) {//A
    Serial.println("Button 3 was Pressed!");
  };
  if (buttonpiso1.isPressed(4) == true) {//LEFT
    Serial.println("Button 4 was Pressed!");
  };
  if (buttonpiso1.isPressed(5) == true) {//DOWN
    Serial.println("Button 5 was Pressed!");
  };
  if (buttonpiso1.isPressed(6) == true) {//RIGHT
    Serial.println("Button 6 was Pressed!");
  };
  if (buttonpiso1.isPressed(7) == true) {//B
    Serial.println("Button 7 was Pressed!");
  };
}



double calculateVoltageDivider(unsigned long r1, unsigned long r2, unsigned int pinSelect)
{
  //voltage Divider
  //https://randomnerdtutorials.com/esp32-adc-analog-read-arduino-ide/
  //unsigned long r1 = 100000;//connects to Battery(vin)
  //unsigned long r2 = 100000;//connects to gnd
  unsigned int pinValue = analogRead(pinSelect);//0-4095//0V - 3.3V
  double resistors;
  double vout;
  double vin;
  vout = mapdouble((double)pinValue, (double)0, (double)4095, (double)0.0, (double)3.3);
  resistors = (double(r2) / double(r1 + r2));
  vin = vout / resistors;
  /*
    //vout=vin*(r2/(r1+r2))
    //vin=vout/(r2/(r1+r2))
  */
  /*
    Serial.print("pinValue:");Serial.println(pinValue);
    Serial.print("vout:");Serial.println(vout);
    Serial.print("vin:");Serial.println(vin);
    //*/
  return vin;
};


float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
double mapdouble(double x, double in_min, double in_max, double out_min, double out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}







unsigned char* convertTextHextoCharArray(char *inputHexText) {
  static unsigned char output[ 1024 ];
  memcpy(output, inputHexText, strlen(inputHexText) >> 1);
  unsigned int outputIndex = 0;
  for (uint16_t readIndex = 0; readIndex < strlen(inputHexText); readIndex += 2) {
    char hexChar[3];
    char *ptr;
    strncpy(hexChar, &inputHexText[readIndex], 2);
    hexChar[2] = '\0';
    output[ outputIndex ] = strtoul(hexChar, &ptr, 16);
    outputIndex++;
  }
  return output;
}



unsigned char* readTCAImgF(char *inputHexText) {
  //TCAImgF = Terrien's Custom Animated Image Format
  static unsigned char output[ 1024 ];
  unsigned char fheader[5 + 1] = "tcaif";
  unsigned char fversion[2 + 1] = "00";
  unsigned char spriteName[16 + 1] = "AmongUSisSUSTest";
  uint16_t totalFrameCount = 0;//(0-65535)-> (1-65536)
  uint8_t globalPixelWidth = 0;//(0-255)-> (1-256)
  uint8_t globalPixelHeight = 0;//(0-255)-> (1-256)
  uint8_t imageFormatFlags = B00000000;
  /*
    FileData Structure:
    5 bytes - file header
    2 bytes - file version
    16 bytes - in-program name
    2 bytes - Total Frame Count (1-65536)( tFcount )
    1 byte - Global Pixel Width (1-256)
    1 byte - Global Pixel Height (1-256)
    1 byte - image format flags (6 bools + 2bit value)
    ->B00000001 = Per-Frame Pixel Width&Height Enabled
    ->B00000010 = Per-Frame X&Y Pos Offset Enabled
    ->B00000100 = Per-Frame Delay Enabled
    ->B00001000 = Clear Area before each Frame Enabled
    ---> draws a black square at display location before drawing frame
    ->B00110000 = encoded image rotation
    ---> (Clockwise)( imgEncodeRot )
    ---> used if the encoded image needs to be rotated before stored in memory
    ->B01000000 = loop animation enabled
    ---> ( used if tFcount > 1 )
    ->B10000000 = inverted Image Data flag
    ---> if enabled flip each Pixel bit before output
    Per frame:
    1 byte - Pixel Width (if enabled)( fbitpxw )
    1 byte - Pixel Height (if enabled)( fbitpxh )
    1 byte - Image Xpos offset (if enabled)
    1 byte - Image Ypos offset (if enabled)
    2 bytes - hold Frame Delay before next draw (if enabled) (1-65536 milliseconds)
    2 bytes - Frame Data Bytes Count (fdBytec)
    fdBytec Bytes - Frame Pixel Data

  */

  return output;
}
