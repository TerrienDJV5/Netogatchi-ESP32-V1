//TeegyBigz and TerrienDJV5
//tamagotchi_V0.0.0 for Hardware V1
//ESP-Wroom-32

//please View "Helpful-URL-OwO.txt"



#include "FreeRTOSConfig.h" //https://www.freertos.org/a00106.html
#define INCLUDE_eTaskGetState true

#include "esp_attr.h" //https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/memory-types.html

/*#################################################*/
//Try This!
//#include "painlessMesh.h" //https://randomnerdtutorials.com/esp-mesh-esp32-esp8266-painlessmesh/

/*#################################################*/


//Define Simple Functions
#define ARRAY_LEN(array) ((sizeof(array))/(sizeof(array[0])))



/*
   Dictionary
   https://github.com/arkhipenko/Dictionary
*/

#define _DICT_CRC_ 32 //choices 16, 32, 64, etc

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
#define FeatureEnable_WiFiHTTP_Control true //Option not implemented (enabled or disabled)

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
#include <string>

#include "FS.h"
#include "SPIFFS.h"
//https://www.tutorialspoint.com/esp32_for_iot/esp32_for_iot_spiffs_storage.htm


//Json Support
//<ArduinoJson.h> By Benoit BLANCHON
#include <ArduinoJson.h> //https://arduinojson.org/





//WiFi Support
#include "WiFi.h"
#include <esp_wifi.h>
#include "WiFiType.h"
#include <WiFiMulti.h>
#include <SimpleFTPServer.h> //FTP


WiFiMulti wifiMulti;

//set #define FTP_DEBUG in ESP8266FtpServer.h to see ftp verbose on serial
FtpServer ftpSrv;

// WiFi connect timeout per AP. Increase when connecting takes longer.
const uint32_t connectTimeoutMs = 10000;

int wifi_status = WL_IDLE_STATUS;
WiFiServer SerialWiFiserver(23);

// Initialize the client library
WiFiClient SerialWiFiclient;





//other Stuff Support

#include "time.h"

#include "driver/adc.h"
#include "driver/dac.h"

#include <Preferences.h> 
Preferences preferences;



#include <SPI.h>
#include <Wire.h>



#if (FeatureEnable_SH1106_Display==true)
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include "Taskbar.h"



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
#define IR_RX_Pin 39 //GPIO 39
#define IR_SEND_PIN 27



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


#if defined(ESP32)
//#include <HardwareSerial.h>
//HardwareSerial ExtraSerial0(0);//ESP32Serial0
//HardwareSerial ExtraSerial1(1);//ESP32Serial1
//HardwareSerial ExtraSerial2(2);//ESP32Serial2

#endif





#include "LoRa_E220.h"
#define ENABLE_RSSI true
#define FREQUENCY_915
#define E220_22


//https://github.com/xreef/EByte_LoRa_E220_Series_Library


//LoRa_E220 e220ttl(&ExtraSerial2);  // e22 TX e22 RX
LoRa_E220 e220ttl(&Serial2);  // e22 TX e22 RX

//#include "EByte_LoRa_E220_library.h"



//Devices Setup Complete

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

//https://www.alt-codes.net/arrow_alt_codes.php
char* returnButtonName(byte buttonID, byte opMode = 2) {
  /* Using the string table in program memory requires the use of special functions to retrieve the data.
       The strcpy_P function copies a string from program space to a string in RAM ("buffer").
       Make sure your receiving string in RAM  is large enough to hold whatever
       you are retrieving from program space. */
  
  if (buttonID < 0){
    return NULL;
  }
  if (buttonID >= buttonPISOcount){
    return NULL;
  }
  if (buttonID >= 8){
    return NULL;
  }
  #define rBN_BASE false
  #define rBN_DPAD true
  byte buttonDPADorBASE_Mask = 0b01110001;//0 = BASE, 1 = DPAD //order = MSB //(76543210)
  //byte buttonDPAD_MaskIndex[4] = {0, 6, 5, 4};//up, Right, Down, Left
  //byte buttonBASE_MaskIndex[4] = {1, 2, 3, 7};//Start, Select, A, B
  byte buttonID_NibbleMaskIndex[8] = {0, 0, 1, 2, 3, 2, 1, 3};//up, Start, Select, A, Left, Down, Right, B
  char* buttonDPAD_Symbols[2][4+1] = {"↑→↓←", "▲►▼◄"};
  char buttonBASE_Symbols[4+1] = "+-AB";
  //char* button_Symbols[3][4+1] = {"↑→↓←", "▲►▼◄", "+-AB"};
  
  switch (opMode) {
  case 0:
  case 1:
    switch (bitRead(buttonDPADorBASE_Mask, buttonID)) {
      case rBN_BASE:
        return (char*)buttonBASE_Symbols[ buttonID_NibbleMaskIndex[ buttonID ] ];
        break;
      case rBN_DPAD:
        return buttonDPAD_Symbols[ opMode ][ buttonID_NibbleMaskIndex[ buttonID ] ];
        break;
      default:
        return NULL;
        break;
    }
    break;
  case 2:
    return buttonNameString[buttonID];
    break;
  default:
    return NULL;
    break;
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

/*
   Basic Struct Image
*/
//https://www.tutorialspoint.com/structs-in-arduino-program
//http://www.cs.ecu.edu/~karl/3300/spr14/Notes/C/Structure/constructor.html
typedef struct _IMGbitmapStructure{
  char imageName[32+1] = "IMAGE";//max name length = 32 charaters
  uint16_t imageByteLength = 0;
  uint16_t widthPx = 0;//in pixels
  uint16_t heightPx = 0;//in pixels
  unsigned char* pixelBitDataArray = NULL;//size is imageByteLength
  bool setPixel(unsigned int widthIndex, unsigned int heightIndex)
  {
    bool result = false;
    if ( (widthIndex < widthPx)&(heightIndex < heightPx) )
    {
      //set pixel
      result = true;
    }
    return result;
  }
  bool clearPixel(unsigned int widthIndex, unsigned int heightIndex)
  {
    bool result = false;
    if ( (widthIndex < widthPx)&(heightIndex < heightPx) )
    {
      //clear pixel
      result = true;
    }
    return result;
  }
  void allocate(unsigned int width, unsigned int height)
  {
    if ((void*)pixelBitDataArray!=NULL)//case if image Pointer is already allocated, if so use free on it too Reallocate.
    {
      free(pixelBitDataArray);
    }
    widthPx = width;
    heightPx = height;
    widthPadding = (8 - (widthPx % 8));
    imageByteLength = ((widthPx + widthPadding)>>3) * heightPx;
    pixelBitDataArray = new unsigned char[imageByteLength];
    autoAllocatedFlag = true;
    Serial.print(_funcMSG);
    Serial.printf("allocated %d bytes of memory at %p for image of size width (%d) and height (%d).\n",  imageByteLength, (void*)imageByteLength, widthPx, heightPx);
  }
  _IMGbitmapStructure(unsigned int width, unsigned int height, char name[32+1])
  {
    strncpy(imageName, name, sizeof(imageName));
    allocate(width, height);
  }
  _IMGbitmapStructure(unsigned int width, unsigned int height)
  {
    allocate(width, height);
  }
  _IMGbitmapStructure()
  {
    ;
  }
  ~_IMGbitmapStructure()
  {
    if ((void*)pixelBitDataArray != NULL){
      Serial.print(_funcMSG);
      Serial.printf("autoAllocatedFlag:(%d)---", autoAllocatedFlag);
      Serial.printf("deallocating %d bytes of memory at address (%p) for image of size width (%d) and height (%d).\n",  imageByteLength, (void*)pixelBitDataArray, widthPx, heightPx);
      delete [] pixelBitDataArray;
    }else{
      Serial.print(_funcMSG);
      Serial.printf("deallocating %d bytes of memory at address (%p)(NULL) for image of size width (%d) and height (%d).\n",  imageByteLength, (void*)pixelBitDataArray, widthPx, heightPx);
    }
    
  }
  private:
    uint16_t widthPadding = 0;//in bits
    bool autoAllocatedFlag = false;//true if allocated via Struct Allocation Function
    const char *_funcMSG = "-----STRUCT_FUNC_MSG(_IMGbitmapStructure)-----";
} IMGbitmapStruct;




//Debug Based
byte showDebugMenu = 0;//0 = no debug menu, 1= first debug menu, etc
bool disableButtonPISO_update = false;

//System Variables
unsigned int currentMenuID = 0;
unsigned int previousMenuID = 0;
unsigned long frameCountVariable;//This variable stores the number of frames that have been displayed since the program started.
IMGbitmapStruct batteryIconBig;
static unsigned char* batteryIconBigImage; //batteryBig_Full.bmp //SmallBatteryPercent.bmp
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
  "IP Menu",
  "Composite Video Test",
  "Op12",
  "Op13",
  "Op14",
  "Op15",
  "Op15",
  "Op16",
  "Op17",
  "Op18",
  "Op19",
  "Op20",
  "Op21",
  "Op22",
  "Op23",
  "Op24",
  "Op25",
  "Op26",
  "Op27",
  "Op28",
  "Op29",
  "Op30",
  "Op31",
};


//Wifi Menu //currentMenuID==3
static unsigned long lastScanTimer;


//SPIFFs menu //currentMenuID==8






//character variables
const unsigned int characterPixelWidth = 32;//4 bytes
const unsigned int characterPixelHeight = 56;
const unsigned int characterFrameByteSize = ((characterPixelWidth % 8 + characterPixelWidth) >> 3) * characterPixelHeight;
const unsigned int characterBufferFrameCount = 32;
//static unsigned char visableCharacterBuffer[ characterBufferFrameCount ][ characterFrameByteSize ];
IMGbitmapStruct visableCharacterBuffer[ characterBufferFrameCount ];

//Sprite Storage/Memory (_gMSB)(__GMSB__)
const unsigned int spritePagePixelWidth = 1024;
const unsigned int spritePagePixelHeight = 1024;
const byte spritePagePixelData_Bit_Length = 1;
#define CAL_BUFF_BYTE_SIZE__GMSB__(pixW, pixH, pixDataBitL) ( int(pixW/((8)/pixDataBitL))*pixH )//(PixelWidth, PixelHeight, PixelData_Bit_Length)
unsigned int calculateBufferByteSize_gMSB( unsigned int pixW, unsigned int pixH, byte pixDataBitL ){return ( int(pixW/((8)/pixDataBitL))*pixH );};


static unsigned char global_Master_Sprite_Buffer[ CAL_BUFF_BYTE_SIZE__GMSB__( spritePagePixelWidth, spritePagePixelHeight, spritePagePixelData_Bit_Length ) ];






//battery Variabls
static unsigned int batteryXpos = 99;
static unsigned int batteryYpos = 1;
static volatile bool autoUPdateBatteryVariables = true;//used for Debuging
static volatile byte batteryPercent = 100;//out of 100
static volatile double batteryVoltage = 0;
static volatile bool chargingFlag = false;

//cable Icon Variables
byte cableStatus = 0;

//user Settings
byte taskbarRotation = 1;//1
int frame_location_offset = 16;

//timeSettings
//time(), gmtime(), localtime(), mktime(), gettimeofday()
//https://lastminuteengineers.com/esp32-ntp-server-date-time-tutorial/
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = (-5)*60*60;
const int   daylightOffset_sec = 3600;
bool timeConfigured = false;


void printLocalTime(Stream &serialport){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    serialport.println("Failed to obtain time");
    return;
  }
  serialport.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  serialport.print("Day of week: ");
  serialport.println(&timeinfo, "%A");
  serialport.print("Month: ");
  serialport.println(&timeinfo, "%B");
  serialport.print("Day of Month: ");
  serialport.println(&timeinfo, "%d");
  serialport.print("Year: ");
  serialport.println(&timeinfo, "%Y");
  serialport.print("Hour: ");
  serialport.println(&timeinfo, "%H");
  serialport.print("Hour (12 hour format): ");
  serialport.println(&timeinfo, "%I");
  serialport.print("Minute: ");
  serialport.println(&timeinfo, "%M");
  serialport.print("Second: ");
  serialport.println(&timeinfo, "%S");

  serialport.println("Time variables");
  char timeHour[3];
  strftime(timeHour,3, "%H", &timeinfo);
  serialport.println(timeHour);
  char timeWeekDay[10];
  strftime(timeWeekDay,10, "%A", &timeinfo);
  serialport.println(timeWeekDay);
  serialport.println();
}



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

static const unsigned char PROGMEM wifi16x16Icon[] =
{
  B11111111, B11111111,
  B10000000, B00000001,
  B10000000, B00000001,
  B10000000, B00000001,
  B10000000, B00000001,
  B10000000, B00000001,
  B10000000, B00000001,
  B10000000, B00000001,
  B10000000, B00000001,
  B00000000, B00000000,
  B00000000, B00000000,
  B00011111, B11111000,
  B00111000, B00011100,
  B00110011, B11001100,
  B00000111, B11100000,
  B00000111, B11100000,
};

static const unsigned char PROGMEM LoRa16x16Icon[] =
{
  B11000000, B00000000,
  B11000000, B00000000,
  B11000000, B00000000,
  B11000000, B00000000,
  B11000000, B00000000,
  B11000000, B00000000,
  B11000000, B00000000,
  B11000000, B00000000,
  B11000000, B00000000,
  B11000000, B00000000,
  B11000000, B00000000,
  B11000000, B00000000,
  B11000000, B00000000,
  B11000000, B00000000,
  B11111111, B11111111,
  B11111111, B11111111,
};

/*

   End of Sprites Sheet and Tiles Sheet

*/








/*
 * showPrePrint (Can Be used anyWhere!)
 */
void showPrePrint(Stream &serialport, char* textInput){
  const unsigned int prePrintSize = 32;
  char *prePrintText;
  prePrintText = (char*)malloc(prePrintSize+1);
  memset(prePrintText, '\0', prePrintSize+1);
  if (strlen(textInput) >= prePrintSize){
    strncpy(prePrintText, textInput, prePrintSize);
  }else{
    strncpy(prePrintText, textInput, strlen(textInput));
  }
  serialport.print( prePrintText );
  free(prePrintText);
}



/*
 * WiFi Stuff Start
 */


IPAddress ip(192, 168, 0, 50);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

IPAddress local_IP(192, 168, 4, 22);
IPAddress local_gateway(192, 168, 4, 9);
IPAddress local_subnet(255, 255, 255, 0);


unsigned int countNewLinesFromFile(const char *filePath, fs::FS &fs){
  // Open file for reading
  File file = fs.open(filePath, FILE_READ);
  unsigned int newLineCount = 0;
  while(file.available()){
    newLineCount += (file.read()=='\n');
  }
  file.close();
  return newLineCount;
}


typedef struct {
  //six characters are not allowed (SSID): ?, ", $, [, \, ], and +
  char ssid[33];//max length 32 //Min Length 2
  char pass[64];//max length 63
} Credentials_WiFi_Struct;


typedef struct {
  unsigned int length;
  Credentials_WiFi_Struct *credentials;
} List_WiFi_Credentials_Struct;




//https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/wifi-security.html


const char *filenameWiFi = "/Wifi_Connections.json";
List_WiFi_Credentials_Struct wifiCredentialList;
//make Functions that convert Json to Structs


void disable_STA_WiFi();
void enable_STA_WiFi();




void loadWiFiConfigurationCharArray(char jsonChar[], size_t jsonCharSize, Credentials_WiFi_Struct &wifiCredentials);


// Loads the configuration from a charArray
void loadWiFiConfigurationCharArray(char jsonChar[], size_t jsonCharSize, Credentials_WiFi_Struct &wifiCredentials) {
  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use https://arduinojson.org/v6/assistant to compute the capacity.
  StaticJsonDocument<128> connection;

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(connection, jsonChar);
  if (error)
    Serial.println(F("Failed to read JsonCharArray, using default configuration"));

  // Copy values from the JsonDocument to the Config
  strlcpy(wifiCredentials.pass,                  // <- destination
          connection["pass"] | "",  // <- source
          sizeof(wifiCredentials.pass));         // <- destination's capacity
  strlcpy(wifiCredentials.ssid,                  // <- destination
          connection["ssid"] | "",  // <- source
          sizeof(wifiCredentials.ssid));         // <- destination's capacity
  ;
}


//https://jraleman.medium.com/c-programming-language-functions-malloc-calloc-realloc-and-free-61cfc3e45da7

void saveWiFicredentials(Credentials_WiFi_Struct &wifiCredentials);


//to make Reading and writeing faster checkt this out 
//https://github.com/bblanchon/ArduinoStreamUtils

bool ensureValid_Credentials(Credentials_WiFi_Struct &wifiCredential);
bool ensureValid_SSID(Credentials_WiFi_Struct &wifiCredential);
bool ensureValid_PASS(Credentials_WiFi_Struct &wifiCredential);
bool ensureValid_SSIDchar(char* inputSSID);//Make Private if put in a Class!
bool ensureValid_PASSchar(char* inputPASS);//Make Private if put in a Class!


void saveWiFiCredentialsList(const char *filename, const List_WiFi_Credentials_Struct &wifiCredentialList, fs::FS &fs);
void loadWiFiCredentialsList(const char *filename, List_WiFi_Credentials_Struct &wifiCredentialList, fs::FS &fs);
void saveWiFiCredentialToWiFiCredentialsList(List_WiFi_Credentials_Struct &wifiCredentialList, Credentials_WiFi_Struct &wifiCredential);
void remove_WiFiCredential_From_WiFiCredentialsList(List_WiFi_Credentials_Struct &wifiCredentialList, Credentials_WiFi_Struct &wifiCredential);



// Saves the CredentialsList to a file
void saveWiFiCredentialsList(const char *filename, const List_WiFi_Credentials_Struct &wifiCredentialList, fs::FS &fs) {
  Serial.println("File Will Be Saved!");
  File file;
  //fs.remove(filename);// Delete existing file, otherwise the configuration is appended to the file
  file = fs.open(filename, FILE_WRITE);// Open file for writing
  
  if (!file) {
    Serial.println(F("Failed to create file"));
    return;
  }
  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use https://arduinojson.org/assistant to compute the capacity.
  DynamicJsonDocument credentialList(128 * wifiCredentialList.length);
  credentialList[ "Length" ] = wifiCredentialList.length;
  for (int index = 0; index < wifiCredentialList.length; index++) {
    Serial.print("index: ");Serial.println(index);
    // Set the values in the document
    credentialList["info"][ index ]["ssid"] = wifiCredentialList.credentials[ index ].ssid;
    credentialList["info"][ index ]["pass"] = wifiCredentialList.credentials[ index ].pass;
    Serial.print("ssid: ");Serial.println(wifiCredentialList.credentials[ index ].ssid);
    Serial.print("pass: ");Serial.println(wifiCredentialList.credentials[ index ].pass);
  }
  serializeJson(credentialList, file);
  serializeJson(credentialList, Serial);
  Serial.println();
  
  // Close the file
  file.close();
}

