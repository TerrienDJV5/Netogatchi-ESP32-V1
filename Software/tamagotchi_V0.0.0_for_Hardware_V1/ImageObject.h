//By Terrien-DJV5
//ImageObject.h
//Date Created: jun 29 2023 6:48PM

#ifndef MY_IMAGEOBJECT_H
#define MY_IMAGEOBJECT_H
#include <Arduino.h>



class ImageObject {
  public:
    struct PixelStruct;
    //how/where image is stored im memory
    typedef struct PixelArrayStruct{
      uint8_t* data;//Array
      unsigned int dataByteLength;//ArrayLength
      
      uint8_t pixelBitLength;//Max is 32
      
      //LSB -> MSB
      uint8_t redBitOffset;
      uint8_t greenBitOffset;
      uint8_t blueBitOffset;
      uint8_t alphaBitOffset;
      
      uint8_t redBitLength;
      uint8_t greenBitLength;
      uint8_t blueBitLength;
      uint8_t alphaBitLength;
    } PixelArrayStruct;
    typedef struct ImageStruct{
      unsigned int width;
      unsigned int height;
      unsigned int pixelCount;
      PixelArrayStruct PixelArray;
    } ImageStruct;
  public:
    ImageObject(bool grayScaleFlag, bool alphaPixelFlag, int w, int h, uint8_t inputValue0 = -1, uint8_t inputValue1 = -1, uint8_t inputValue2 = -1, uint8_t inputValue3 = -1);
    unsigned int getWidth();
    unsigned int getHeight();
    unsigned int getPixelCount();
    PixelStruct getPixelValue(int xPos, int yPos);
    void setPixelValue(int xPos, int yPos, PixelStruct pixel);
  private:
    ImageStruct ImageOBJ;
    void createImage(int w, int h, int redBitOffset, int greenBitOffset, int blueBitOffset, int alphaBitOffset, int redBitLength, int greenBitLength, int blueBitLength, int alphaBitLength);
    uint32_t bitSelect_i32o32(uint32_t input, unsigned int offset, unsigned int length);
    uint64_t arraybitSelect_o64(uint8_t inputArray[], unsigned int bitIndex);
    uint32_t arraybitSelect_o32(uint8_t inputArray[], unsigned int bitIndex);
    PixelStruct setPixelValues(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    PixelStruct filterPixelValues(PixelStruct pixelIn, uint8_t rBitLength, uint8_t gBitLength, uint8_t bBitLength, uint8_t aBitLength);
    uint32_t ConvertPixelStruct_to_Binary(PixelStruct pixel, uint8_t rBitLength, uint8_t gBitLength, uint8_t bBitLength, uint8_t aBitLength);
  protected:
    ;
};



#endif
