//By Terrien-DJV5
//ImageObject.h
//Date Created: jun 29 2023 6:48PM

#ifndef MY_IMAGEOBJECT_H
#define MY_IMAGEOBJECT_H
#include <Arduino.h>


class ImageObject {
  public:
    ImageObject(bool grayScaleFlag, bool alphaPixelFlag, int w, int h, uint8_t inputValue0 = -1, uint8_t inputValue1 = -1, uint8_t inputValue2 = -1, uint8_t inputValue3 = -1);
    ImageStruct getImageOBJ();
    unsigned int getWidth();
    unsigned int getHeight();
    unsigned int getPixelCount();
    PixelStruct getPixelValue(int xPos, int yPos);
    void setPixelValue(int xPos, int yPos, PixelStruct pixel);
  private:
    void createImage(int w, int h, int redBitOffset, int greenBitOffset, int blueBitOffset, int alphaBitOffset, int redBitLength, int greenBitLength, int blueBitLength, int alphaBitLength);
    uint32_t bitSelect_i32o32(uint32_t input, unsigned int offset, unsigned int length);
    uint64_t arraybitSelect_o64(uint8_t inputArray[], unsigned int bitIndex);
    uint32_t arraybitSelect_o32(uint8_t inputArray[], unsigned int bitIndex);
    PixelStruct setPixelValues(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    PixelStruct filterPixelValues(PixelStruct pixelIn, uint8_t rBitLength, uint8_t gBitLength, uint8_t bBitLength, uint8_t aBitLength);
    uint32_t ConvertPixelStruct_to_Binary(PixelStruct pixel, uint8_t rBitLength, uint8_t gBitLength, uint8_t bBitLength, uint8_t aBitLength);
  protected:
    ImageStruct ImageOBJ;
};



#endif