// Loads the CredentialsList from a file
void loadWiFiCredentialsList(const char *filename, List_WiFi_Credentials_Struct &wifiCredentialList, fs::FS &fs) {
  File file;
  file = fs.open(filename, FILE_READ);// Open file for writing
  Serial.println("File Has Been Read");
  if (!file) {
    Serial.println(F("Failed to create file"));
    return;
  }
  //DynamicJsonDocument credentialList(file.available()*2);
  DynamicJsonDocument credentialList(file.size()*2);
  Serial.println("DynamicJsonDocument");
  //StaticJsonDocument<1024> credentialList;
  //DynamicJsonDocument credentialList(128 * wifiCredentialList.length);
  deserializeJson(credentialList, file);
  Serial.println("deserializeJson");
  
  
  wifiCredentialList.length = credentialList[ "Length" ];
  Serial.println("Length");
  wifiCredentialList.credentials = (Credentials_WiFi_Struct*)calloc(wifiCredentialList.length, sizeof(Credentials_WiFi_Struct));
  Serial.print("length: ");Serial.println(wifiCredentialList.length);
  Serial.println("calloc");
  Credentials_WiFi_Struct temp_wifiCredential;
  unsigned int indexACC = 0;
  for (int index = 0; index < wifiCredentialList.length; index++) {
    Serial.print("index: ");Serial.println(index);
    // Set the values from the document
    strlcpy(temp_wifiCredential.ssid,                  // <- destination
          credentialList["info"][ index ]["ssid"],  // <- source
          sizeof(temp_wifiCredential.ssid));         // <- destination's capacity
    strlcpy(temp_wifiCredential.pass,                  // <- destination
          credentialList["info"][ index ]["pass"],  // <- source
          sizeof(temp_wifiCredential.pass));         // <- destination's capacity
    
    if ( ensureValid_Credentials( temp_wifiCredential ) == false){
      wifiCredentialList.length -= 1;
      continue;
    }
    // Set the values from the document
    strlcpy(wifiCredentialList.credentials[ indexACC ].ssid,                  // <- destination
          credentialList["info"][ index ]["ssid"],  // <- source
          sizeof(wifiCredentialList.credentials[ indexACC ].ssid));         // <- destination's capacity
    strlcpy(wifiCredentialList.credentials[ indexACC ].pass,                  // <- destination
          credentialList["info"][ index ]["pass"],  // <- source
          sizeof(wifiCredentialList.credentials[ indexACC ].pass));         // <- destination's capacity
    Serial.print("ssid: ");Serial.println(wifiCredentialList.credentials[ indexACC ].ssid);
    Serial.print("pass: ");Serial.println(wifiCredentialList.credentials[ indexACC ].pass);
    indexACC++;
  }
  // Close the file
  file.close();
  
  
  //Check if SSID And PASS are Valid
  Serial.println("Checking if Valid!");
  for (int index = 0; index < wifiCredentialList.length; index++) {
    Serial.print("SSID: ");Serial.println(wifiCredentialList.credentials[ index ].ssid);
    Serial.print("PASS: ");Serial.println(wifiCredentialList.credentials[ index ].pass);
  }
  bool credentialsChanged = false;
  for (int index = 0; index < wifiCredentialList.length; index++) {
    if ( ensureValid_Credentials( wifiCredentialList.credentials[ index ] ) ){
      continue;
    }
    credentialsChanged = true;
    remove_WiFiCredential_From_WiFiCredentialsList(wifiCredentialList, wifiCredentialList.credentials[ index ]);
  }
  Serial.println("Checking if Valid!");
  for (int index = 0; index < wifiCredentialList.length; index++) {
    Serial.print("SSID: ");Serial.println(wifiCredentialList.credentials[ index ].ssid);
    Serial.print("PASS: ");Serial.println(wifiCredentialList.credentials[ index ].pass);
  }
  if (credentialsChanged){
    //resaves changed CredentialsList by removing inValided Credentials
    saveWiFiCredentialsList(filename, wifiCredentialList, fs);
  }
}


void saveWiFiCredentialToWiFiCredentialsList(List_WiFi_Credentials_Struct &wifiCredentialList, Credentials_WiFi_Struct &wifiCredential) {
  Serial.println("saveWiFiCredentialToWiFiCredentialsList()");
  //Fix Me?
  bool isINList = false;
  for (int index = 0; index < wifiCredentialList.length; index++) {
    if (strcmp(wifiCredentialList.credentials[ index ].ssid, wifiCredential.ssid)==0){
      strlcpy(wifiCredentialList.credentials[ index ].pass,                  // <- destination
          wifiCredential.pass,  // <- source
          sizeof(wifiCredentialList.credentials[ index ].pass));         // <- destination's capacity
      isINList = true;
      break;
    }
  }
  Serial.print("isINList: ");Serial.println(isINList);
  if (isINList){
    return;
  }
  Credentials_WiFi_Struct *tempCredentials;
  tempCredentials = (Credentials_WiFi_Struct*)calloc(wifiCredentialList.length, sizeof(Credentials_WiFi_Struct));
  for (int index = 0; index < wifiCredentialList.length; index++) {
    strcpy(tempCredentials[ index ].ssid, wifiCredentialList.credentials[ index ].ssid);
    strcpy(tempCredentials[ index ].pass, wifiCredentialList.credentials[ index ].pass);
  }
  Serial.println("Relocating og List");
  wifiCredentialList.length += 1;
  free(wifiCredentialList.credentials);
  wifiCredentialList.credentials = (Credentials_WiFi_Struct*)calloc(wifiCredentialList.length, sizeof(Credentials_WiFi_Struct));
  Serial.println("malloc/calloc/realloc og List Complete");
  for (int index = 0; index < (wifiCredentialList.length - 1); index++) {
    strcpy(wifiCredentialList.credentials[ index ].ssid, tempCredentials[ index ].ssid);
    strcpy(wifiCredentialList.credentials[ index ].pass, tempCredentials[ index ].pass);
  }
  Serial.println("adding New index!");
  strcpy(wifiCredentialList.credentials[ wifiCredentialList.length-1 ].ssid, wifiCredential.ssid);
  strcpy(wifiCredentialList.credentials[ wifiCredentialList.length-1 ].pass, wifiCredential.pass);
  free(tempCredentials);
}



void remove_WiFiCredential_From_WiFiCredentialsList(List_WiFi_Credentials_Struct &wifiCredentialList, Credentials_WiFi_Struct &wifiCredential) {
  Serial.println("remove_WiFiCredential_From_WiFiCredentialsList()");
  for (int index = 0; index < wifiCredentialList.length; index++) {
    Serial.print("SSID: ");Serial.println(wifiCredentialList.credentials[ index ].ssid);
    Serial.print("PASS: ");Serial.println(wifiCredentialList.credentials[ index ].pass);
  }
  Serial.println("Remove");
  Serial.print("SSID: ");Serial.println(wifiCredential.ssid);
  Serial.print("PASS: ");Serial.println(wifiCredential.pass);
  //will remove saved "WiFiCredential" if ssid is on the List!
  bool isINList = false;
  unsigned int selectIndex = 0;
  for (unsigned int index = 0; index < wifiCredentialList.length; index++) {
    if (wifiCredentialList.credentials[ index ].ssid == wifiCredential.ssid){
      isINList = true;
      selectIndex = index;
      break;
    }
  }
  Serial.print("isINList: ");Serial.println(isINList);
  if (!isINList){
    return;
  }
  Serial.println("removing Selected index!");
  //strcpy(wifiCredentialList.credentials[ selectIndex ].ssid, "");
  //strcpy(wifiCredentialList.credentials[ selectIndex ].pass, "");
  wifiCredentialList.length -= 1;
  
  //Reallocate by making a temp Array of the new size add all the Saved Credentials to that, then call calloc on "wifiCredentialList.credentials" and re-add the Credentials from the Temp Array
  Serial.println("Relocating og List");
  Credentials_WiFi_Struct *tempList;
  tempList = (Credentials_WiFi_Struct*)calloc(wifiCredentialList.length, sizeof(Credentials_WiFi_Struct));
  
  Serial.println("malloc/calloc/realloc og List Complete");
  Serial.println("Populate Resize List!");
  //bug: results in all saved Credentials to be removed if one is invalid! (Status : Fixed)
  //Finish me!
  unsigned int indexoffset = 0;
  for (unsigned int index = 0; index < (wifiCredentialList.length+1); index++) {
    Serial.print("indexoffset:");Serial.println( indexoffset );
    Serial.print("index:");Serial.println( index );
    if ( wifiCredentialList.credentials[ index ].ssid != wifiCredential.ssid ){
      Serial.println("NotEqual");
      //Fix Me!
      strcpy(tempList[ indexoffset ].ssid, wifiCredentialList.credentials[ index ].ssid);
      strcpy(tempList[ indexoffset ].pass, wifiCredentialList.credentials[ index ].pass);
      Serial.println(tempList[ indexoffset ].ssid);
      Serial.println(tempList[ indexoffset ].pass);
      Serial.println(wifiCredentialList.credentials[ index ].ssid);
      Serial.println(wifiCredentialList.credentials[ index ].pass);
      indexoffset++;
    }
  }
  free(wifiCredentialList.credentials);
  wifiCredentialList.credentials = (Credentials_WiFi_Struct*)calloc(wifiCredentialList.length, sizeof(Credentials_WiFi_Struct));
  for (unsigned int index = 0; index < (wifiCredentialList.length); index++) {
    ;
  }
  
  for (int index = 0; index < wifiCredentialList.length; index++) {
    Serial.print("SSID: ");Serial.println(wifiCredentialList.credentials[ index ].ssid);
    Serial.print("PASS: ");Serial.println(wifiCredentialList.credentials[ index ].pass);
  }
  
  
}



bool ensureValid_Credentials(Credentials_WiFi_Struct &wifiCredential){
  //later make "ensureValid_SSID" and "ensureValid_PASS" use (Credentials_WiFi_Struct &wifiCredential) as input
  return ( ensureValid_SSID( wifiCredential ) and ensureValid_PASS( wifiCredential ) );
}

bool ensureValid_SSID(Credentials_WiFi_Struct &wifiCredential){
  return ensureValid_SSIDchar( wifiCredential.ssid );
}
bool ensureValid_PASS(Credentials_WiFi_Struct &wifiCredential){
  return ensureValid_PASSchar( wifiCredential.pass );
}

bool ensureValid_SSIDchar(char* inputSSID){
  //https://routersecurity.org/SSID.php
  //https://www.cisco.com/assets/sol/sb/WAP321_Emulators/WAP321_Emulator_v1.0.0.3/help/Wireless05.html
  
  unsigned int inputSSIDLen = strlen(inputSSID);
  //The SSID can be any alphanumeric, case-sensitive entry from 2 to 32 characters.
  if ((inputSSIDLen>32)or(inputSSIDLen<2)){
    Serial.println("Returning False!");
    return false;
  }
  // In addition, these three characters cannot be the first character:!, #, and ; (ASCII 0x21, 0x23, and 0x3B, respectively).
  if ( (inputSSID[ 0 ] == 0x21)or(inputSSID[ 0 ] == 0x23)or(inputSSID[ 0 ] == 0x3B) ){
    Serial.println("Returning False!");
    return false;
  }
  //Trailing and leading spaces (ASCII 0x20) are not permitted.
  //NOTE     This means that spaces are allowed within the SSID, but not as the first or last character, and the period “.” (ASCII 0x2E) is also allowed. 
  if ( (inputSSID[ 0 ] == 0x20)or(inputSSID[ inputSSIDLen-1 ] == 0x20) ){
    Serial.println("Returning False!");
    return false;
  }
  char blackListChar[7] = {'?', '"', '$', '[', '\\', ']', '+'};//0x3F, 0x22, 0x24, 0x5B, 0x5C, 0x5D, 0x2B
  char blackListCharHex[7] = {0x3F, 0x22, 0x24, 0x5B, 0x5C, 0x5D, 0x2B};
  for (unsigned int index = 0; index < inputSSIDLen; index++) {
    Serial.print("CharIndex: ");Serial.println(inputSSID[ index ]);
    // The allowable characters are: ASCII 0x20, 0x21, 0x23, 0x25 through 0x2A, 0x2C through 0x3E, 0x40 through 0x5A, 0x5E through 0x7E.
    if (not ( (inputSSID[ index ] == 0x20)or(inputSSID[ index ] == 0x21)or(inputSSID[ index ] == 0x23)or(isCharRange(inputSSID[ index ], 0x25, 0x2A))or(isCharRange(inputSSID[ index ], 0x2C, 0x3E))or(isCharRange(inputSSID[ index ], 0x40, 0x5A))or(isCharRange(inputSSID[ index ], 0x5E, 0x7E)) ) ){
      Serial.println("Returning False!");
      return false;
    }
    //The printable characters plus the space (ASCII 0x20) are allowed, but these six characters are not: ?, ", $, [, \, ], and +.
    //{'?', '"', '$', '[', '\\', ']', '+'} == {0x3F, 0x22, 0x24, 0x5B, 0x5C, 0x5D, 0x2B}
    if (inputSSID[ index ] == 0x3F){
      Serial.println("Returning False!");
      Serial.print("inputSSID[ index ]: ");Serial.println(inputSSID[ index ], HEX);
      return false;
    }
    if (inputSSID[ index ] == 0x22){
      Serial.println("Returning False!");
      Serial.print("inputSSID[ index ]: ");Serial.println(inputSSID[ index ], HEX);
      return false;
    }
    if (inputSSID[ index ] == 0x24){
      Serial.println("Returning False!");
      Serial.print("inputSSID[ index ]: ");Serial.println(inputSSID[ index ], HEX);
      return false;
    }
    if (inputSSID[ index ] == 0x5B){
      Serial.println("Returning False!");
      Serial.print("inputSSID[ index ]: ");Serial.println(inputSSID[ index ], HEX);
      return false;
    }
    if (inputSSID[ index ] == 0x5C){
      Serial.println("Returning False!");
      Serial.print("inputSSID[ index ]: ");Serial.println(inputSSID[ index ], HEX);
      return false;
    }
    if (inputSSID[ index ] == 0x5D){
      Serial.println("Returning False!");
      Serial.print("inputSSID[ index ]: ");Serial.println(inputSSID[ index ], HEX);
      return false;
    }
    if (inputSSID[ index ] == 0x2B){
      Serial.println("Returning False!");
      Serial.print("inputSSID[ index ]: ");Serial.println(inputSSID[ index ], HEX);
      return false;
    }
    /*
    for (unsigned int indexCheck = 0; indexCheck < 7; indexCheck++) {
      Serial.print("indexCheck: ");Serial.print(indexCheck);
      Serial.print(", ");
      Serial.print("CharCheck: ");Serial.println(blackListChar[indexCheck]);
      Serial.print("CharCheckHex: ");Serial.println(blackListCharHex[indexCheck]);
      if (inputSSID[ index ] == blackListChar[indexCheck]){
        Serial.println("Returning False!");
        return false;
      }
    }
    //*/
    
  }
  Serial.println("Returning True!");
  return true;
}
bool ensureValid_PASSchar(char* inputPASS){
  //Enter a string of at least 8 characters to a maximum of 63 characters. Acceptable characters include upper and lower case alphabetic letters, the numeric digits, and special symbols such as @ and #.
  unsigned int inputPASSLen = strlen(inputPASS);
  if ( ((inputPASSLen>63)or(inputPASSLen<8)) and (inputPASSLen!=0) ){
    Serial.println("Returning False!");
    return false;
  }
  Serial.println("Returning True!");
  return true;
}


bool isCharRange(char inputChar, unsigned int startv, unsigned int endv){
  return (inputChar >= startv and inputChar < endv);
}




/*
//https://playground.arduino.cc/Code/Enum/
enum userType
{
   x,
   y,
   z
};
*/






void saveWiFicredentials(Credentials_WiFi_Struct &wifiCredentials){
  // "/Wifi_Connections.json"
  List_WiFi_Credentials_Struct temp_wifiCredentialList;
  //Load WiFiCredentialsList
  Serial.println(F("Loading WiFi configuration..."));
  loadWiFiCredentialsList("/Wifi_Connections.json", temp_wifiCredentialList, SPIFFS);
  Serial.println(wifiCredentials.ssid);
  Serial.println(wifiCredentials.pass);
  saveWiFiCredentialToWiFiCredentialsList(temp_wifiCredentialList, wifiCredentials);
  saveWiFiCredentialsList("/Wifi_Connections.json", temp_wifiCredentialList, SPIFFS);
}





void OnWiFiEvent(WiFiEvent_t event)
{
  //https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/wifi.html
  switch (event) {
    case SYSTEM_EVENT_STA_CONNECTED:
      showPrePrint(Serial, "(ON_SYSTEM_EVENT)-->");
      Serial.println(F("ESP32 Connected to WiFi Network"));
      showPrePrint(Serial, "(ON_SYSTEM_EVENT)-->");
      Serial.println(F("the ESP32 is connected in station mode to an access point/hotspot (your router)"));
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      showPrePrint(Serial, "(ON_SYSTEM_EVENT)-->");
      Serial.println(F("the ESP32 station disconnected from the access point."));
      break;
    case SYSTEM_EVENT_AP_START:
      showPrePrint(Serial, "(ON_SYSTEM_EVENT)-->");
      Serial.println(F("ESP32 soft AP started"));
      break;
    case SYSTEM_EVENT_AP_STOP:
      showPrePrint(Serial, "(ON_SYSTEM_EVENT)-->");
      Serial.println(F("ESP32 soft AP Stopped."));
      break;
    case SYSTEM_EVENT_AP_STACONNECTED:
      showPrePrint(Serial, "(ON_SYSTEM_EVENT)-->");
      Serial.println(F("Station connected to ESP32 soft AP"));
      break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
      showPrePrint(Serial, "(ON_SYSTEM_EVENT)-->");
      Serial.println(F("Station disconnected from ESP32 soft AP"));
      break;
    
    default:
      showPrePrint(Serial, "(ON_SYSTEM_EVENT)-->");
      Serial.print("(CatchALL)-->");
      Serial.printf("Got Event: %d\n", event);
      break;
  }
}








void startSoftAP(Stream &serialport){
  //https://randomnerdtutorials.com/esp32-access-point-ap-web-server/
  char ssid[33]     = "ESP32-Access-Point";
  char password[64] = "123456789";
  // Connect to Wi-Fi network with SSID and password
  serialport.print("Setting AP (Access Point)…");
  WiFi.softAPConfig(ip, gateway, subnet);
  // Remove the password parameter, if you want the AP (Access Point) to be open
  (void)WiFi.softAP(ssid, password);
  ;
  IPAddress IP = WiFi.softAPIP();
  serialport.print("AP IP address: ");
  serialport.println(IP);
}


//https://www.mischianti.org/2021/03/06/esp32-practical-power-saving-manage-wifi-and-cpu-1/
void disable_STA_WiFi() {
  WiFi.disconnect(true);  // Disconnect from the network
  disableWiFi_STA_Mode();
}



