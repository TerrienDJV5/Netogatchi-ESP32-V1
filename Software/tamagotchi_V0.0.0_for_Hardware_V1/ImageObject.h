//By Terrien-DJV5
//ImageObject.h
//Date Created: jun 29 2023 6:48PM

#ifndef MY_IMAGEOBJECT_H
#define MY_IMAGEOBJECT_H
#include <Arduino.h>

typedef struct {
  unsigned char white;
  unsigned char red;
  unsigned char green;
  unsigned char blue;
  unsigned char alpha;
} PixelStruct;

class ImageObject {
  public:
    //how/where image is stored im memory
    typedef struct {
      unsigned char* data;//Array
      unsigned long int dataByteLength;//ArrayLength
      
      uint8_t pixelBitLength;//Max is 32
      
      //LSB -> MSB
      uint8_t redBitOffset;
      uint8_t greenBitOffset;
      uint8_t blueBitOffset;
      uint8_t alphaBitOffset;
      
      uint8_t whiteBitLength;
      uint8_t redBitLength;
      uint8_t greenBitLength;
      uint8_t blueBitLength;
      uint8_t alphaBitLength;
    } PixelArrayStruct;
    typedef struct {
      uint8_t width;
      uint8_t height;
      PixelArrayStruct PixelArray;
    } ImageStruct;
  private:
    typedef struct {
      unsigned char whiteMask;
      unsigned char redMask;
      unsigned char greenMask;
      unsigned char blueMask;
      unsigned char alphaMask;
    } PixelMaskStruct;
    void setPixelMask(PixelMaskStruct *pixelMaskInput);
  public:
    ImageObject(bool grayscaleFlag, bool alphaChannelFlag, int w, int h, uint8_t inputValue0 = -1, uint8_t inputValue1 = -1, uint8_t inputValue2 = -1, uint8_t inputValue3 = -1);
    uint8_t getWidth();
    uint8_t getHeight();
    unsigned short int getPixelCount();
    PixelStruct setPixelStructValues(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
    PixelStruct getPixelValue(int xPos, int yPos);
    void setPixelValue(int xPos, int yPos, PixelStruct pixel);
  private:
    bool grayscaleFlag;
    bool alphaChannelFlag;
    PixelMaskStruct PixelsMask;
    ImageStruct ImageOBJ;
    void createImageFunc(int w, int h, int redBitLength, int greenBitLength, int blueBitLength, int alphaBitLength);
    uint32_t bitSelect_i32o32(uint32_t input, unsigned int offset, unsigned int length);
    uint64_t arraybitSelect_o64(uint8_t inputArray[], unsigned int bitIndex);
    uint32_t arraybitSelect_o32(uint8_t inputArray[], unsigned int bitIndex);
    void uint32_convert_to_4xuint8Array(uint8_t inputArray[], uint32_t input);
    void byteArray_bitWrite_i32(uint8_t inputArray[], uint32_t input, uint64_t bitoffset, uint8_t inputbitlength);
    PixelStruct filterPixelValues(PixelStruct pixelIn, uint8_t rBitLength, uint8_t gBitLength, uint8_t bBitLength, uint8_t aBitLength);
    PixelStruct applyPixelMask(PixelStruct pixelIn);
    uint32_t ConvertPixelStruct_to_Binary(PixelStruct pixel, uint8_t rBitLength, uint8_t gBitLength, uint8_t bBitLength, uint8_t aBitLength);
  protected:
    ;
};



#endif