void enable_STA_WiFi() {
  //Remove This Function if it becomes basically on necessary
  //https://docs.arduino.cc/library-examples/wifi-library/ScanNetworks
  enableWiFi_STA_Mode();
  
  WiFi.disconnect(false);  // Reconnect the network
  
  //WiFi.begin(STA_SSID, STA_PASS);
  //wifi_status = WiFi.begin("TP-Link_0F3D", "Jerkface13597603");//control WiFi connect "TP-Link_0F3D" "Jerkface13597603"
  //wifi_status = WiFi.begin("Hail Hydra", "aarsabteeros@48");//control WiFi connect "Hail Hydra" "aarsabteeros@48"
  //wifi_status = WiFi.begin("Test Network", "12345678");
  
  /*
  int numSsid = WiFi.scanNetworks();
  if (numSsid == -1) {
    Serial.println("Couldn't get a wifi connection");
    return;
  }
  */
  
  
  //
  
  /*
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }//*/
  /*
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("WiFi Failed!\n");
    return;
  }
  //*/
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


//https://techtutorialsx.com/2021/01/04/esp32-soft-ap-and-station-modes/
void disableWiFiSoftAP()
{
  WiFi.softAPdisconnect(false);
  WiFi.enableAP(false);
  disableWiFi_AP_Mode();
}

void enableWiFiSoftAP(char* soft_ap_ssid, char* soft_ap_password)
{
  //https://espressif-docs.readthedocs-hosted.com/projects/arduino-esp32/en/latest/api/wifi.html
  WiFi.softAPdisconnect(true);
  WiFi.enableAP(true);
  enableWiFi_AP_Mode();
  //bool softAP(const char* ssid, const char* passphrase = NULL, int channel = 1, int ssid_hidden = 0, int max_connection = 4, bool ftm_responder = false);
  WiFi.softAP(soft_ap_ssid, soft_ap_password);
}


/*
 * \/ Mode Disable/Enable \/
 */
 void disableWiFi_AP_Mode() {
  if (WiFi.getMode() == WIFI_MODE_AP) {
    WiFi.mode(WIFI_OFF);
  } else if (WiFi.getMode() == WIFI_MODE_APSTA) {
    WiFi.mode(WIFI_STA);
  }
}
void enableWiFi_AP_Mode() {
  if (WiFi.getMode() == WIFI_MODE_NULL) {
    WiFi.mode(WIFI_AP);
  } else if (WiFi.getMode() == WIFI_MODE_APSTA) {
    WiFi.mode(WIFI_AP_STA);
  }
}
void disableWiFi_STA_Mode() {
  if (WiFi.getMode() == WIFI_MODE_STA) {
    WiFi.mode(WIFI_OFF);
  } else if (WiFi.getMode() == WIFI_MODE_APSTA) {
    WiFi.mode(WIFI_AP);
  }
}
void enableWiFi_STA_Mode() {
  if (WiFi.getMode() == WIFI_MODE_NULL) {
    WiFi.mode(WIFI_STA);
  } else if (WiFi.getMode() == WIFI_MODE_AP) {
    WiFi.mode(WIFI_AP_STA);
  }
}
/*
 * /\ Mode Disable/Enable /\
 */


void printAPStationNum(Stream &serialport){
  serialport.printf("Stations connected to soft-AP = %d\n", WiFi.softAPgetStationNum());
}




/*
 * WiFi Stuff End
 */






/*
 * allow usage for .bmp files for images
 */
typedef struct {
  //https://en.wikipedia.org/wiki/BMP_file_format
  char imageName[32] = "IMAGE";//max name length = 31 charaters
  uint16_t imageByteLength;
  uint16_t widthPx;//in pixels
  uint16_t heightPx;//in pixels
  unsigned char* image;//size is imageByteLength

  char file_BitmapFileHeader[14];
  
  
  char file_DIB_Header[];
  char file_Extra_bit_masks[12 or 16];//Optional
  byte* file_Color_table;//Semi-Optional
  byte* file_Gap1;//Optional
  byte* file_Pixel_array;
  byte* file_Gap2;//Optional
  byte* file_ICC_color_profile;//Optional
} WINbitmapIMG_Struct;


typedef struct {
  char headerField[2];
  char bmpFileSize[4];//size is in Bytes
  byte reserved_0[2];
  byte reserved_1[2];
  byte offset_pixelArray[4];
} BitmapFileHeader_Struct;





class DIB_Class {
  private:
    uint32_t headerSize;
    uint8_t *rawDIB_HeaderData;
    void init(){
      ;
    };
  public:
    DIB_Class(){
      init();
    };
    int returnDIB_Header_Size(){
      return NULL;
    };
    int returnPixelWidth(){
      return NULL;
    };
    int returnPixelHeight(){
      return NULL;
    };
    int returnPlanes(){
      return NULL;
    };
    int returnBitsPerPixel(){
      return NULL;
    };
    int returnCompression(){
      return NULL;
    };
    int returnImageSize(){
      return NULL;
    };
    int returnXPixelsPerMeter(){
      return NULL;
    };
    int returnYPixelsPerMeter(){
      return NULL;
    };
    int returnColoursInColourTables(){
      return NULL;
    };
    int returnImportantColoursCount(){
      return NULL;
    };
    int return_Red_channel_bitmask(){
      return NULL;
    };
    int return_Green_channel_bitmask(){
      return NULL;
    };
    int return_Blue_channel_bitmask(){
      return NULL;
    };
    int return_Alpha_channel_bitmask(){
      return NULL;
    };
    int return_Colour_Space_Type(){
      return NULL;
    };
    int return_Color_Space_Endpoints(){
      return NULL;
    };
    int return_Gamma_for_Red_channel(){
      return NULL;
    };
    int return_Gamma_for_Green_channel(){
      return NULL;
    };
    int return_Gamma_for_Blue_channel(){
      return NULL;
    };
    int return_Intent(){
      return NULL;
    };
    int return_ICC_Profile_Data(){
      return NULL;
    };
    int return_ICC_Profile_Size(){
      return NULL;
    };
    int return_Reserved(){
      return NULL;
    };
};



typedef struct {
  uint8_t rawData[14];
  uint8_t Signature[2];
  uint8_t File_Size[4];
  uint8_t Reserved1[2];
  uint8_t Reserved2[2];
  uint8_t File_Offset_to_PixelArray[4];
} BITMAPFILEHEADER_byteArray_Struct;

typedef struct {
  char Signature[2+1] = "  ";//only two chars
  uint32_t File_Size;
  uint16_t Reserved1;
  uint16_t Reserved2;
  uint32_t File_Offset_to_PixelArray;
} BITMAPFILEHEADER_Struct;




typedef unsigned long DWORD;
typedef word WORD;
typedef long LONG;


typedef struct tagBITMAPFILEHEADER {
  WORD  bfType;
  DWORD bfSize;
  WORD  bfReserved1;
  WORD  bfReserved2;
  DWORD bfOffBits;
} BITMAPFILEHEADER, *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct _Win4xBitmapHeader{
  DWORD Size;            /* Size of this header in bytes */
  LONG  Width;           /* Image width in pixels */
  LONG  Height;          /* Image height in pixels */
  WORD  Planes;          /* Number of color planes */
  WORD  BitsPerPixel;    /* Number of bits per pixel */
  DWORD Compression;     /* Compression methods used */
  DWORD SizeOfBitmap;    /* Size of bitmap in bytes */
  LONG  HorzResolution;  /* Horizontal resolution in pixels per meter */
  LONG  VertResolution;  /* Vertical resolution in pixels per meter */
  DWORD ColorsUsed;      /* Number of colors in the image */
  DWORD ColorsImportant; /* Minimum number of important colors */
  /* Fields added for Windows 4.x follow this line */
  
  DWORD RedMask;       /* Mask identifying bits of red component */
  DWORD GreenMask;     /* Mask identifying bits of green component */
  DWORD BlueMask;      /* Mask identifying bits of blue component */
  DWORD AlphaMask;     /* Mask identifying bits of alpha component */
  DWORD CSType;        /* Color space type */
  LONG  RedX;          /* X coordinate of red endpoint */
  LONG  RedY;          /* Y coordinate of red endpoint */
  LONG  RedZ;          /* Z coordinate of red endpoint */
  LONG  GreenX;        /* X coordinate of green endpoint */
  LONG  GreenY;        /* Y coordinate of green endpoint */
  LONG  GreenZ;        /* Z coordinate of green endpoint */
  LONG  BlueX;         /* X coordinate of blue endpoint */
  LONG  BlueY;         /* Y coordinate of blue endpoint */
  LONG  BlueZ;         /* Z coordinate of blue endpoint */
  DWORD GammaRed;      /* Gamma red coordinate scale value */
  DWORD GammaGreen;    /* Gamma green coordinate scale value */
  DWORD GammaBlue;     /* Gamma blue coordinate scale value */
} WIN4XBITMAPHEADER;

typedef struct _Win5xBitmapHeader{
  DWORD Size;            /* Size of this header in bytes */
  LONG  Width;           /* Image width in pixels */
  LONG  Height;          /* Image height in pixels */
  WORD  Planes;          /* Number of color planes */
  WORD  BitsPerPixel;    /* Number of bits per pixel */
  DWORD Compression;     /* Compression methods used */
  DWORD SizeOfBitmap;    /* Size of bitmap in bytes */
  LONG  HorzResolution;  /* Horizontal resolution in pixels per meter */
  LONG  VertResolution;  /* Vertical resolution in pixels per meter */
  DWORD ColorsUsed;      /* Number of colors in the image */
  DWORD ColorsImportant; /* Minimum number of important colors */
  /* Fields added for Windows 4.x follow this line */
  
  DWORD RedMask;       /* Mask identifying bits of red component */
  DWORD GreenMask;     /* Mask identifying bits of green component */
  DWORD BlueMask;      /* Mask identifying bits of blue component */
  DWORD AlphaMask;     /* Mask identifying bits of alpha component */
  DWORD CSType;        /* Color space type */
  LONG  RedX;          /* X coordinate of red endpoint */
  LONG  RedY;          /* Y coordinate of red endpoint */
  LONG  RedZ;          /* Z coordinate of red endpoint */
  LONG  GreenX;        /* X coordinate of green endpoint */
  LONG  GreenY;        /* Y coordinate of green endpoint */
  LONG  GreenZ;        /* Z coordinate of green endpoint */
  LONG  BlueX;         /* X coordinate of blue endpoint */
  LONG  BlueY;         /* Y coordinate of blue endpoint */
  LONG  BlueZ;         /* Z coordinate of blue endpoint */
  DWORD GammaRed;      /* Gamma red coordinate scale value */
  DWORD GammaGreen;    /* Gamma green coordinate scale value */
  DWORD GammaBlue;     /* Gamma blue coordinate scale value */
  
  DWORD        Intent;
  DWORD        ProfileData;
  DWORD        ProfileSize;
  DWORD        Reserved;
} WIN5XBITMAPHEADER;






/*
 * allow usage for .bmp files for images /\
 */




/*
   Struct Images and Animations: Begin
*/


//basic convertion from .bmp to something usable!
typedef enum {
  LSB,
  MSB
} ENUMbyteOrder;
unsigned long createDWORDfromBytes(unsigned char byteArray[], ENUMbyteOrder bitOrderMode);
long createLONGfromBytes(unsigned char byteArray[], ENUMbyteOrder bitOrderMode);
word createWORDfromBytes(unsigned char byteArray[], ENUMbyteOrder bitOrderMode);

void load_bitmapIMG_File_struct(File &fileBMP, IMGbitmapStruct &bitmapIMG);


unsigned long createDWORDfromBytes(unsigned char byteArray[], ENUMbyteOrder bitOrderMode)
{
  unsigned long dbuff[4];
  switch (bitOrderMode){
    case LSB:
      dbuff[0] = (unsigned long)byteArray[ 3 ];
      dbuff[1] = (unsigned long)byteArray[ 2 ];
      dbuff[2] = (unsigned long)byteArray[ 1 ];
      dbuff[3] = (unsigned long)byteArray[ 0 ];
      break;
    case MSB:
      dbuff[0] = (unsigned long)byteArray[ 0 ];
      dbuff[1] = (unsigned long)byteArray[ 1 ];
      dbuff[2] = (unsigned long)byteArray[ 2 ];
      dbuff[3] = (unsigned long)byteArray[ 3 ];
      break;
    default:
      return NULL;
      break;
  }
  return (dbuff[0] << 24) | (dbuff[1] << 16) | (dbuff[2] << 8) | dbuff[3];
}

word createWORDfromBytes(unsigned char byteArray[], ENUMbyteOrder bitOrderMode)
{
  word wbuff[2];
  switch (bitOrderMode){
    case LSB:
      wbuff[0] = (word)byteArray[ 1 ];
      wbuff[1] = (word)byteArray[ 0 ];
      break;
    case MSB:
      wbuff[0] = (word)byteArray[ 0 ];
      wbuff[1] = (word)byteArray[ 1 ];
      break;
    default:
      return NULL;
      break;
  }
  return (wbuff[0] << 8) | wbuff[1];
}

long createLONGfromBytes(unsigned char byteArray[], ENUMbyteOrder bitOrderMode)
{
  long lbuff[4];
  switch (bitOrderMode){
    case LSB:
      lbuff[0] = (long)byteArray[ 3 ];
      lbuff[1] = (long)byteArray[ 2 ];
      lbuff[2] = (long)byteArray[ 1 ];
      lbuff[3] = (long)byteArray[ 0 ];
      break;
    case MSB:
      lbuff[0] = (long)byteArray[ 0 ];
      lbuff[1] = (long)byteArray[ 1 ];
      lbuff[2] = (long)byteArray[ 2 ];
      lbuff[3] = (long)byteArray[ 3 ];
      break;
    default:
      return NULL;
      break;
  }
  return (((long)lbuff[0] << 24) | ((long)lbuff[1] << 16) | ((long)lbuff[2] << 8) | (long)lbuff[3]);
}


//try sourcing from here 
//https://www.fileformat.info/format/bmp/egff.htm
//https://www.fileformat.info/format/os2bmp/egff.htm

void load_bitmapIMG_File_struct(File &fileBMP, IMGbitmapStruct &bitmapIMG) {
  Serial.println("-----(Func)load_bitmapIMG_File_struct-----");
  printFreeHeap(Serial);
  const char _NotSupportedMessage[] = "Not Supported!";
  typedef  enum {
    BI_RGB = 0x0000,
    BI_RLE8 = 0x0001,
    BI_RLE4 = 0x0002,
    BI_BITFIELDS = 0x0003,
    BI_JPEG = 0x0004,
    BI_PNG = 0x0005,
    BI_CMYK = 0x000B,
    BI_CMYKRLE8 = 0x000C,
    BI_CMYKRLE4 = 0x000D
  } Compression;
  
  typedef  enum {
    DIB_OS21XBITMAPHEADER_headerSize = 12,    //not supported
    DIB_OS22XBITMAPHEADER_16_headerSize = 16, //not supported
    DIB_OS22XBITMAPHEADER_64_headerSize = 64, //not supported
    
    DIB_BITMAPCOREHEADER_headerSize = 12,     //not supported
    DIB_BITMAPINFOHEADER_headerSize = 40,     //not supported
    DIB_BITMAPV2INFOHEADER_headerSize = 52,   //not supported
    DIB_BITMAPV3INFOHEADER_headerSize = 56,   //not supported
    DIB_BITMAPV4HEADER_headerSize = 108,      //supported
    DIB_BITMAPV5HEADER_headerSize = 124,      //supported
  } BITMAP_Header_Sizes_Enum;
  
  
  if (!fileBMP) {
    Serial.println("Failed to open file for reading");
    return;
  }
  //→ used for struct pointer
  
  int read_len;
  int avail_len = fileBMP.available();
  bool seekResult;
  
  //read File Header
  BITMAPFILEHEADER_byteArray_Struct * bitmapHeaderByteArray = new BITMAPFILEHEADER_byteArray_Struct;
  (void)fileBMP.seek(0);
  read_len = fileBMP.read(bitmapHeaderByteArray->rawData, 14); // read all to buffer to buffer
  memcpy ( &bitmapHeaderByteArray->Signature, &bitmapHeaderByteArray->rawData[0], sizeof(bitmapHeaderByteArray->Signature) );
  memcpy ( &bitmapHeaderByteArray->File_Size, &bitmapHeaderByteArray->rawData[2], sizeof(bitmapHeaderByteArray->File_Size) );
  memcpy ( &bitmapHeaderByteArray->Reserved1, &bitmapHeaderByteArray->rawData[6], sizeof(bitmapHeaderByteArray->Reserved1) );
  memcpy ( &bitmapHeaderByteArray->Reserved2, &bitmapHeaderByteArray->rawData[8], sizeof(bitmapHeaderByteArray->Reserved2) );
  memcpy ( &bitmapHeaderByteArray->File_Offset_to_PixelArray, &bitmapHeaderByteArray->rawData[10], sizeof(bitmapHeaderByteArray->File_Offset_to_PixelArray) );
  
  //store File Header data better
  BITMAPFILEHEADER_Struct * bitmapHeader = new BITMAPFILEHEADER_Struct;
  bitmapHeader->Signature[0] = (char)bitmapHeaderByteArray->Signature[0];
  bitmapHeader->Signature[1] = (char)bitmapHeaderByteArray->Signature[1];
  bitmapHeader->Signature[2] = '\0';
  
  bitmapHeader->File_Size = (((uint32_t)bitmapHeaderByteArray->File_Size[3]) << 24) | (((uint32_t)bitmapHeaderByteArray->File_Size[2]) << 16) | (((uint32_t)bitmapHeaderByteArray->File_Size[1]) << 8) | (uint32_t)bitmapHeaderByteArray->File_Size[0];
  bitmapHeader->Reserved1 = (((uint16_t)bitmapHeaderByteArray->Reserved1[1]) << 8) | (uint16_t)bitmapHeaderByteArray->Reserved1[0];
  bitmapHeader->Reserved2 = (((uint16_t)bitmapHeaderByteArray->Reserved2[1]) << 8) | (uint16_t)bitmapHeaderByteArray->Reserved2[0];
  bitmapHeader->File_Offset_to_PixelArray = (((uint32_t)bitmapHeaderByteArray->File_Offset_to_PixelArray[3]) << 24) | (((uint32_t)bitmapHeaderByteArray->File_Offset_to_PixelArray[2]) << 16) | (((uint32_t)bitmapHeaderByteArray->File_Offset_to_PixelArray[1]) << 8) | (uint32_t)bitmapHeaderByteArray->File_Offset_to_PixelArray[0];
  delete bitmapHeaderByteArray;
  
  //Check File Header Signature
  if (strcmp(bitmapHeader->Signature, "BM")==0){
    Serial.println("case \"BM\"");
  }else if (strcmp(bitmapHeader->Signature, "BA")==0){
    Serial.println("case \"BA\"");
    Serial.print("bitmapHeader->Signature: ");Serial.println(bitmapHeader->Signature);
    Serial.println(_NotSupportedMessage);
    delete bitmapHeader;
    return;
  }else if (strcmp(bitmapHeader->Signature, "CI")==0){
    Serial.println("case \"CI\"");
    Serial.print("bitmapHeader->Signature: ");Serial.println(bitmapHeader->Signature);
    Serial.println(_NotSupportedMessage);
    delete bitmapHeader;
    return;
  }else if (strcmp(bitmapHeader->Signature, "CP")==0){
    Serial.println("case \"CP\"");
    Serial.print("bitmapHeader->Signature: ");Serial.println(bitmapHeader->Signature);
    Serial.println(_NotSupportedMessage);
    delete bitmapHeader;
    return;
  }else if (strcmp(bitmapHeader->Signature, "IC")==0){
    Serial.println("case \"IC\"");
    Serial.print("bitmapHeader->Signature: ");Serial.println(bitmapHeader->Signature);
    Serial.println(_NotSupportedMessage);
    delete bitmapHeader;
    return;
  }else if (strcmp(bitmapHeader->Signature, "PT")==0){
    Serial.println("case \"PT\"");
    Serial.print("bitmapHeader->Signature: ");Serial.println(bitmapHeader->Signature);
    Serial.println(_NotSupportedMessage);
    delete bitmapHeader;
    return;
  }else{
    Serial.print("bitmapHeader->Signature: ");Serial.println(bitmapHeader->Signature);
    Serial.print("Signature ");Serial.println(_NotSupportedMessage);
    delete bitmapHeader;
    return;
  };

  //Process DIB Header
  Serial.print("fileBMP.position() Result: ");Serial.println( fileBMP.position() );
  uint32_t headerDIBSize = fileBMP.read() | ((fileBMP.read()) << 8) | ((fileBMP.read()) << 16) | ((fileBMP.read()) << 24);
  Serial.print("headerDIBSize: ");Serial.println(headerDIBSize);
  
  unsigned char* raw_DIB_Header = (unsigned char*)malloc(headerDIBSize + 1);
  (void)fileBMP.seek(14);
  fileBMP.read(raw_DIB_Header, headerDIBSize);
  printFreeHeap(Serial);
  WIN4XBITMAPHEADER * bitmap_DIB_header_V4 = new WIN4XBITMAPHEADER;
  WIN5XBITMAPHEADER * bitmap_DIB_header_V5 = new WIN5XBITMAPHEADER;
  WIN5XBITMAPHEADER * bitmap_DIB_header = new WIN5XBITMAPHEADER;
  Serial.print("Pointer of bitmap_DIB_header_V4: ");Serial.printf("%p\n", (void *)bitmap_DIB_header_V4);//prints Pointer
  Serial.print("Pointer of bitmap_DIB_header_V5: ");Serial.printf("%p\n", (void *)bitmap_DIB_header_V5);//prints Pointer
  printFreeHeap(Serial);
  
  
  bool flag_DIBSupported = false;
  if (strcmp(bitmapHeader->Signature, "BM")==0){
  switch (headerDIBSize)
  {
    case DIB_BITMAPCOREHEADER_headerSize or DIB_OS21XBITMAPHEADER_headerSize:
      Serial.println("case DIB_BITMAPCOREHEADER_headerSize");
      Serial.println(_NotSupportedMessage);
      flag_DIBSupported = false;
      break;
    case DIB_BITMAPINFOHEADER_headerSize:
      Serial.println("case DIB_BITMAPINFOHEADER_headerSize");
      Serial.println(_NotSupportedMessage);
      flag_DIBSupported = false;
      break;
    case DIB_BITMAPV2INFOHEADER_headerSize:
      Serial.println("case DIB_BITMAPV2INFOHEADER_headerSize");
      Serial.println(_NotSupportedMessage);
      flag_DIBSupported = false;
      break;
    case DIB_BITMAPV3INFOHEADER_headerSize:
      Serial.println("case DIB_BITMAPV3INFOHEADER_headerSize");
      Serial.println(_NotSupportedMessage);
      flag_DIBSupported = false;
      break;
    case DIB_BITMAPV4HEADER_headerSize:
      Serial.println("case DIB_BITMAPV4HEADER_headerSize");
      flag_DIBSupported = true;
      //https://learn.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapv4header
      bitmap_DIB_header_V4->Size =            createDWORDfromBytes(&raw_DIB_Header[0], LSB);
      bitmap_DIB_header_V4->Width =           createLONGfromBytes(&raw_DIB_Header[4], LSB);
      bitmap_DIB_header_V4->Height =          createLONGfromBytes(&raw_DIB_Header[8], LSB);
      bitmap_DIB_header_V4->Planes =          createWORDfromBytes(&raw_DIB_Header[12], LSB);
      bitmap_DIB_header_V4->BitsPerPixel =    createWORDfromBytes(&raw_DIB_Header[14], LSB);
      
      bitmap_DIB_header_V4->Compression =     createDWORDfromBytes(&raw_DIB_Header[16], LSB);
      bitmap_DIB_header_V4->SizeOfBitmap =    createDWORDfromBytes(&raw_DIB_Header[20], LSB);
      
      bitmap_DIB_header_V4->HorzResolution =  createLONGfromBytes(&raw_DIB_Header[24], LSB);
      bitmap_DIB_header_V4->VertResolution =  createLONGfromBytes(&raw_DIB_Header[28], LSB);
      
      bitmap_DIB_header_V4->ColorsUsed =      createDWORDfromBytes(&raw_DIB_Header[32], LSB);
      bitmap_DIB_header_V4->ColorsImportant = createDWORDfromBytes(&raw_DIB_Header[36], LSB);
      
      bitmap_DIB_header_V4->RedMask =         createDWORDfromBytes(&raw_DIB_Header[40], LSB);
      bitmap_DIB_header_V4->GreenMask =       createDWORDfromBytes(&raw_DIB_Header[44], LSB);
      bitmap_DIB_header_V4->BlueMask =        createDWORDfromBytes(&raw_DIB_Header[48], LSB);
      bitmap_DIB_header_V4->AlphaMask =       createDWORDfromBytes(&raw_DIB_Header[52], LSB);
      bitmap_DIB_header_V4->CSType =          createDWORDfromBytes(&raw_DIB_Header[56], LSB);
      
      bitmap_DIB_header_V4->RedX =            createDWORDfromBytes(&raw_DIB_Header[60], LSB);
      bitmap_DIB_header_V4->RedY =            createDWORDfromBytes(&raw_DIB_Header[64], LSB);
      bitmap_DIB_header_V4->RedZ =            createDWORDfromBytes(&raw_DIB_Header[68], LSB);
      bitmap_DIB_header_V4->GreenX =          createDWORDfromBytes(&raw_DIB_Header[72], LSB);
      bitmap_DIB_header_V4->GreenY =          createDWORDfromBytes(&raw_DIB_Header[76], LSB);
      bitmap_DIB_header_V4->GreenZ =          createDWORDfromBytes(&raw_DIB_Header[80], LSB);
      bitmap_DIB_header_V4->BlueX =           createDWORDfromBytes(&raw_DIB_Header[84], LSB);
      bitmap_DIB_header_V4->BlueY =           createDWORDfromBytes(&raw_DIB_Header[88], LSB);
      bitmap_DIB_header_V4->BlueZ =           createDWORDfromBytes(&raw_DIB_Header[92], LSB);
      
      bitmap_DIB_header_V4->GammaRed =        createDWORDfromBytes(&raw_DIB_Header[96], LSB);
      bitmap_DIB_header_V4->GammaGreen =      createDWORDfromBytes(&raw_DIB_Header[100], LSB);
      bitmap_DIB_header_V4->GammaBlue =       createDWORDfromBytes(&raw_DIB_Header[104], LSB);


      //copy all values to V5
      Serial.println("-----copy bitmap_DIB_header_V4 data to bitmap_DIB_header_V5 !-----");
      bitmap_DIB_header_V5->Size = bitmap_DIB_header_V4->Size;
      bitmap_DIB_header_V5->Width = bitmap_DIB_header_V4->Width;
      bitmap_DIB_header_V5->Height = bitmap_DIB_header_V4->Height;
      bitmap_DIB_header_V5->Planes = bitmap_DIB_header_V4->Planes;
      bitmap_DIB_header_V5->BitsPerPixel = bitmap_DIB_header_V4->BitsPerPixel;
      bitmap_DIB_header_V5->Compression = bitmap_DIB_header_V4->Compression;
      bitmap_DIB_header_V5->SizeOfBitmap = bitmap_DIB_header_V4->SizeOfBitmap;
      bitmap_DIB_header_V5->HorzResolution = bitmap_DIB_header_V4->HorzResolution;
      bitmap_DIB_header_V5->VertResolution = bitmap_DIB_header_V4->VertResolution;
      bitmap_DIB_header_V5->ColorsUsed = bitmap_DIB_header_V4->ColorsUsed;
      bitmap_DIB_header_V5->ColorsImportant = bitmap_DIB_header_V4->ColorsImportant;
      bitmap_DIB_header_V5->RedMask = bitmap_DIB_header_V4->RedMask;
      bitmap_DIB_header_V5->GreenMask = bitmap_DIB_header_V4->GreenMask;
      bitmap_DIB_header_V5->BlueMask = bitmap_DIB_header_V4->BlueMask;
      bitmap_DIB_header_V5->AlphaMask = bitmap_DIB_header_V4->AlphaMask;
      bitmap_DIB_header_V5->CSType = bitmap_DIB_header_V4->CSType;
      bitmap_DIB_header_V5->RedX = bitmap_DIB_header_V4->RedX;
      bitmap_DIB_header_V5->RedY = bitmap_DIB_header_V4->RedY;
      bitmap_DIB_header_V5->RedZ = bitmap_DIB_header_V4->RedZ;
      bitmap_DIB_header_V5->GreenX = bitmap_DIB_header_V4->GreenX;
      bitmap_DIB_header_V5->GreenY = bitmap_DIB_header_V4->GreenY;
      bitmap_DIB_header_V5->GreenZ = bitmap_DIB_header_V4->GreenZ;
      bitmap_DIB_header_V5->BlueX = bitmap_DIB_header_V4->BlueX;
      bitmap_DIB_header_V5->BlueY = bitmap_DIB_header_V4->BlueY;
      bitmap_DIB_header_V5->BlueZ = bitmap_DIB_header_V4->BlueZ;
      bitmap_DIB_header_V5->GammaRed = bitmap_DIB_header_V4->GammaRed;
      bitmap_DIB_header_V5->GammaGreen = bitmap_DIB_header_V4->GammaGreen;
      bitmap_DIB_header_V5->GammaBlue = bitmap_DIB_header_V4->GammaBlue;
      
      bitmap_DIB_header_V5->Intent = NULL;
      bitmap_DIB_header_V5->ProfileData = NULL;
      bitmap_DIB_header_V5->ProfileSize = NULL;
      bitmap_DIB_header_V5->Reserved = NULL;
      
      //memcpy(&bitmap_DIB_header_V5, &bitmap_DIB_header_V4, sizeof(bitmap_DIB_header_V4));// fucks Shit up
      Serial.println("-----Done copy bitmap_DIB_header_V4 data to bitmap_DIB_header_V5 !-----");
      break;
    case DIB_BITMAPV5HEADER_headerSize:
      Serial.println("case DIB_BITMAPV5HEADER_headerSize");
      flag_DIBSupported = true;
      //https://learn.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapv5header
      bitmap_DIB_header_V5->Size =            createDWORDfromBytes(&raw_DIB_Header[0], LSB);
      bitmap_DIB_header_V5->Width =           createLONGfromBytes(&raw_DIB_Header[4], LSB);
      bitmap_DIB_header_V5->Height =          createLONGfromBytes(&raw_DIB_Header[8], LSB);
      
      bitmap_DIB_header_V5->Planes =          createWORDfromBytes(&raw_DIB_Header[12], LSB);
      bitmap_DIB_header_V5->BitsPerPixel =    createWORDfromBytes(&raw_DIB_Header[14], LSB);
      
      bitmap_DIB_header_V5->Compression =     createDWORDfromBytes(&raw_DIB_Header[16], LSB);
      bitmap_DIB_header_V5->SizeOfBitmap =    createDWORDfromBytes(&raw_DIB_Header[20], LSB);
      
      bitmap_DIB_header_V5->HorzResolution =  createLONGfromBytes(&raw_DIB_Header[24], LSB);
      bitmap_DIB_header_V5->VertResolution =  createLONGfromBytes(&raw_DIB_Header[28], LSB);
      
      bitmap_DIB_header_V5->ColorsUsed =      createDWORDfromBytes(&raw_DIB_Header[32], LSB);
      bitmap_DIB_header_V5->ColorsImportant = createDWORDfromBytes(&raw_DIB_Header[36], LSB);
      
      bitmap_DIB_header_V5->RedMask =         createDWORDfromBytes(&raw_DIB_Header[40], LSB);
      bitmap_DIB_header_V5->GreenMask =       createDWORDfromBytes(&raw_DIB_Header[44], LSB);
      bitmap_DIB_header_V5->BlueMask =        createDWORDfromBytes(&raw_DIB_Header[48], LSB);
      bitmap_DIB_header_V5->AlphaMask =       createDWORDfromBytes(&raw_DIB_Header[52], LSB);
      bitmap_DIB_header_V5->CSType =          createDWORDfromBytes(&raw_DIB_Header[56], LSB);
      
      bitmap_DIB_header_V5->RedX =            createDWORDfromBytes(&raw_DIB_Header[60], LSB);
      bitmap_DIB_header_V5->RedY =            createDWORDfromBytes(&raw_DIB_Header[64], LSB);
      bitmap_DIB_header_V5->RedZ =            createDWORDfromBytes(&raw_DIB_Header[68], LSB);
      bitmap_DIB_header_V5->GreenX =          createDWORDfromBytes(&raw_DIB_Header[72], LSB);
      bitmap_DIB_header_V5->GreenY =          createDWORDfromBytes(&raw_DIB_Header[76], LSB);
      bitmap_DIB_header_V5->GreenZ =          createDWORDfromBytes(&raw_DIB_Header[80], LSB);
      bitmap_DIB_header_V5->BlueX =           createDWORDfromBytes(&raw_DIB_Header[84], LSB);
      bitmap_DIB_header_V5->BlueY =           createDWORDfromBytes(&raw_DIB_Header[88], LSB);
      bitmap_DIB_header_V5->BlueZ =           createDWORDfromBytes(&raw_DIB_Header[92], LSB);
      
      bitmap_DIB_header_V5->GammaRed =        createDWORDfromBytes(&raw_DIB_Header[96], LSB);
      bitmap_DIB_header_V5->GammaGreen =      createDWORDfromBytes(&raw_DIB_Header[100], LSB);
      bitmap_DIB_header_V5->GammaBlue =       createDWORDfromBytes(&raw_DIB_Header[104], LSB);
      
      bitmap_DIB_header_V5->Intent =          createDWORDfromBytes(&raw_DIB_Header[108], LSB);
      bitmap_DIB_header_V5->ProfileData =     createDWORDfromBytes(&raw_DIB_Header[112], LSB);
      bitmap_DIB_header_V5->ProfileSize =     createDWORDfromBytes(&raw_DIB_Header[116], LSB);
      bitmap_DIB_header_V5->Reserved =        createDWORDfromBytes(&raw_DIB_Header[120], LSB);
      break;
    default:
      Serial.println("case None");
      Serial.println(_NotSupportedMessage);
      flag_DIBSupported = false;
      break;
  }
  }
  free(raw_DIB_Header);
  //Process DIB Header Done
    
  bitmapIMG.widthPx = bitmap_DIB_header_V5->Width;
  bitmapIMG.heightPx = bitmap_DIB_header_V5->Height;
  Serial.print("bitmap_DIB_header_V5->Height: ");Serial.println(bitmap_DIB_header_V5->Height);
  Serial.print("bitmap_DIB_header_V5->Width: ");Serial.println(bitmap_DIB_header_V5->Width);
  
  unsigned int rowPaddedSize = (32 - (bitmap_DIB_header_V5->Width * bitmap_DIB_header_V5->BitsPerPixel)%32)%32;//BMP Pixel array
  unsigned int newpaddedSize = (8 - (bitmapIMG.widthPx)%8)%8;
  Serial.print("rowPaddedSize: ");Serial.println(rowPaddedSize);
  Serial.print("newpaddedSize: ");Serial.println(newpaddedSize);
  unsigned int widthByteSizeBMP = (bitmap_DIB_header_V5->Width + rowPaddedSize) >> 3;//">>3" divides by 8
  unsigned int widthByteSizeNEW = (bitmapIMG.widthPx + newpaddedSize) >> 3;//">>3" divides by 8
  Serial.print("widthByteSizeBMP: ");Serial.println(widthByteSizeBMP);
  Serial.print("widthByteSizeNEW: ");Serial.println(widthByteSizeNEW);
  
  Serial.print("bitmap_DIB_header_V5->BitsPerPixel: ");Serial.println( bitmap_DIB_header_V5->BitsPerPixel );
  switch (bitmap_DIB_header_V5->BitsPerPixel) {
    case 0:
      break;
    case 1:
      bitmapIMG.imageByteLength = widthByteSizeNEW * bitmapIMG.heightPx;
      break;
    case 4:
      break;
    case 8:
      break;
    case 16:
      break;
    case 24:
      break;
    case 32:
      break;
  }
  Serial.print("bitmap_DIB_header_V5->Compression: ");Serial.println( bitmap_DIB_header_V5->Compression );
  //enum {BI_RGB, BI_RLE8, BI_RLE4, BI_BITFIELDS, BI_JPEG, BI_PNG, BI_CMYK, BI_CMYKRLE8, BI_CMYKRLE4} bV5Compression;
  switch (bitmap_DIB_header_V5->Compression) {
    case BI_RGB:
      //calculate image size if dummy value is found
      if (bitmap_DIB_header_V5->SizeOfBitmap==0){
        bitmap_DIB_header_V5->SizeOfBitmap = widthByteSizeBMP * bitmap_DIB_header_V5->Height;
      }
      break;
    case BI_RLE8:
      break;
    case BI_RLE4:
      break;
    case BI_BITFIELDS:
      break;
    case BI_JPEG:
      break;
    case BI_PNG:
      break;
    case BI_CMYK:
      break;
    case BI_CMYKRLE8:
      break;
    case BI_CMYKRLE4:
      break;
  }
  //read file's Color Table
  //No!
  
  //read file's pixel array
  Serial.println("-----read file's pixel array-----");
  Serial.print("bitmapHeader->File_Offset_to_PixelArray: ");Serial.println( bitmapHeader->File_Offset_to_PixelArray );
  (void)fileBMP.seek( bitmapHeader->File_Offset_to_PixelArray );
  Serial.print("fileBMP.position() Result: ");Serial.println( fileBMP.position() );
  
  unsigned char* raw_Pixel_Array = (unsigned char*)malloc(bitmap_DIB_header_V5->SizeOfBitmap + 1);
  fileBMP.read(raw_Pixel_Array, bitmap_DIB_header_V5->SizeOfBitmap);
  
  //convert BMP pixel array into useable Struct
  Serial.println("-----convert BMP pixel array into useable Struct-----");
  //bitmapIMG.pixelBitDataArray = (unsigned char*)malloc(bitmapIMG.imageByteLength);
  bitmapIMG.allocate(bitmap_DIB_header_V5->Width, bitmap_DIB_header_V5->Height);
  
  unsigned int index_0 = 0;
  unsigned int index_1 = 0;
  Serial.print("bitmap_DIB_header_V5->SizeOfBitmap: ");Serial.println(bitmap_DIB_header_V5->SizeOfBitmap);
  Serial.print("bitmapIMG.imageByteLength: ");Serial.println(bitmapIMG.imageByteLength);
  
  for (unsigned int indexH = 0; indexH < bitmapIMG.heightPx; indexH+=1) {
    index_0 = widthByteSizeNEW * indexH;
    index_1 = widthByteSizeBMP * (bitmapIMG.heightPx - indexH - 1);
    for (unsigned int indexW = 0; indexW < widthByteSizeNEW; indexW++) {
      bitmapIMG.pixelBitDataArray[ index_0 + indexW ] = raw_Pixel_Array[ index_1 + indexW ];
    }
  }
  Serial.println("-----Free Up Memory-----");
  free(raw_Pixel_Array);
  delete bitmap_DIB_header_V4;
  delete bitmap_DIB_header_V5;
  delete bitmap_DIB_header;
  delete bitmapHeader;
  Serial.println("-----Free Up Memory Complete-----");
  
  //finish Me and test Me
  Serial.print("bitmapIMG.widthPx: ");Serial.println(bitmapIMG.widthPx);
  Serial.print("bitmapIMG.heightPx: ");Serial.println(bitmapIMG.heightPx);
  Serial.print("bitmapIMG.imageByteLength: ");Serial.println(bitmapIMG.imageByteLength);
  printFreeHeap(Serial);
  Serial.println("-----(Func)(Leave)load_bitmapIMG_File_struct-----");
}


void load_struct_bitmapIMG_dat_File(File &fileBMP, IMGbitmapStruct &bitmapIMG);
void display_struct_bitmapIMG(Adafruit_SH1106 &display, IMGbitmapStruct &bitmapIMG, int locx, int locy);






/* Sorce: https://forum.arduino.cc/t/passing-struct-info-as-function-parameters/37330/2
    Sorce: https://forum.arduino.cc/t/passing-struct-info-as-function-parameters/37330/7
   The "&" effectively means that the original will be operated on rather than a copy that only exists in the scope of the function.
   References are a less powerful but far easier to use derivative of pointers, Google should find more about them if you want to know.
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
  bitmapIMG.pixelBitDataArray = (unsigned char*)malloc(bitmapIMG.imageByteLength);
  fileBMP.read(imageTemp, bitmapIMG.imageByteLength);
  for (int index = 0; index < bitmapIMG.imageByteLength; index++) {
    bitmapIMG.pixelBitDataArray[index] = imageTemp[index];
  }
  free(imageTemp);
}


void display_struct_bitmapIMG(Adafruit_SH1106 &display, IMGbitmapStruct &bitmapIMG, int locx, int locy) {
  display.drawBitmap(locx, locy, bitmapIMG.pixelBitDataArray, bitmapIMG.widthPx, bitmapIMG.heightPx, WHITE);
}


typedef struct {
  int x;
  int y;
  int z;
} Position3D_int_Struct;

typedef struct {
  long x;
  long y;
  long z;
} Position3D_long_Struct;

typedef struct {
  int x;
  int y;
} Position2D_int_Struct;

typedef struct {
  long x;
  long y;
} Position2D_long_Struct;




typedef struct {
  //uint16_t locxPxOffset = 0; //X Offset in pixels
  //uint16_t locyPxOffset = 0; //Y Offset in pixels
  Position2D_int_Struct locPxOffset;//X&Y Offset in pixels
  uint16_t widthPx;//in pixels(Global)
  uint16_t heightPx;//in pixels(Global)
  
  
  //subframeDataRotation is used if "rotationSFCompression" is Enabled
  uint8_t subframeDataRotation = 0; //data rotated Clockwise 4 phases
  /* subframeDataRotation
     0 == 0 CW rotations Needed
     1 == 3 CW rotations Needed
     2 == 2 CW rotations Needed
     3 == 1 CW rotations Needed
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
  if (strncpy(checkHeaderFormat, "datgif", 6) == 0) {
    formatCheck = true;
  } else {
    formatCheck = false;
  }


  unsigned char* imageTemp;

  unsigned int gifdataReadIndex = 0;
  unsigned int imagedataSize = 0;
  Serial.println("load_struct_bitmapGIF_dat_File -Check -00");
  printFreeHeap(Serial);
  if (formatCheck == false) {
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
    bitmapGIF.frameCount = int(filesize / (frameDataSize + 2));
    //bitmapGIF.frameDataPool = (unsigned char*)malloc(bitmapGIF.frameCount*(frameDataSize-2-2));
    Serial.print("bitmapGIF.frameDataPool: "); Serial.println( bitmapGIF.frameCount * (frameDataSize - 2 - 2) );


    //bitmapGIF.subFramePointers = (GIFframebitmapStruct*)malloc(bitmapGIF.frameCount);//Check This
    bitmapGIF.subFramePointers = (GIFframebitmapStruct*)malloc(bitmapGIF.frameCount*32);
    
    
    //bitmapGIF.subimageByteOffset = (uint16_t*)malloc(bitmapGIF.frameCount);
    //bitmapGIF.frameStaydelayms = (unsigned int*)malloc(bitmapGIF.frameCount);
    bitmapGIF.subframeCompressionFlags = 0b00000000;
    Serial.println("load_struct_bitmapGIF_dat_File -Check -02");
    gifdataReadIndex = 0;
    bitmapGIF.gifByteLength = 0;
    frameIndex = 0;
    while (file.available()) {
      Serial.print("load_struct_bitmapGIF_dat_File -Check -03 -"); Serial.println(frameIndex);
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
      bitmapGIF.subFramePointers[ frameIndex ].locPxOffset.x = 0;
      bitmapGIF.subFramePointers[ frameIndex ].locPxOffset.y = 0;
      display.clearDisplay();
      display.drawBitmap(0, 0, bitmapGIF.subFramePointers[ frameIndex ].frameDataPool, bitmapGIF.subFramePointers[ frameIndex ].widthPx, bitmapGIF.subFramePointers[ frameIndex ].heightPx, WHITE);
      display.display();

      Serial.println("load_struct_bitmapGIF_dat_File -Check -05");

      printFreeHeap(Serial);
      bitmapGIF.subFramePointers[ frameIndex ].frameStaydelayms = 1;
      gifdataReadIndex += bitmapGIF.globalsubimageByteLength;
      bitmapGIF.gifByteLength += bitmapGIF.globalsubimageByteLength;

      frameIndex++;
    }
    frameIndex--;
    Serial.print("frameIndex: "); Serial.println(frameIndex);
  }
  if (formatCheck == true) {
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
    bitmapGIF.cropSFCompression = (bitmapGIF.subframeCompressionFlags & 0b10000000) == 0b10000000;
    bitmapGIF.rotationSFCompression = (bitmapGIF.subframeCompressionFlags & 0b01000000) == 0b01000000;
    bitmapGIF.globalsubimageByteLength = ((file.read()) << 8) | (file.read());
    bitmapGIF.widthGPx = ((file.read()) << 8) | (file.read());
    bitmapGIF.heightGPx = ((file.read()) << 8) | (file.read());

    //allocate
    bitmapGIF.subFramePointers = (GIFframebitmapStruct*)malloc(bitmapGIF.frameCount*32);
    

    gifdataReadIndex = 0;
    imagedataSize = 0;
    for (int frameIndex = 0; frameIndex < bitmapGIF.frameCount; frameIndex++) {
      //subimageByteLength, subimageByteOffset, frameStaydelayms, locxPxOffset, locyPxOffset, widthPx, heightPx, subframeDataRotation
      
      if (bitmapGIF.cropSFCompression | bitmapGIF.rotationSFCompression) {
        bitmapGIF.subFramePointers[ frameIndex ].frameByteLength = ((file.read()) << 8) | (file.read());
        bitmapGIF.subFramePointers[ frameIndex ].widthPx = ((file.read()) << 8) | (file.read());
        bitmapGIF.subFramePointers[ frameIndex ].frameByteLength -= 2;
        bitmapGIF.subFramePointers[ frameIndex ].heightPx = ((file.read()) << 8) | (file.read());
        bitmapGIF.subFramePointers[ frameIndex ].frameByteLength -= 2;
      }
      if (bitmapGIF.cropSFCompression) {
        bitmapGIF.subFramePointers[ frameIndex ].locPxOffset.x = ((file.read()) << 8) | (file.read());
        bitmapGIF.subFramePointers[ frameIndex ].frameByteLength -= 2;
        bitmapGIF.subFramePointers[ frameIndex ].locPxOffset.y = ((file.read()) << 8) | (file.read());
        bitmapGIF.subFramePointers[ frameIndex ].frameByteLength -= 2;
      }
      if (bitmapGIF.rotationSFCompression) {
        bitmapGIF.subFramePointers[ frameIndex ].subframeDataRotation = file.read();
        bitmapGIF.subFramePointers[ frameIndex ].frameByteLength -= 1;
      }
      bitmapGIF.subFramePointers[ frameIndex ].frameStaydelayms = ((file.read()) << 8) | (file.read());
      bitmapGIF.subFramePointers[ frameIndex ].frameDataPool = (unsigned char*)malloc(bitmapGIF.subFramePointers[ frameIndex ].frameByteLength);
      if (bitmapGIF.cropSFCompression | bitmapGIF.rotationSFCompression) {
        bitmapGIF.subFramePointers[ frameIndex ].frameByteLength -= 2;

        gifdataReadIndex += bitmapGIF.subFramePointers[ frameIndex ].frameByteLength;
        imagedataSize += bitmapGIF.subFramePointers[ frameIndex ].frameByteLength;
        file.read( bitmapGIF.subFramePointers[ frameIndex ].frameDataPool, bitmapGIF.subFramePointers[ frameIndex ].frameByteLength );
      } else {
        bitmapGIF.subFramePointers[ frameIndex ].frameByteLength -= 2;
        
        gifdataReadIndex += bitmapGIF.subFramePointers[ frameIndex ].frameByteLength;
        imagedataSize += bitmapGIF.subFramePointers[ frameIndex ].frameByteLength;
        file.read( bitmapGIF.subFramePointers[ frameIndex ].frameDataPool, bitmapGIF.subFramePointers[ frameIndex ].frameByteLength );
      }

    }

    free(dataFileVersionString);
  }
  free(checkHeaderFormat);
  Serial.println("load_struct_bitmapGIF_dat_File: End");
}


void display_struct_bitmapGIF(Adafruit_SH1106 &display, GIFbitmapStruct &bitmapGIF, int frame, int locx, int locy) {
  Serial.println("display_struct_bitmapGIF");
  for (byte index = frame; index <= frame; index++) {
    printFreeHeap(Serial);
    Serial.print("index: "); Serial.println(index);
    //Serial.print("subimageByteOffset: "); Serial.println(bitmapGIF.subimageByteOffset[ index ]);
    Serial.print("frameCount: "); Serial.println(bitmapGIF.frameCount);
    Serial.print("gifByteLength: "); Serial.println(bitmapGIF.gifByteLength);

    if (bitmapGIF.cropSFCompression | bitmapGIF.rotationSFCompression) {
      unsigned char* imageTemp;
      imageTemp = (unsigned char*)malloc(bitmapGIF.subFramePointers[ index ].frameByteLength);
      Serial.print("bitmapGIF.subFramePointers[ index ].frameByteLength: "); Serial.println(bitmapGIF.subFramePointers[ index ].frameByteLength);
      memcpy(imageTemp, bitmapGIF.subFramePointers[ index ].frameDataPool, bitmapGIF.subFramePointers[ index ].frameByteLength);
      if (bitmapGIF.cropSFCompression) {
        display.drawBitmap(locx + bitmapGIF.subFramePointers[ index ].locPxOffset.x, locy + bitmapGIF.subFramePointers[ index ].locPxOffset.y, imageTemp, bitmapGIF.subFramePointers[ index ].widthPx, bitmapGIF.subFramePointers[ index ].heightPx, WHITE);
      } else if (bitmapGIF.rotationSFCompression) {
        //add rotate
        display.drawBitmap(locx, locy, imageTemp, bitmapGIF.subFramePointers[ index ].widthPx, bitmapGIF.subFramePointers[ index ].heightPx, WHITE);
      }
      free(imageTemp);
    } else {
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
      display.drawBitmap(bitmapGIF.subFramePointers[ index ].locPxOffset.x, bitmapGIF.subFramePointers[ index ].locPxOffset.y, bitmapGIF.subFramePointers[ index ].frameDataPool, bitmapGIF.subFramePointers[ index ].widthPx, bitmapGIF.subFramePointers[ index ].heightPx, WHITE);
    }
    printFreeHeap(Serial);
  }
  Serial.println("display_struct_bitmapGIF: End");
}



/*
   Struct Images and Animations: End
*/





void readbitmapdatFile(Adafruit_SH1106 &display, File &fileBMP);

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

void printFile(const char *filename, fs::FS &fs);


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
  if (storageDevice == STORAGE_SELECT_SD) {
    file = SD.open(filename);
  }
#endif
#if (STORAGE_INCLUDE_SPIFFS)
  if (storageDevice == STORAGE_SELECT_SPIFFS) {
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
  if (storageDevice == STORAGE_SELECT_SD) {
    SD.remove(filename);
  }
#endif
#if (STORAGE_INCLUDE_SPIFFS)
  if (storageDevice == STORAGE_SELECT_SPIFFS) {
    SPIFFS.remove(filename);
  }
#endif

  // Open file for writing
  File file;
#if (STORAGE_INCLUDE_SD)
  if (storageDevice == STORAGE_SELECT_SD) {
    file = SD.open(filename, FILE_WRITE);
  }
#endif
#if (STORAGE_INCLUDE_SPIFFS)
  if (storageDevice == STORAGE_SELECT_SPIFFS) {
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
  StaticJsonDocument<512> doc;
  
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
void printFile(const char *filename, fs::FS &fs) {
  // Open file for reading
  File file;
  file = fs.open(filename);
  
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







void test_bmp_image_file_read()
{
  File fileBMP;
  printFreeHeap(Serial);
  fileBMP = SPIFFS.open("/gigachad64h_Inverted.bmp");
  IMGbitmapStruct gigaChadTest;
  printFreeHeap(Serial);
  Serial.println("IMGbitmapStruct Test: Begin");
  
  load_bitmapIMG_File_struct(fileBMP, gigaChadTest);
  display.clearDisplay();
  display_struct_bitmapIMG(display, gigaChadTest, 0, 0);
  display.display();
  
  fileBMP.close();
  Serial.println("IMGbitmapStruct Test: Closed");
  printFreeHeap(Serial);

  delay(1000);
  
  printFreeHeap(Serial);
  fileBMP = SPIFFS.open("/SmallBatteryPercent.bmp");
  IMGbitmapStruct smallbatterypercentTest;
  printFreeHeap(Serial);
  Serial.println("IMGbitmapStruct Test: Begin");
  
  load_bitmapIMG_File_struct(fileBMP, smallbatterypercentTest);
  display.clearDisplay();
  display_struct_bitmapIMG(display, smallbatterypercentTest, 0, 0);
  display.display();
  
  fileBMP.close();
  Serial.println("IMGbitmapStruct Test: Closed");
  printFreeHeap(Serial);

  delay(1000);
  
  printFreeHeap(Serial);
  fileBMP = SPIFFS.open("/SmallBatteryPercent_2.bmp");
  IMGbitmapStruct smallbatterypercent2Test;
  printFreeHeap(Serial);
  Serial.println("IMGbitmapStruct Test: Begin");
  
  load_bitmapIMG_File_struct(fileBMP, smallbatterypercent2Test);
  display.clearDisplay();
  display_struct_bitmapIMG(display, smallbatterypercent2Test, 0, 0);
  display.display();
  
  fileBMP.close();
  Serial.println("IMGbitmapStruct Test: Closed");
  printFreeHeap(Serial);

  delay(1000);
  
  printFreeHeap(Serial);
  fileBMP = SPIFFS.open("/Neco-Arc_0.bmp");
  IMGbitmapStruct * NecoArcTest = new IMGbitmapStruct;
  printFreeHeap(Serial);
  Serial.println("IMGbitmapStruct Test: Begin");
  
  load_bitmapIMG_File_struct(fileBMP, *NecoArcTest);
  fileBMP.close();
  
  display.clearDisplay();
  display_struct_bitmapIMG(display, *NecoArcTest, 0, 0);
  display.display();
  
  Serial.println("IMGbitmapStruct Test: Closed");
  delete NecoArcTest;
  printFreeHeap(Serial);
}



void test_bitmapgif_dat_FBF()
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

void test_bitmapgif_dat_GIF()
{
  GIFbitmapStruct boyKisserGIFTest;
  File fileStructBMP;
  fileStructBMP = SPIFFS.open("/boyKisserFaceGif_bitmapgif.dat");
  load_struct_bitmapGIF_dat_File(fileStructBMP, boyKisserGIFTest);
  Serial.println(fileStructBMP);
  fileStructBMP.close();
  
  for(byte frame = 0; frame < boyKisserGIFTest.frameCount; frame++){
    display.clearDisplay();
    display_struct_bitmapGIF(display, boyKisserGIFTest, frame, 0, 0);
    Serial.print("Frame: ");Serial.print(frame);Serial.print("/");Serial.println(boyKisserGIFTest.frameCount);
    display.display();
  }
  //free(boyKisserGIFTest);
  Serial.println("IMGbitmapStruct Test: End");
}




void printParameters(Stream &serialport, struct Configuration configuration);
void printModuleInformation(Stream &serialport, struct ModuleInformation moduleInformation);


void printScrollTextLeft(Adafruit_SH1106 &display, char textInput[], unsigned int textMaxScrollLength, unsigned int frameOffset);





//https://www.freertos.org/a00106.html
TaskHandle_t Task1;
TaskHandle_t Task2;
TaskHandle_t Task3;
TaskHandle_t Task4;
TaskHandle_t Task5;

TaskHandle_t Task6;
TaskHandle_t Task7;
TaskHandle_t Task8;
TaskHandle_t Task9;
TaskHandle_t Task10;


TaskHandle_t TaskIRDecode;
TaskHandle_t Task_WiFi_LoRa_Bridge;
TaskHandle_t TaskWiFiMultiConnect;
TaskHandle_t TaskBuzzer;





//https://forum.arduino.cc/t/how-can-i-do-dual-core-task-use-same-memory-esp32-rtos/702929/24?page=2 //https://youtu.be/ywbq1qR-fY0?t=1206
QueueHandle_t queue;



//https://docs.espressif.com/projects/esp-idf/en/v4.3/esp32s2/api-reference/peripherals/index.html
void setup()   {
  Serial.begin(115200);
  Serial.print("setup() running on core "); Serial.println(xPortGetCoreID());
  printFreeHeap(Serial);
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
  pinMode(buzzerPin, OUTPUT);

  //BatterySensePin
  pinMode(batteryVoltageDivider, INPUT);

  // Startup all pins and UART
  e220ttl.begin();//LoRa Usage



  //Enable Devices
  adc_power_on();

  //https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/performance/size.html#idf-py-size
  printFreeHeap(Serial);
#if (FeatureEnable_BT_Serial==true)
  SerialBT.begin("ESP32btSerial"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  printFreeHeap(Serial);
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

  file = SPIFFS.open("/Wifi_Connections.json");
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
    Dictionary *buttonNamesDict = new Dictionary();//buttonNamesDict
    buttonNamesDict->insert("0", "key_Up");
    buttonNamesDict->insert("1", "key_Start");
    buttonNamesDict->insert("2", "key_Select");
    buttonNamesDict->insert("3", "key_A");
    buttonNamesDict->insert("4", "key_Left");
    buttonNamesDict->insert("5", "key_Down");
    buttonNamesDict->insert("6", "key_Right");
    buttonNamesDict->insert("7", "key_B");
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
  
  


  /*
   * Startup Tests Starts
   */
  printFreeHeap(Serial);
  delay(1000);
  test_bmp_image_file_read();
  delay(1000);
  printFreeHeap(Serial);
  delay(1000);
  
  test_bitmapgif_dat_FBF();
  
  delay(250);
  test_bitmapgif_dat_GIF();
  
  delay(250);
  
  
  printFreeHeap(Serial);
  /*
  {
  File fileBMP;
  fileBMP = SPIFFS.open("/boyKisserFaceGif_bitmapgif.dat");
  readbitmapdatFile(display, fileBMP);
  fileBMP.close();
  }
  delay(250);
  //*/
  
  /*
  {
  //Neco-Arc_bitmapimg.dat
  File fileBMP;
  fileBMP = SPIFFS.open("/Neco-Arc_bitmapimg.dat");
  readbitmapdatFile(display, fileBMP);
  fileBMP.close();
  }
  delay(250);
  */

  printFreeHeap(Serial);
  bool hexTest = false;//true;
  if (hexTest)
  {
  File fileBMP;
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
  }
  delay(250);
  
  {
  //load boy-kisser from sprite sheet
  Serial.println("-----Load Sprite of Boy-kisser Vertical-----");
  printFreeHeap(Serial);
  File fileBMP;
  fileBMP = SPIFFS.open("/BoyKisserSpriteVertical.bmp");
  IMGbitmapStruct * boyKisserSpriteSheet = new IMGbitmapStruct;
  load_bitmapIMG_File_struct(fileBMP, *boyKisserSpriteSheet);
  fileBMP.close();
  Serial.println("cutSpriteSheet");
  //cut sprite sheet
  IMGbitmapStruct boyKisserFrames[22];
  unsigned int spriteWidth = 85;
  unsigned int spriteHeight = 64;
  for(byte frame = 0; frame < 22; frame++){
    boyKisserFrames[frame].widthPx = spriteWidth;
    boyKisserFrames[frame].heightPx = spriteHeight;
    unsigned int framePaddingLength = (8 - boyKisserFrames[frame].widthPx%8);
    unsigned int widthByteLength = ((8 - (boyKisserFrames[frame].widthPx % 8)) + boyKisserFrames[frame].widthPx) >> 3;
    //create images
    boyKisserFrames[frame].imageByteLength = ( widthByteLength )*boyKisserFrames[frame].heightPx;
    //boyKisserFrames[frame].pixelBitDataArray = (unsigned char*)malloc( boyKisserFrames[frame].imageByteLength );
    boyKisserFrames[frame].allocate(spriteWidth, spriteHeight);
    memcpy(boyKisserFrames[frame].pixelBitDataArray, &boyKisserSpriteSheet->pixelBitDataArray[ boyKisserFrames[frame].imageByteLength * frame ], boyKisserFrames[frame].imageByteLength);
  }
  delete boyKisserSpriteSheet;
  for(byte frame = 0; frame < 22; frame++){
    display.clearDisplay();
    display_struct_bitmapIMG(display, boyKisserFrames[frame], 0, 0);
    display.display();
    delay(100);
  }
  
  printFreeHeap(Serial);
  Serial.println("-----Continuing from Sprite of Boy-kisser Vertical-----");
  delay(1000);
  }
  
  delay(1000);
  
  bool rotTest = true;//true;
  if (rotTest)
  {
  //"/boyKisserFaceGif_bitmapgif.txt"
  File fileBMP;
  fileBMP = SPIFFS.open("/boyKisserFaceGif_bitmapgif.txt");
  if (!fileBMP) {
    Serial.println("Failed to open file for reading");
    return;
  }
  Serial.println("File Content:");
  
  
  //init buffers
  char filereadBuffer[2048];
  char fileImageBuffer[2048];
  memset(filereadBuffer, '\0', 2048);
  memset(fileImageBuffer, '\0', 2048);
  //image Basic Test Loop
  while (fileBMP.available()) {
    fileBMP.readBytesUntil('\n', filereadBuffer, 2048);
    fileBMP.readBytesUntil('\n', fileImageBuffer, 2048);
    display.clearDisplay();
    display.drawBitmap(0, 0, convertTextHextoCharArray( fileImageBuffer ), 85, 64, WHITE);//Test Image Read
    ///*
    printFreeHeap(Serial);
    unsigned char* rotateImageTest;
    rotateImageTest = (unsigned char *) calloc(4 * 32, sizeof(unsigned char *)); //32*32
    bitmapIMGmanipulate0.rotateBitImageClockwise( rotateImageTest, (unsigned char*)rotateImageTestIcon, 32, 32);//rotates 90 deg clockwise //new Px Width and Height will be flipped
    display.drawBitmap(90, 0, rotateImageTest, 32, 32, WHITE);//Test Image Read
    //*/
    display.display();
    Serial.println("Displayed");
    //delay(2000);
    ///*
    printFreeHeap(Serial);
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
  }

  printFreeHeap(Serial);
  /*
   * Startup Tests Ends
   */

  /*
   * Load Sprites Start
   */
  {
    File fileBMP;
    fileBMP = SPIFFS.open("/batteryBig_Full.bmp");
    load_bitmapIMG_File_struct(fileBMP, batteryIconBig);
    fileBMP.close();
  }
  {
    File fileBMP;
    fileBMP = SPIFFS.open("/Neco-Arc_0.bmp");
    load_bitmapIMG_File_struct(fileBMP, visableCharacterBuffer[ 0 ]);
    fileBMP.close();
  }
  
  /*
   * Load Sprites End
   */
  
  
  display.clearDisplay();
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
  printFile(filename, SPIFFS);
  
  
  
  Serial.println("Starting WiFi Setup!");
  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, local_gateway, local_subnet) ? "Ready" : "Failed!");
  
  //WiFi Setup
  //https://www.upesy.com/blogs/tutorials/how-to-connect-wifi-acces-point-with-esp32
  //disable_STA_WiFi();
  //enable_STA_WiFi();
  
  WiFi.onEvent(OnWiFiEvent);

  
  printFile("/Wifi_Connections.json", SPIFFS);
  //Load WiFiCredentialsList
  Serial.println(F("Loading WiFi configuration..."));
  loadWiFiCredentialsList("/Wifi_Connections.json", wifiCredentialList, SPIFFS);
  //Save WiFiCredentialsList
  Serial.println(F("Saving WiFi configuration..."));
  saveWiFiCredentialsList("/Wifi_Connections.json", wifiCredentialList, SPIFFS);
  printFile("/Wifi_Connections.json", SPIFFS);
  
  Serial.println(F("Reading WiFi configuration..."));
  
  for (int index = 0; index < wifiCredentialList.length; index++) { 
    Serial.print("ssid: ");Serial.println(wifiCredentialList.credentials[ index ].ssid);
    Serial.print("pass: ");Serial.println(wifiCredentialList.credentials[ index ].pass);
    // Fix me
    wifiMulti.addAP(wifiCredentialList.credentials[ index ].ssid, wifiCredentialList.credentials[ index ].pass);
    Serial.println();
    delay(100);
  }
  
  startSoftAP( Serial );
  SerialWiFiserver.begin();
  //WiFi Setup Complete
  Serial.println("Complete WiFi Setup!");
  
  
  
  // Connect to Wi-Fi using wifiMulti (connects to the SSID with strongest connection)
  Serial.println("Connecting Wifi...");
  ///*
  if(wifiMulti.run() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
  //*/
  
  
  /////FTP Setup, ensure SPIFFS is started before ftp;  /////////
  
  /////FTP Setup, ensure SPIFFS is started before ftp;  /////////
#ifdef ESP32       //esp32 we send true to format spiffs if cannot mount
  if (SPIFFS.begin(true)) {
#elif defined ESP8266
  if (SPIFFS.begin()) {
#endif
      Serial.println("SPIFFS opened!");
      ftpSrv.begin("esp8266","esp8266");    //username, password for ftp.  set ports in ESP8266FtpServer.h  (default 21, 50009 for PASV)
  } 
  
  
  /*
     Task Handles
  */
  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
    Task1code,   /* Task function. */
    "BatteryStatusCheck",     /* name of task. */
    2048,       /* Stack size of task */
    NULL,        /* parameter of the task */
    5,           /* priority of the task */
    &Task1,      /* Task handle to keep track of created task */
    0);          /* pin task to core 0 */
  delay(500);

  //create a task that will be executed in the Task2code() function, with priority 6 and executed on core 0
  xTaskCreatePinnedToCore(
    Task2code,   /* Task function. */
    "GetTime",     /* name of task. */
    4096,       /* Stack size of task */
    NULL,        /* parameter of the task */
    6,           /* priority of the task */
    &Task2,      /* Task handle to keep track of created task */
    0);          /* pin task to core 0 */
  delay(500);

  //create a task that will be executed in the Task3code() function, with priority 99 and executed on core 0
  xTaskCreatePinnedToCore(
    TaskIRDecodeFunc,   /* Task function. */
    "TaskIRDecodeFunc",     /* name of task. */
    1000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    99,           /* priority of the task */
    &TaskIRDecode,      /* Task handle to keep track of created task */
    0);          /* pin task to core 0 */
  delay(500);
  
  //create a task that will be executed in the Task3code() function, with priority 2 and executed on core 0
  xTaskCreatePinnedToCore(
    TaskWiFiLoRaBridgeFunc,   /* Task function. */
    "TaskWiFiLoRaBridgeFunc",     /* name of task. */
    2000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    2,           /* priority of the task */
    &Task_WiFi_LoRa_Bridge,      /* Task handle to keep track of created task */
    0);          /* pin task to core 0 */
  delay(500);

  //create a task that will be executed in the Task3code() function, with priority 99 and executed on core 0
  xTaskCreatePinnedToCore(
    TaskWiFiMultiConnectFunc,   /* Task function. */
    "TaskWiFiMultiConnectFunc",     /* name of task. */
    2000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    99,           /* priority of the task */
    &TaskWiFiMultiConnect,      /* Task handle to keep track of created task */
    0);          /* pin task to core 0 */
  delay(500);

  
  

  printFreeHeap(Serial);
  Serial.println("Exiting Setup!");
  delay(1250);
}

//https://techtutorialsx.com/2017/05/06/esp32-arduino-creating-a-task/
//https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos_idf.html

void Task1code( void * pvParameters ) {
  Serial.print("Task1 running on core "); Serial.println(xPortGetCoreID());
  //BatteryStatusCheck
  double vin;
  double maxVin;
  double minVin;
  for (;;) {
    if (autoUPdateBatteryVariables){
      //Calculate Battery Percentage
      
      vin = calculateVoltageDivider(100000, 100000, batteryVoltageDivider);
      maxVin = 4.1;
      minVin = 3.3;
      /*
      double vin = calculateVoltageDivider(100000, 100000, batteryVoltageDivider);
      double maxVin = 4.1;
      double minVin = 3.3;
      //*/
      batteryPercent = (byte)mapdouble((double)vin, (double)minVin, (double)maxVin, (double)0.0, (double)100.0);
      batteryVoltage = vin;
      chargingFlag = false;
      //Serial.print("batteryPercent%:");Serial.println(batteryPercent);
    }
    delay(1000);
  }
  Serial.println("Ending Task1");
  vTaskDelete( NULL );
}

void Task2code( void * pvParameters ) {
  Serial.print("Task2 running on core "); Serial.println(xPortGetCoreID());
  //GetTime
  for (;;) {
    printLocalTime(Serial);
    delay(10000);
  }
  Serial.println("Ending Task2");
  vTaskDelete( NULL );
}


void Task4code( void * pvParameters ) {
  Serial.print("Task4 running on core "); Serial.println(xPortGetCoreID());
  for (;;) {
    delay(4000);
  }
  Serial.println("Ending Task4");
  vTaskDelete( NULL );
} 

void Task5code( void * pvParameters ) {
  Serial.print("Task5 running on core "); Serial.println(xPortGetCoreID());
  delay(1000);
  Serial.println("Ending Task5");
  vTaskDelete( NULL );
}


void TaskIRDecodeFunc( void * pvParameters ) {
  Serial.print("TaskIRDecodeFunc running on core "); Serial.println(xPortGetCoreID());
  for (;;) {
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
    delay(3000);
  }
  Serial.println("Ending TaskIRDecodeFunc");
  vTaskDelete( NULL );
}



void TaskWiFiLoRaBridgeFunc( void * pvParameters ) {
  //Mesh
  Serial.print("TaskWiFiBridgeSTAtoAPFunc running on core "); Serial.println(xPortGetCoreID());
  Serial.print("TaskWiFiBridgeLoRatoAPFunc running on core "); Serial.println(xPortGetCoreID());
  Serial.print("TaskWiFiBridgeSTAtoLoRaFunc running on core "); Serial.println(xPortGetCoreID());
  //https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/esp-wifi-mesh.html#overview
  //https://randomnerdtutorials.com/esp-mesh-esp32-esp8266-painlessmesh/
  
  //https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_wifi.html
  //https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/wifi.html
  //https://randomnerdtutorials.com/esp32-useful-wi-fi-functions-arduino/
  //https://randomnerdtutorials.com/esp32-wifimulti/
  
  //https://randomnerdtutorials.com/esp32-access-point-ap-web-server/
  
  for (;;) {
    //https://espressif-docs.readthedocs-hosted.com/projects/arduino-esp32/en/latest/api/wifi.html
    uint8_t softAP_getStationNum = WiFi.softAPgetStationNum();
    Serial.print("Stations connected: ");
    Serial.println(softAP_getStationNum);
    IPAddress softAP_BroadcastIP = WiFi.softAPBroadcastIP();
    Serial.print("softAPBroadcastIP: ");
    Serial.println(softAP_BroadcastIP);
    IPAddress softAP_IP = WiFi.softAPIP();
    Serial.print("softAPIP: ");
    Serial.println(softAP_IP);
    IPAddress softAP_NetworkID = WiFi.softAPNetworkID();
    Serial.print("softAPNetworkID: ");
    Serial.println(softAP_NetworkID);
    delay(750);
  }
  Serial.println("Ending TaskWiFiBridgeSTAtoAPFunc");
  vTaskDelete( NULL );
}


void TaskWiFiMultiConnectFunc( void * pvParameters ) {
  Serial.print("TaskWiFiMultiConnectFunc running on core "); Serial.println(xPortGetCoreID());
  for (;;) {
    //if the connection to the stongest hotstop is lost, it will connect to the next network on the list
    ///*
    if (wifiMulti.run(connectTimeoutMs) == WL_CONNECTED) {
      Serial.print("WiFi connected: ");
      Serial.print(WiFi.SSID());
      Serial.print(" ");
      Serial.println(WiFi.RSSI());
    }
    else {
      Serial.println("WiFi not connected!");
    }
    //*/
    delay(10000);
  }
  Serial.println("Ending TaskWiFiMultiConnectFunc");
  vTaskDelete( NULL );
}


void TaskBuzzercode( void * pvParameters ) {
  //https://techtutorialsx.com/2017/05/07/esp32-arduino-passing-a-variable-as-argument-of-a-freertos-task/
  Serial.print("TaskBuzzer running on core "); Serial.println(xPortGetCoreID());
  Serial.println(*((char*)pvParameters));
  char *cName = ((char*)pvParameters);
  Serial.println(cName);
  //Work on Me!
  int *sound;
  sound = (int*)calloc(1024, sizeof(int));
  /*
  for (int index = 0; index < 1024; index++) {
    sound[ index ] = 0b0000111111111111;
  }
  */
  for (int index = 0; index < 1024; index++) {
    analogWrite(buzzerPin, sound[ index ]);
    delayMicroseconds(10);
  }
  free(sound);
  Serial.println("Ending TaskBuzzer");
  vTaskDelete( NULL );
}



static const unsigned char PROGMEM buttonInputIcon4x8[2][8] =
{
  {
    B11110000,
    B01100000,
    B11110000,
    B01100000,
    B11110000,
    B01100000,
    B11110000,
    B01100000,
  },
  {
    B11110000,
    B01100000,
    B00000000,
    B10010000,
    B01100000,
    B00000000,
    B10010000,
    B01100000,
  },
};



//Try esp32's inner Temp Sensor


void loop()
{
  // put your main code here, to run repeatedly:
  //Serial.print("loop() running on core "); Serial.println(xPortGetCoreID());
  Serial.print("Main_loop() running on core "); Serial.println(xPortGetCoreID());
  
  
  //calculate FPS
  system_Frame_FPS = (double)(1 / ((double)(frameEndTime - frameStartTime) / 1000000));
  //\/ start Frame Here \/
  frameStartTime = micros();

  ftpSrv.handleFTP();        //make sure in loop you call handleFTP()!!  
  // server.handleClient();   //example if running a webserver you still need to call .handleClient();
  

  char localIntVar[64];
  strcpy(localIntVar, "Buzzer.pwm");
  //create a task that will be executed in the TaskBuzzercode() function, with priority 20 and executed on core 1
  if ((frameCountVariable % 65536) == 0){
  xTaskCreatePinnedToCore(
    TaskBuzzercode,   /* Task function. */
    "TaskBuzzercode",     /* name of task. */
    8192,       /* Stack size of task */
    (void*)&localIntVar[0],        /* parameter of the task */
    20,           /* priority of the task */
    &TaskBuzzer,      /* Task handle to keep track of created task */
    1);          /* pin task to core 1 */
  }  

  if (timeConfigured==false){
    if (WiFi.status() == WL_CONNECTED){
      configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
      timeConfigured = true;
    }
  }
  
  
  
  
  if (disableButtonPISO_update == false) {
    buttonpiso1.update();
  };
  display.clearDisplay();
  display.setCursor(0, 0);
  
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
    display.print("CurrentMenuID:"); display.println(currentMenuID, DEC);
    
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.print("DeBug_Status:"); display.println(showDebugMenu);
  }

  
  //main Menu //currentMenuID==0
  if (currentMenuID == 0) {
    menu_MAIN();
  }
  //selection menu //currentMenuID==1
  if (currentMenuID == 1) {
    menu_SELECTION();
  }
  //Lora menu //currentMenuID==2
  if (currentMenuID == 2) {
    menu_LORA();
  }


  //Wifi menu //currentMenuID==3
  if (currentMenuID == 3) {
    menu_WIFI();
    
  }

  //IR menu //currentMenuID==5
  if (currentMenuID == 5) {
    menu_IR();    
  }



  //SPIFFs menu //currentMenuID==8
  if (currentMenuID == 8) {
    menu_SPIFFS();
  }

  //IP menu //currentMenuID==10
  if (currentMenuID == 10) {
    menu_IP();
  }
  //Composite Video Test //currentMenuID==11
  if (currentMenuID == 11) {
    //Add Me As an Option
    ;
  }
  
  
  {
  if (buttonpiso1.isPressed(2) == true) {//Select
    batteryPercent = 100;
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
  }
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
  if (!(currentMenuID==0 | currentMenuID==1 | currentMenuID==2)){
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.print("currentMenuID:"); display.println(currentMenuID);
  }
  
  display.setCursor(5, display.getCursorY());
  printScrollTextLeft(display, "OwO123456789ABCDEFOwOabcdef", 5, frameCountVariable);
  
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
  
  checkAllDebugInputs();
  
  printFreeHeap(Serial);
  printHeapFrag(Serial);
  display.print("FreeHeap:"); display.print(ESP.getFreeHeap()); display.print(" B"); display.println();
  display.print("HeapFrag:"); display.print(getFragmentation()); display.print("%"); display.println();
  display.print("AP IP address: "); display.print( WiFi.softAPIP() ); display.println();
  
  //update some variables

  //Draw Button Press GUI
  if (buttonpiso1.getRAWState()!=0b00000000){
    int bGUI_PosX = 0;
    int bGUI_PosY = 46;
    int bGUI_LenX = 30;
    int bGUI_LenY = 17;
    //Position2D_int_Struct bGUIPoint0 = {bGUI_PosX, bGUI_PosY};
    Position2D_int_Struct bGUIPoint0;
    bGUIPoint0.x = bGUI_PosX;
    bGUIPoint0.y = bGUI_PosY;
    Position2D_int_Struct bGUIPoint1;
    bGUIPoint1.x = bGUI_PosX+8;
    bGUIPoint1.y = bGUI_PosY;
    Position2D_int_Struct bGUIPoint2;
    bGUIPoint2.x = bGUI_PosX+11;
    bGUIPoint2.y = bGUI_PosY+4;
    Position2D_int_Struct bGUIPoint3;
    bGUIPoint3.x = bGUI_PosX;
    bGUIPoint3.y = bGUI_PosY+bGUI_LenY;
    Position2D_int_Struct bGUIPoint4;
    bGUIPoint4.x = bGUI_PosX+bGUI_LenX;
    bGUIPoint4.y = bGUI_PosY+4;
    Position2D_int_Struct bGUIPoint5;
    bGUIPoint5.x = bGUI_PosX+bGUI_LenX;
    bGUIPoint5.y = bGUI_PosY+bGUI_LenY-5;
    Position2D_int_Struct bGUIPoint6;
    bGUIPoint6.x = bGUI_PosX+bGUI_LenX-5;
    bGUIPoint6.y = bGUI_PosY+bGUI_LenY;

    display.fillTriangle(bGUIPoint0.x, bGUIPoint0.y,
                         bGUIPoint1.x, bGUIPoint1.y,
                         bGUIPoint2.x, bGUIPoint2.y, WHITE);
    display.fillTriangle(bGUIPoint0.x, bGUIPoint0.y,
                         bGUIPoint3.x, bGUIPoint3.y,
                         bGUIPoint2.x, bGUIPoint2.y, WHITE);
    display.fillTriangle(bGUIPoint2.x, bGUIPoint2.y,
                         bGUIPoint4.x, bGUIPoint4.y,
                         bGUIPoint5.x, bGUIPoint5.y, WHITE);
    display.fillTriangle(bGUIPoint2.x, bGUIPoint2.y,
                         bGUIPoint6.x, bGUIPoint6.y,
                         bGUIPoint3.x, bGUIPoint3.y, WHITE);
    display.fillTriangle(bGUIPoint2.x, bGUIPoint2.y,
                         bGUIPoint5.x, bGUIPoint5.y,
                         bGUIPoint6.x, bGUIPoint6.y, WHITE);

    display.fillTriangle(bGUIPoint0.x+1, bGUIPoint0.y+1,
                         bGUIPoint1.x-1, bGUIPoint1.y+1,
                         bGUIPoint2.x-1, bGUIPoint2.y+1, BLACK);
    display.fillTriangle(bGUIPoint0.x+1, bGUIPoint0.y+1,
                         bGUIPoint3.x+1, bGUIPoint3.y-1,
                         bGUIPoint2.x-1, bGUIPoint2.y+1, BLACK);
    display.fillTriangle(bGUIPoint2.x-1, bGUIPoint2.y+1,
                         bGUIPoint4.x-1, bGUIPoint4.y+1,
                         bGUIPoint5.x-1, bGUIPoint5.y+0, BLACK);
    display.fillTriangle(bGUIPoint2.x-1, bGUIPoint2.y+1,
                         bGUIPoint6.x+0, bGUIPoint6.y-1,
                         bGUIPoint3.x+1, bGUIPoint3.y-1, BLACK);
    display.fillTriangle(bGUIPoint2.x-1, bGUIPoint2.y+1,
                         bGUIPoint5.x-1, bGUIPoint5.y+0,
                         bGUIPoint6.x+0, bGUIPoint6.y-1, BLACK);
    
    Position2D_int_Struct bGUIInputIconPos;
    bGUIInputIconPos.x = bGUI_PosX+2;
    bGUIInputIconPos.y = bGUI_PosY+2;
    Position2D_int_Struct bGUIInputIconLength;
    bGUIInputIconLength.x = 4;
    bGUIInputIconLength.y = 8;
    
    //if buttonInput Mode is HTTPS
    display.fillRect(bGUIInputIconPos.x, bGUIInputIconPos.y, bGUIInputIconLength.x, bGUIInputIconLength.y, BLACK);
    display.drawBitmap(bGUIInputIconPos.x, bGUIInputIconPos.y, buttonInputIcon4x8[1], bGUIInputIconLength.x, bGUIInputIconLength.y, WHITE);
    
    //if buttonInput Mode is PISO
    display.fillRect(bGUIInputIconPos.x, bGUIInputIconPos.y, bGUIInputIconLength.x, bGUIInputIconLength.y, BLACK);
    display.drawBitmap(bGUIInputIconPos.x, bGUIInputIconPos.y, buttonInputIcon4x8[0], bGUIInputIconLength.x, bGUIInputIconLength.y, WHITE);
    
    
    Position2D_int_Struct bGUIButtonPos;
    bGUIButtonPos.x = bGUI_PosX+2+1;
    bGUIButtonPos.y = bGUI_PosY+8-2;
    
    //Start
    display.fillRect(bGUIButtonPos.x+11+0, bGUIButtonPos.y+0+0, 4, 4, WHITE);
    display.fillRect(bGUIButtonPos.x+11+1, bGUIButtonPos.y+0+1, 2, 2, BLACK);
    //Select
    display.fillRect(bGUIButtonPos.x+16+0, bGUIButtonPos.y+0+0, 4, 4, WHITE);
    display.fillRect(bGUIButtonPos.x+16+1, bGUIButtonPos.y+0+1, 2, 2, BLACK);
    
    //UP
    display.fillRect(bGUIButtonPos.x+5+0, bGUIButtonPos.y+0+0, 4, 4, WHITE);
    display.fillRect(bGUIButtonPos.x+5+1, bGUIButtonPos.y+0+1, 2, 2, BLACK);
    //LEFT
    display.fillRect(bGUIButtonPos.x+0+0, bGUIButtonPos.y+5+0, 4, 4, WHITE);
    display.fillRect(bGUIButtonPos.x+0+1, bGUIButtonPos.y+5+1, 2, 2, BLACK);
    //DOWN
    display.fillRect(bGUIButtonPos.x+5+0, bGUIButtonPos.y+5+0, 4, 4, WHITE);
    display.fillRect(bGUIButtonPos.x+5+1, bGUIButtonPos.y+5+1, 2, 2, BLACK);
    //RIGHT
    display.fillRect(bGUIButtonPos.x+10+0, bGUIButtonPos.y+5+0, 4, 4, WHITE);
    display.fillRect(bGUIButtonPos.x+10+1, bGUIButtonPos.y+5+1, 2, 2, BLACK);
    
    //A
    display.fillRect(bGUIButtonPos.x+16+0, bGUIButtonPos.y+5+0, 4, 4, WHITE);
    display.fillRect(bGUIButtonPos.x+16+1, bGUIButtonPos.y+5+1, 2, 2, BLACK);
    //B
    display.fillRect(bGUIButtonPos.x+21+0, bGUIButtonPos.y+2+0, 4, 4, WHITE);
    display.fillRect(bGUIButtonPos.x+21+1, bGUIButtonPos.y+2+1, 2, 2, BLACK);
    
    if (buttonpiso1.isPressed(0) == true) {//UP
      //UP
      display.fillRect(bGUIButtonPos.x+5+0, bGUIButtonPos.y+0+0, 4, 4, WHITE);
    };
    if (buttonpiso1.isPressed(1) == true) {//Start
      //Start
      display.fillRect(bGUIButtonPos.x+11+0, bGUIButtonPos.y+0+0, 4, 4, WHITE);
    };
    if (buttonpiso1.isPressed(2) == true) {//Select
      //Select
      display.fillRect(bGUIButtonPos.x+16+0, bGUIButtonPos.y+0+0, 4, 4, WHITE);
    };
    if (buttonpiso1.isPressed(3) == true) {//A
      //A
      display.fillRect(bGUIButtonPos.x+16+0, bGUIButtonPos.y+5+0, 4, 4, WHITE);
    };
    if (buttonpiso1.isPressed(4) == true) {//LEFT
      //LEFT
      display.fillRect(bGUIButtonPos.x+0+0, bGUIButtonPos.y+5+0, 4, 4, WHITE);
    };
    if (buttonpiso1.isPressed(5) == true) {//DOWN
      //DOWN
      display.fillRect(bGUIButtonPos.x+5+0, bGUIButtonPos.y+5+0, 4, 4, WHITE);
    };
    if (buttonpiso1.isPressed(6) == true) {//RIGHT
      //RIGHT
      display.fillRect(bGUIButtonPos.x+10+0, bGUIButtonPos.y+5+0, 4, 4, WHITE);
    };
    if (buttonpiso1.isPressed(7) == true) {//B
      //B
      display.fillRect(bGUIButtonPos.x+21+0, bGUIButtonPos.y+2+0, 4, 4, WHITE);
    };
  }
  
  //update Frame
  display.display();
  //update frame count variable
  frameCountVariable++;
  //update frame End Time
  frameEndTime = micros();
}





/*
void display_connected_devices()
{
  //https://www.upesy.com/blogs/tutorials/how-create-a-wifi-acces-point-with-esp32
    wifi_sta_list_t wifi_sta_list;
    tcpip_adapter_sta_list_t adapter_sta_list;
    esp_wifi_ap_get_sta_list(&wifi_sta_list);
    tcpip_adapter_get_sta_list(&wifi_sta_list, &adapter_sta_list);

    if (adapter_sta_list.num > 0)
        Serial.println("-----------");
    for (uint8_t i = 0; i < adapter_sta_list.num; i++)
    {
        tcpip_adapter_sta_info_t station = adapter_sta_list.sta[i];
        Serial.print((String)"[+] Device " + i + " | MAC : ");
        Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X", station.mac[0], station.mac[1], station.mac[2], station.mac[3], station.mac[4], station.mac[5]);
        Serial.println((String) " | IP " + ip4addr_ntoa(&(station.ip)));
    }
}
//*/





void printScrollTextLeft(Adafruit_SH1106 &display, char textInput[], unsigned int textMaxScrollLength, unsigned int frameOffset)
{
  //uses (Standard ASCII 5x7 font) for Charater size.
  //char textInput[]; //Text Input
  //unsigned int textMaxScrollLength; //Length of Text from Input to scroll!
  //unsigned int frameOffset; //global frame count since start
  const byte charaSX = 5;
  const byte charaSY = 7;
  //currently only can only scroll from start to end going Left.
  unsigned int frameStartScroll_Index = 0;//specify what frame the function was first called.
  /*
   * The Start frame the scroll math will be base off will always be 0
   */
  byte pixelSpeedFrame = 1;//how many pixels does the text move in a single frame!
  unsigned int localFrameOffset = frameOffset - frameStartScroll_Index;
  if (frameOffset < frameStartScroll_Index){
    localFrameOffset = 0;
  }
  //fix sync issue
  //Work on (oct 20 2023)
  bool printDebugText = false;
  if (printDebugText){
    Serial.printf("-----FUNC(printScrollTextLeft(display, %s, %i, %i))-----\n", textInput, textMaxScrollLength, frameOffset );
  }
  char textInputShort[textMaxScrollLength+1];
  unsigned int maxlocalFrame = ((strlen(textInput)-textMaxScrollLength)*(charaSX*pixelSpeedFrame));
  unsigned int localFrame = (localFrameOffset)%(maxlocalFrame);
  unsigned int textStartIndex = (localFrame)/(charaSX*pixelSpeedFrame);
  if (printDebugText){
    Serial.printf("maxlocalFrame= %d\n", maxlocalFrame);
    Serial.printf("localFrame= %d\n", localFrame);
    Serial.printf("textStartIndex= %d, textInputShort= \"%s\"\n", textStartIndex, textInputShort);
  }
  memset(textInputShort, '\0', textMaxScrollLength+1);
  strncpy(textInputShort, &textInput[ textStartIndex ], textMaxScrollLength);
  if (printDebugText){
    Serial.printf("&textInput[ textStartIndex = %d ]= \"%s\" = textInputShort= \"%s\", textMaxScrollLength= %d \n", textStartIndex, &textInput[ textStartIndex ], textInputShort, textMaxScrollLength);
  }
  unsigned int setCursorX = display.getCursorX() - localFrame%(charaSX*pixelSpeedFrame);
  unsigned int setCursorY = display.getCursorY();
  if (printDebugText){
    Serial.printf("setCursorX= %d , setCursorY= %d, textInputShort= \"%s\"\n", setCursorX, setCursorY, textInputShort);
  }
  display.setCursor(setCursorX, setCursorY);
  display.println(textInputShort);
}

void printFreeHeap(Stream &serialport) {
  bool showMSGIdentifierOnPrint = false;
  if (showMSGIdentifierOnPrint){
    serialport.print("-----ESP_STAT-----");
  }
  //-----ESP_STAT-----
  serialport.print("ESP.getFreeHeap():");
  serialport.println(ESP.getFreeHeap());
}

void printHeapFrag(Stream &serialport) {
  bool showMSGIdentifierOnPrint = false;
  if (showMSGIdentifierOnPrint){
    serialport.print("-----ESP_STAT-----");
  }
  //-----ESP_STAT-----
  serialport.print("HeapFrag:"); serialport.print(getFragmentation()); serialport.print("%"); serialport.println();
}  

void menu_MAIN(){
  display.clearDisplay();
  {
    display.drawRoundRect(0, 2 - 8, 128, 16 - 2 + 8, 4, WHITE);
    display.fillRect(0, 0, 48 + 8, 16, BLACK);
    display.drawLine(0, 7, 47, 7, WHITE);
    display.drawLine(47, 7, 47 + 8, 7 + 8, WHITE);
  }
  {
    display_struct_bitmapIMG(display, batteryIconBig, batteryXpos, batteryYpos);
    //display.drawBitmap(batteryXpos, batteryYpos, batteryIconBigImage, batteryIconBigWidth, batteryIconBigHeight, WHITE);
    if (batteryVoltage == 0.0){
      display.fillRect(batteryXpos + 4, batteryYpos + 2, 20, 8, BLACK);
    }else{
      display.fillRect(batteryXpos + 4, batteryYpos + 2, (double)((batteryPercent) * (20 / 100)), 8, BLACK);
    }
  }
  
  display.fillRect(80, 0, 16, 16+16, BLACK);
  if ((WiFi.getMode() == WIFI_MODE_STA) or (WiFi.getMode() == WIFI_MODE_APSTA)) {
    display.fillRect(80, 0, 16, 16, WHITE);
    display.drawBitmap(80, 0, wifi16x16Icon, 16, 16, BLACK);
    if (WiFi.status() == WL_CONNECTED) {
      display.fillRect(80, 0, 16, 16, BLACK);
      display.drawBitmap(80, 0, wifi16x16Icon, 16, 16, WHITE);
    }
  }
  if ((WiFi.getMode() == WIFI_MODE_AP) or (WiFi.getMode() == WIFI_MODE_APSTA)) {
    display.drawBitmap(80, 16, wifi16x16Icon, 16, 16, WHITE);
  }
  //batteryPercent, chargingFlag
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.print("FPS~:"); display.print( system_Frame_FPS ); display.println();
  //display.print("RawButtons:"); display.println(buttonpiso1.getRAWState(), BIN);
  display.print("battery:"); display.print(batteryPercent); display.print("%, "); display.print(batteryVoltage); display.println("V");
  display.print("ShowingVCB_frame:"); display.println(frameCountVariable % characterBufferFrameCount);
  display_struct_bitmapIMG(display, visableCharacterBuffer[ frameCountVariable % characterBufferFrameCount ], 0, 8);
  //display.drawBitmap(0, 8, visableCharacterBuffer[ frameCountVariable % 32 ], 32, 56, WHITE);
}


void menu_SELECTION(){
  display.clearDisplay();
  int buttonDrawCount = 4;
  int buttonDrawWidth = display.width() - 2;
  int buttonDrawHeight = 16;//display.height()/4;
  int optionNameID = 0;
  unsigned int printBufferSize = 18;//33
  char printOptionsBuffer[ printBufferSize ];
  memset(printOptionsBuffer, '\0', printBufferSize);
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
    strlcpy(printOptionsBuffer, smOptionName_List[ optionNameID ], printBufferSize);
    display.print(optionNameID); display.print(":"); display.println(printOptionsBuffer);
  }
}



  

void menu_LORA(){
  //ToDo: make this its own Loop And only breakOut of it if the (currentMenuID!=2)
  display.clearDisplay();
  //https://github.com/xreef/EByte_LoRa_E220_Series_Library
  display.setCursor(0, 1 * 8);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.print("LoRa Menu"); display.println();
  //https://cplusplus.com/reference/string/string/
  static String timeLog;
  static String messageLog;
  String message;
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
      struct tm timeinfo;
      if(!getLocalTime(&timeinfo)){
        Serial.println("Failed to obtain time");
      }
      Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
      char timeInfoChar[64+1];
      strftime(timeInfoChar,64+1, "%A, %B %d %Y %H:%M:%S", &timeinfo);
      Serial.println(timeInfoChar);
      timeLog = timeInfoChar;
      messageLog = rc.data;
      // Print the data received
      Serial.println(rc.status.getResponseDescription());
      Serial.println(rc.data);
      Serial.println(rc.data[0], DEC);//IDK
      Serial.println(rc.data[1], DEC);//IDK
      Serial.println(rc.data[2], DEC);//channel
      display.println(rc.status.getResponseDescription());
      display.println(rc.data);
      #ifdef ENABLE_RSSI
      Serial.print("RSSI: "); Serial.println(rc.rssi, DEC);
      display.print("RSSI: "); display.println(rc.rssi, DEC);
      #endif
    }
    {
      // Send message
      //ResponseStatus rs = e220ttl.sendBroadcastFixedMessage(0, 0, 0x17, &messaggione, sizeof(Messaggione));
      ResponseStatus rs = e220ttl.sendBroadcastFixedMessage(0x17, "Message Received!");
      // Check If there is some problem of succesfully send
      Serial.println(rs.getResponseDescription());
    }
    {
      String input = "Oh, i bet you like kissing boys, boykisser!";
      e220ttl.sendMessage(input);
    }
    
  }
  display.println("MessageLog: ");
  display.print(timeLog);
  display.println();
  
  display.print("\"");
  display.print(messageLog);
  display.print("\"");
  display.println();
  display.setCursor(0, 5 * 8);
}

void menu_SPIFFS(){
  display.clearDisplay();
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
  printDirectory(root, 0);
  Serial.println("CheckPoint-01");
  free(savedFileNames);
}

void printDirectory(File dir, int numTabs) {
  while (true) {
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.print(entry.size(), DEC);
      Serial.print("\t --> ");
      Serial.print(numTabs);
      Serial.println();
    }
    entry.close();
  }
}


void menu_WIFI(){
  display.clearDisplay();
  //https://randomnerdtutorials.com/esp32-useful-wi-fi-functions-arduino/
  //https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/soft-access-point-class.html
  display.setCursor(0, 1 * 8);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.print("WiFi Menu"); display.println();
  if (buttonpiso1.isTapped(3) == true) {//A
    if (WiFi.getMode() == WIFI_MODE_NULL) {
      enable_STA_WiFi();
    } else {
      disable_STA_WiFi();
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
  
  const char preSetCharArray[16] = "WiFi.getMode()";
  Serial.print( preSetCharArray );Serial.print(":"); Serial.print(WiFi.getMode()); Serial.print(", ");
  switch (WiFi.getMode()){
    case (WIFI_MODE_NULL):
      Serial.print( preSetCharArray );Serial.println(" == WIFI_MODE_NULL");
      break;
    case (WIFI_MODE_STA):
      Serial.print( preSetCharArray );Serial.println(" == WIFI_MODE_STA");
      break;
    case (WIFI_MODE_AP):
      Serial.print( preSetCharArray );Serial.println(" == WIFI_MODE_AP");
      break;
    case (WIFI_MODE_APSTA):
      Serial.print( preSetCharArray );Serial.println(" == WIFI_MODE_APSTA");
      break;
    case (WIFI_MODE_MAX):
      Serial.print( preSetCharArray );Serial.println(" == WIFI_MODE_MAX");
      break;
    default:
      Serial.print( preSetCharArray );Serial.print(" == ");Serial.print( WiFi.getMode() );Serial.println();
      break;
  }
}

void menu_IR(){
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.print("IR Menu"); display.println();
  display.print("Functionality Not yet added!"); display.println();
}

void menu_IP(){
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.print("IP Menu"); display.println();
  display.print("AP IP address: "); display.print( WiFi.softAPIP() ); display.println();
}








void checkAllDebugInputs(){
  //https://arduino.stackexchange.com/questions/31256/multiple-client-server-over-wifi
  //Make Serial Debug command into Function in order for usage with both SerialBT and Serial
  serialDebugCommands(Serial);
#if (FeatureEnable_BT_Serial==true)
  serialDebugCommands(SerialBT);
#endif


  // listen for incoming clients
  if (!SerialWiFiclient.connected()) {
    SerialWiFiclient = SerialWiFiserver.available();
  }
  Serial.print("Client Connected:"); Serial.print(SerialWiFiclient.connected()); Serial.print(" Client available:"); Serial.print(SerialWiFiclient.available()); Serial.println();


  //MAX_SERIAL_WIFI_CLIENTS
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
      Serial.print("Local IP address: "); Serial.println(WiFi.localIP());
    };
    if (WiFi.getMode() != WIFI_MODE_STA) {
      Serial.print("SoftAP IP address: "); Serial.println(WiFi.softAPIP());
    };
  }
}



#define MAXDEBUG_TARGETCOMMAND_LENGTH 64
#define MAXDEBUG_COMMANDINPUT_LENGTH 256
#define MAXDEBUG_SUBTARGETCOMMAND_LENGTH 256
#define MAXDEBUG_SUBCOMMANDINPUT_LENGTH 256
class DebugCommands {
  public:
    //buffers
    char targetCommand[ MAXDEBUG_TARGETCOMMAND_LENGTH ];
    char commandInputs[ MAXDEBUG_COMMANDINPUT_LENGTH ];
    char subTargetCommand[ MAXDEBUG_SUBTARGETCOMMAND_LENGTH ];
    char subCommandInputs[ MAXDEBUG_SUBCOMMANDINPUT_LENGTH ];
};



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
    if (commandSelect(commandInputs, "getconfig")) {
      //command format: control LoRa getconfig
      strcpy(subTargetCommand, "getconfig");
      strcpy(subCommandInputs, &commandInputs[strlen(subTargetCommand)]);
      printCharArrayValue(serialport, subCommandInputs, "subCommandInputs");
      printLoRaConfig(serialport, e220ttl);
    }
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
      serialport.print("WiFi.getMode():"); serialport.print(WiFi.getMode()); serialport.print(", ");
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
      
      const char ch0 = '"';
      char *pointer0;
      char *pointer1;
      char *pointer2;
      char *pointer3;
      unsigned int pSize0;
      unsigned int pSize1;
      unsigned int pSize2;
      unsigned int pSize3;
      char STA_SSID[33];
      char STA_PASS[64];
      memset(STA_SSID, '\0', 33);
      memset(STA_PASS, '\0', 64);
      pointer0 = strchr(subCommandInputs, ch0);
      pSize0 = strlen(pointer0);
      pointer1 = strchr(&pointer0[1], ch0);
      pSize1 = strlen(pointer1);
      pointer2 = strchr(&pointer1[1], ch0);
      pSize2 = strlen(pointer2);
      pointer3 = strchr(&pointer2[1], ch0);
      pSize3 = strlen(pointer3);
      
      serialport.printf("Looling For: |%c|\n", ch0);
      serialport.printf("Pointer0: |%s| Len=%d\n", pointer0, pSize0);
      serialport.printf("Pointer1: |%s| Len=%d\n", pointer1, pSize1);
      serialport.printf("Pointer2: |%s| Len=%d\n", pointer2, pSize2);
      serialport.printf("Pointer3: |%s| Len=%d\n", pointer3, pSize3);
      
      strncpy(STA_SSID, &pointer0[1], pSize0-pSize1-1);
      strncpy(STA_PASS, &pointer2[1], pSize2-pSize3-1);
      serialport.printf("STA_SSID: |%s|\n", STA_SSID);
      serialport.printf("STA_PASS: |%s|\n", STA_PASS);
      
      serialport.println("Saving Wifi Credentials!");
      Credentials_WiFi_Struct wifiCredential;
      strcpy(wifiCredential.ssid, STA_SSID);
      strcpy(wifiCredential.pass, STA_PASS);
      readFile(serialport, SPIFFS, "/Wifi_Connections.json");
      
      //saveWiFicredentials( wifiCredential );
      //################################################################################################################################
      List_WiFi_Credentials_Struct temp_wifiCredentialList;
      //Load WiFiCredentialsList
      serialport.println(F("Loading WiFi configuration..."));
      serialport.println(wifiCredential.ssid);
      serialport.println(wifiCredential.pass);
      loadWiFiCredentialsList("/Wifi_Connections.json", temp_wifiCredentialList, SPIFFS);
      saveWiFiCredentialToWiFiCredentialsList(temp_wifiCredentialList, wifiCredential);
      //remove_WiFiCredential_From_WiFiCredentialsList(temp_wifiCredentialList, wifiCredential);
      saveWiFiCredentialsList("/Wifi_Connections.json", temp_wifiCredentialList, SPIFFS);
      //################################################################################################################################
      
      readFile(serialport, SPIFFS, "/Wifi_Connections.json");
      serialport.println("Wifi Credentials Saved!");
      ESP.restart();
    }
    if (commandSelect(commandInputs, "CredentialRemove ")) {
      //command format: control WiFi CredentialRemove "SSID" "PassWord"
      strcpy(subTargetCommand, "CredentialRemove ");
      strcpy(subCommandInputs, &commandInputs[strlen(subTargetCommand)]);
      printCharArrayValue(serialport, subCommandInputs, "subCommandInputs");
      
      const char ch0 = '"';
      char *pointer0;
      char *pointer1;
      char *pointer2;
      char *pointer3;
      unsigned int pSize0;
      unsigned int pSize1;
      unsigned int pSize2;
      unsigned int pSize3;
      char STA_SSID[33];
      char STA_PASS[64];
      memset(STA_SSID, '\0', 33);
      memset(STA_PASS, '\0', 64);
      pointer0 = strchr(subCommandInputs, ch0);
      pSize0 = strlen(pointer0);
      pointer1 = strchr(&pointer0[1], ch0);
      pSize1 = strlen(pointer1);
      pointer2 = strchr(&pointer1[1], ch0);
      pSize2 = strlen(pointer2);
      pointer3 = strchr(&pointer2[1], ch0);
      pSize3 = strlen(pointer3);
      
      serialport.printf("Looling For: |%c|\n", ch0);
      serialport.printf("Pointer0: |%s| Len=%d\n", pointer0, pSize0);
      serialport.printf("Pointer1: |%s| Len=%d\n", pointer1, pSize1);
      serialport.printf("Pointer2: |%s| Len=%d\n", pointer2, pSize2);
      serialport.printf("Pointer3: |%s| Len=%d\n", pointer3, pSize3);
      
      strncpy(STA_SSID, &pointer0[1], pSize0-pSize1-1);
      strncpy(STA_PASS, &pointer2[1], pSize2-pSize3-1);
      serialport.printf("STA_SSID: |%s|\n", STA_SSID);
      serialport.printf("STA_PASS: |%s|\n", STA_PASS);
      
      serialport.println("Saving Wifi Credentials!");
      Credentials_WiFi_Struct wifiCredential;
      strcpy(wifiCredential.ssid, STA_SSID);
      strcpy(wifiCredential.pass, STA_PASS);
      readFile(serialport, SPIFFS, "/Wifi_Connections.json");
      
      //################################################################################################################################
      List_WiFi_Credentials_Struct temp_wifiCredentialList;
      //Load WiFiCredentialsList
      serialport.println(F("Loading WiFi configuration..."));
      serialport.println(wifiCredential.ssid);
      serialport.println(wifiCredential.pass);
      loadWiFiCredentialsList("/Wifi_Connections.json", temp_wifiCredentialList, SPIFFS);
      //saveWiFiCredentialToWiFiCredentialsList(temp_wifiCredentialList, wifiCredential);
      remove_WiFiCredential_From_WiFiCredentialsList(temp_wifiCredentialList, wifiCredential);
      saveWiFiCredentialsList("/Wifi_Connections.json", temp_wifiCredentialList, SPIFFS);
      //################################################################################################################################
      
      readFile(serialport, SPIFFS, "/Wifi_Connections.json");
      serialport.println("Wifi Credentials Saved!");
      ESP.restart();
    }
    if (commandSelect(commandInputs, "disconnect")) {
      //command format: control WiFi disconnect
      strcpy(subTargetCommand, "connect ");
      strcpy(subCommandInputs, &commandInputs[strlen(subTargetCommand)]);
      printCharArrayValue(serialport, subCommandInputs, "subCommandInputs");
      WiFi.disconnect(true);  // Disconnect from the network
      serialport.println("Wifi Disconnected!");
    }
    if (commandSelect(commandInputs, "checkSavedInfo")) {
      //command format: control WiFi checkSavedInfo
      strcpy(subTargetCommand, "checkSavedInfo ");
      strcpy(subCommandInputs, &commandInputs[strlen(subTargetCommand)]);
      printCharArrayValue(serialport, subCommandInputs, "subCommandInputs");
      readFile(serialport, SPIFFS, "/Wifi_Connections.json");
      serialport.println("checkSavedInfo!");
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




void printLoRaConfig(Stream &serialport, LoRa_E220 &e220ttl)
{
  //https://mischianti.org/2022/04/19/ebyte-lora-e220-llcc68-device-for-arduino-esp32-or-esp8266-configuration-3/
  // Startup all pins and UART
  //e220ttl.begin();//LoRa Usage

  //https://mischianti.org/2022/04/19/ebyte-lora-e220-llcc68-device-for-arduino-esp32-or-esp8266-configuration-3/
  ResponseStructContainer c;
  c = e220ttl.getConfiguration();
  // It's important get configuration pointer before all other operation
  Configuration configuration = *(Configuration*) c.data;
  serialport.println(c.status.getResponseDescription());
  serialport.println(c.status.code);

  printParameters(serialport, configuration);

  ResponseStructContainer cMi;
  cMi = e220ttl.getModuleInformation();
  // It's important get information pointer before all other operation
  ModuleInformation mi = *(ModuleInformation*)cMi.data;

  serialport.println(cMi.status.getResponseDescription());
  serialport.println(cMi.status.code);

  printModuleInformation(serialport, mi);
  c.close();
}



//https://mischianti.org/2022/04/19/ebyte-lora-e220-llcc68-device-for-arduino-esp32-or-esp8266-configuration-3/

void printParameters(Stream &serialport, struct Configuration configuration) {
  serialport.println("----------------------------------------");
  
  serialport.print(F("HEAD : "));  serialport.print(configuration.COMMAND, HEX); serialport.print(" "); serialport.print(configuration.STARTING_ADDRESS, HEX); serialport.print(" "); serialport.println(configuration.LENGHT, HEX);
  serialport.println(F(" "));
  serialport.print(F("AddH : "));  serialport.println(configuration.ADDH, HEX);
  serialport.print(F("AddL : "));  serialport.println(configuration.ADDL, HEX);
  serialport.println(F(" "));
  serialport.print(F("Chan : "));  serialport.print(configuration.CHAN, DEC); serialport.print(" -> "); serialport.println(configuration.getChannelDescription());
  serialport.println(F(" "));
  serialport.print(F("SpeedParityBit     : "));  serialport.print(configuration.SPED.uartParity, BIN); serialport.print(" -> "); serialport.println(configuration.SPED.getUARTParityDescription());
  serialport.print(F("SpeedUARTDatte     : "));  serialport.print(configuration.SPED.uartBaudRate, BIN); serialport.print(" -> "); serialport.println(configuration.SPED.getUARTBaudRateDescription());
  serialport.print(F("SpeedAirDataRate   : "));  serialport.print(configuration.SPED.airDataRate, BIN); serialport.print(" -> "); serialport.println(configuration.SPED.getAirDataRateDescription());
  serialport.println(F(" "));
  serialport.print(F("OptionSubPacketSett: "));  serialport.print(configuration.OPTION.subPacketSetting, BIN); serialport.print(" -> "); serialport.println(configuration.OPTION.getSubPacketSetting());
  serialport.print(F("OptionTranPower    : "));  serialport.print(configuration.OPTION.transmissionPower, BIN); serialport.print(" -> "); serialport.println(configuration.OPTION.getTransmissionPowerDescription());
  serialport.print(F("OptionRSSIAmbientNo: "));  serialport.print(configuration.OPTION.RSSIAmbientNoise, BIN); serialport.print(" -> "); serialport.println(configuration.OPTION.getRSSIAmbientNoiseEnable());
  serialport.println(F(" "));
  serialport.print(F("TransModeWORPeriod : "));  serialport.print(configuration.TRANSMISSION_MODE.WORPeriod, BIN); serialport.print(" -> "); serialport.println(configuration.TRANSMISSION_MODE.getWORPeriodByParamsDescription());
  serialport.print(F("TransModeEnableLBT : "));  serialport.print(configuration.TRANSMISSION_MODE.enableLBT, BIN); serialport.print(" -> "); serialport.println(configuration.TRANSMISSION_MODE.getLBTEnableByteDescription());
  serialport.print(F("TransModeEnableRSSI: "));  serialport.print(configuration.TRANSMISSION_MODE.enableRSSI, BIN); serialport.print(" -> "); serialport.println(configuration.TRANSMISSION_MODE.getRSSIEnableByteDescription());
  serialport.print(F("TransModeFixedTrans: "));  serialport.print(configuration.TRANSMISSION_MODE.fixedTransmission, BIN); serialport.print(" -> "); serialport.println(configuration.TRANSMISSION_MODE.getFixedTransmissionDescription());


  serialport.println("----------------------------------------");
}
void printModuleInformation(Stream &serialport, struct ModuleInformation moduleInformation) {
  serialport.println(F("----------------------------------------"));
  serialport.print(F("HEAD: "));  serialport.print(moduleInformation.COMMAND, HEX); serialport.print(" "); serialport.print(moduleInformation.STARTING_ADDRESS, HEX); serialport.print(" "); serialport.println(moduleInformation.LENGHT, DEC);

  serialport.print(F("Model no.: "));  serialport.println(moduleInformation.model, HEX);
  serialport.print(F("Version  : "));  serialport.println(moduleInformation.version, HEX);
  serialport.print(F("Features : "));  serialport.println(moduleInformation.features, HEX);
  serialport.println(F("----------------------------------------"));

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


// Computes the heap fragmentation percentage.
//inline float getFragmentation() {
float getFragmentation() {
  //https://github.com/bblanchon/cpp4arduino/blob/master/HeapFragmentation/MemoryInfo.h
  //https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/heap_debug.html
  return 100 - heap_caps_get_largest_free_block(MALLOC_CAP_8BIT) * 100.0 / heap_caps_get_total_size(MALLOC_CAP_8BIT);
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



/*
 * //https://www.tutorialspoint.com/esp32_for_iot/esp32_for_iot_spiffs_storage.htm
 */

void listDir(Stream &serialport, fs::FS &fs, const char * dirname, uint8_t levels){
   serialport.printf("Listing directory: %s\r\n", dirname);

   File root = fs.open(dirname);
   if(!root){
      serialport.println("− failed to open directory");
      return;
   }
   if(!root.isDirectory()){
      serialport.println(" − not a directory");
      return;
   }

   File file = root.openNextFile();
   while(file){
      if(file.isDirectory()){
         serialport.print("  DIR : ");
         serialport.println(file.name());
         if(levels){
            listDir(serialport, fs, file.name(), levels -1);
         }
      } else {
         serialport.print("  FILE: ");
         serialport.print(file.name());
         serialport.print("\tSIZE: ");
         serialport.println(file.size());
      }
      file = root.openNextFile();
   }
}

void readFile(Stream &serialport, fs::FS &fs, const char * path){
   serialport.printf("Reading file: %s\r\n", path);

   File file = fs.open(path);
   if(!file || file.isDirectory()){
       serialport.println("− failed to open file for reading");
       return;
   }

   serialport.println("− read from file:");
   while(file.available()){
      serialport.write(file.read());
   }
}

void writeFile(Stream &serialport, fs::FS &fs, const char * path, const char * message){
   serialport.printf("Writing file: %s\r\n", path);

   File file = fs.open(path, FILE_WRITE);
   if(!file){
      serialport.println("− failed to open file for writing");
      return;
   }
   if(file.print(message)){
      serialport.println("− file written");
   }else {
      serialport.println("− frite failed");
   }
}

void appendFile(Stream &serialport, fs::FS &fs, const char * path, const char * message){
   serialport.printf("Appending to file: %s\r\n", path);

   File file = fs.open(path, FILE_APPEND);
   if(!file){
      serialport.println("− failed to open file for appending");
      return;
   }
   if(file.print(message)){
      serialport.println("− message appended");
   } else {
      serialport.println("− append failed");
   }
}

void renameFile(Stream &serialport, fs::FS &fs, const char * path1, const char * path2){
   serialport.printf("Renaming file %s to %s\r\n", path1, path2);
   if (fs.rename(path1, path2)) {
      serialport.println("− file renamed");
   } else {
      serialport.println("− rename failed");
   }
}

void deleteFile(Stream &serialport, fs::FS &fs, const char * path){
   serialport.printf("Deleting file: %s\r\n", path);
   if(fs.remove(path)){
      serialport.println("− file deleted");
   } else {
      serialport.println("− delete failed");
   }
}
