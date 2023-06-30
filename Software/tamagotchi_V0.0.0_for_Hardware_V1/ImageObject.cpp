//By Terrien-DJV5
//ImageObject.cpp
//Date Created: jun 29 2023 6:48PM



#include <iostream>
using namespace std;


//how/where image is stored im memory
typedef struct {
  uint8_t* data;//Array
  unsigned int dataByteLength;//ArrayLength
  
  unsigned int pixelBitLength;//Max is 32

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


typedef struct {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  uint8_t alpha;
} PixelStruct;



typedef struct {
  unsigned int width;
  unsigned int height;
  unsigned int pixelCount;
  PixelArrayStruct PixelArray;
} ImageStruct;




#include "ImageObject.h"





ImageObject::ImageObject(bool grayScaleFlag, bool alphaPixelFlag, int w, int h, uint8_t inputValue0, uint8_t inputValue1, uint8_t inputValue2, uint8_t inputValue3)
{
  uint8_t redBitOffset = 0;
  uint8_t greenBitOffset = 0;
  uint8_t blueBitOffset = 0;
  uint8_t alphaBitOffset = 0;
  uint8_t redBitLength = 0;
  uint8_t greenBitLength = 0;
  uint8_t blueBitLength = 0;
  uint8_t alphaBitLength = 0;
  
  if (grayScaleFlag){
    if (alphaPixelFlag){
      //Use inputValue0, inputValue1
      redBitLength = inputValue0;
      greenBitLength = inputValue0;
      blueBitLength = inputValue0;
      
      alphaBitOffset = inputValue0;
      alphaBitLength = inputValue1;
    }else{
      //Use inputValue0
      redBitLength = inputValue0;
      greenBitLength = inputValue0;
      blueBitLength = inputValue0;
    }
  }else{
    if (alphaPixelFlag){
      //Use inputValue0, inputValue1, inputValue2, inputValue3
      redBitLength = inputValue0;
      greenBitLength = inputValue1;
      blueBitLength = inputValue2;
      alphaBitLength = inputValue3;
      redBitOffset = 0;
      greenBitOffset = redBitLength;
      blueBitOffset = redBitLength+greenBitLength;
      alphaBitOffset = redBitLength+greenBitLength+blueBitLength;
    }else{
      //Use inputValue0, inputValue1, inputValue2
      redBitLength = inputValue0;
      greenBitLength = inputValue1;
      blueBitLength = inputValue2;
      redBitOffset = 0;
      greenBitOffset = redBitLength;
      blueBitOffset = redBitLength+greenBitLength;
    }
  }
  createImage(w, h, redBitOffset, greenBitOffset, blueBitOffset, alphaBitOffset, redBitLength, greenBitLength, blueBitLength, alphaBitLength);
}


void ImageObject::createImage(int w, int h, int redBitOffset, int greenBitOffset, int blueBitOffset, int alphaBitOffset, int redBitLength, int greenBitLength, int blueBitLength, int alphaBitLength)
{
  ImageOBJ.width = w;
  ImageOBJ.height = h;
  ImageOBJ.pixelCount = ImageOBJ.width * ImageOBJ.height;
  
  ImageOBJ.PixelArray.pixelBitLength = (redBitLength+redBitOffset);
  ImageOBJ.PixelArray.pixelBitLength = (greenBitLength+greenBitOffset);
  ImageOBJ.PixelArray.pixelBitLength = (blueBitLength+blueBitOffset);
  ImageOBJ.PixelArray.pixelBitLength = (alphaBitLength+alphaBitOffset);
  
  ImageOBJ.PixelArray.redBitOffset = redBitOffset;
  ImageOBJ.PixelArray.redBitLength = redBitLength;
  ImageOBJ.PixelArray.greenBitOffset = greenBitOffset;
  ImageOBJ.PixelArray.greenBitLength = greenBitLength;
  ImageOBJ.PixelArray.blueBitOffset = blueBitOffset;
  ImageOBJ.PixelArray.blueBitLength = blueBitLength;
  ImageOBJ.PixelArray.alphaBitOffset = alphaBitOffset;
  ImageOBJ.PixelArray.alphaBitLength = alphaBitLength;
  
}



uint32_t ImageObject::bitSelect_i32o32(uint32_t input, unsigned int offset, unsigned int length)
{
  //offset = 4
  //length = 12
  const byte inputLength = 32;
  byte shiftLeftCount = inputLength-(offset+length);
  uint32_t bitSelectMask = ~(0B11111111111111111111111111111111<<length);
  
  //"|" Selected bits
  //  ----||||||||||||----------------
  //0B11110000111100001111000011110000
  //x = unused Bits
  //output
  //  --------------------||||||||||||
  //0Bxxxxxxxxxxxxxxxxxxxx000011110000
  
  return (input<<shiftLeftCount) & bitSelectMask;
}

uint64_t ImageObject::arraybitSelect_o64(uint8_t inputArray[], unsigned int bitIndex)
{
  //bitIndex = 10
  //bitGrabCount = 64
  //byteIndex = 1
  //startMask = 0B00111111;
  //endMask = 0B11000000;
  //xxxxxxxx xx|||||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| ||xxxxxx
  //11000000 00111111 11000000 00111111 11000000 00111111 11000000 00111111 11000000 00111111
  byte endMask = (0B11111111)<<(8-bitIndex%8);
  byte startMask = ~(endMask);
  unsigned int byteIndex = ((bitIndex-(bitIndex%8))>>3);
  uint64_t output;
  uint8_t outputList[8];
  outputList[0] = (inputArray[ (byteIndex+0) ]&startMask) | (inputArray[ (byteIndex+1) ]&endMask);
  outputList[1] = (inputArray[ (byteIndex+1) ]&startMask) | (inputArray[ (byteIndex+2) ]&endMask);
  outputList[2] = (inputArray[ (byteIndex+2) ]&startMask) | (inputArray[ (byteIndex+3) ]&endMask);
  outputList[3] = (inputArray[ (byteIndex+3) ]&startMask) | (inputArray[ (byteIndex+4) ]&endMask);
  outputList[4] = (inputArray[ (byteIndex+4) ]&startMask) | (inputArray[ (byteIndex+5) ]&endMask);
  outputList[5] = (inputArray[ (byteIndex+5) ]&startMask) | (inputArray[ (byteIndex+6) ]&endMask);
  outputList[6] = (inputArray[ (byteIndex+6) ]&startMask) | (inputArray[ (byteIndex+7) ]&endMask);
  outputList[7] = (inputArray[ (byteIndex+7) ]&startMask) | (inputArray[ (byteIndex+8) ]&endMask);
  output = (outputList[0]<<(64-8))|(outputList[1]<<(64-8-8))|(outputList[2]<<(64-8-8-8))|(outputList[3]<<(64-8-8-16))|(outputList[4]<<(64-8-16-16))|(outputList[5]<<(64-8-8-32))|(outputList[6]<<(64-8-16-32))|(outputList[7]<<(64-8-8-16-32));
  return output;
}

uint32_t ImageObject::arraybitSelect_o32(uint8_t inputArray[], unsigned int bitIndex)
{
  //bitIndex = 10
  //bitGrabCount = 632
  //byteIndex = 1
  //startMask = 0B00111111;
  //endMask = 0B11000000;
  //xxxxxxxx xx|||||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| ||xxxxxx
  //11000000 00111111 11000000 00111111 11000000 00111111 11000000 00111111 11000000 00111111
  byte endMask = (0B11111111)<<(8-bitIndex%8);
  byte startMask = ~(endMask);
  unsigned int byteIndex = ((bitIndex-(bitIndex%8))>>3);
  uint32_t output;
  uint8_t outputList[8];
  outputList[0] = (inputArray[ (byteIndex+0) ]&startMask) | (inputArray[ (byteIndex+1) ]&endMask);
  outputList[1] = (inputArray[ (byteIndex+1) ]&startMask) | (inputArray[ (byteIndex+2) ]&endMask);
  outputList[2] = (inputArray[ (byteIndex+2) ]&startMask) | (inputArray[ (byteIndex+3) ]&endMask);
  outputList[3] = (inputArray[ (byteIndex+3) ]&startMask) | (inputArray[ (byteIndex+4) ]&endMask);
  output = (outputList[0]<<(32-8))|(outputList[1]<<(32-16))|(outputList[2]<<(32-16-8))|(outputList[3]<<(32-32));
  return output;
}

ImageStruct ImageObject::getImageOBJ()
{
  return ImageOBJ;
}

unsigned int ImageObject::getWidth()
{
  return ImageOBJ.width;
}
unsigned int ImageObject::getHeight()
{
  return ImageOBJ.height;
}

unsigned int ImageObject::getPixelCount()
{
  return ImageOBJ.pixelCount;
}

PixelStruct ImageObject::getPixelValue(int xPos, int yPos)
{
  PixelStruct pixel;
  unsigned int pixelBitindex = (xPos + yPos*ImageOBJ.width)*(ImageOBJ.PixelArray.pixelBitLength);
  unsigned int pixelindex = pixelBitindex/8;
  uint32_t pixelData = arraybitSelect_o32(ImageOBJ.PixelArray.data, pixelBitindex);
  pixelData = pixelData & (~(0B11111111111111111111111111111111 << ImageOBJ.PixelArray.pixelBitLength));
  pixel.red = bitSelect_i32o32(pixelData, ImageOBJ.PixelArray.redBitOffset, ImageOBJ.PixelArray.redBitLength);
  pixel.green = bitSelect_i32o32(pixelData, ImageOBJ.PixelArray.greenBitOffset, ImageOBJ.PixelArray.greenBitLength);
  pixel.blue = bitSelect_i32o32(pixelData, ImageOBJ.PixelArray.blueBitOffset, ImageOBJ.PixelArray.blueBitLength);
  pixel.alpha = bitSelect_i32o32(pixelData, ImageOBJ.PixelArray.alphaBitOffset, ImageOBJ.PixelArray.alphaBitLength);
  return pixel;
}

void ImageObject::setPixelValue(int xPos, int yPos, PixelStruct pixel)
{
  unsigned int pixelBitindex = (xPos + yPos*ImageOBJ.width)*(ImageOBJ.PixelArray.pixelBitLength);
  //Work on Me!
  ;
}

PixelStruct ImageObject::setPixelValues(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
  PixelStruct pixel;
  pixel.red = r;
  pixel.green = g;
  pixel.blue = b;
  pixel.alpha = a;
  return pixel;
}

PixelStruct ImageObject::filterPixelValues(PixelStruct pixelIn, uint8_t rBitLength, uint8_t gBitLength, uint8_t bBitLength, uint8_t aBitLength)
{
  PixelStruct pixelOut;
  byte rMask = (0B11111111<<rBitLength);
  byte gMask = (0B11111111<<gBitLength);
  byte bMask = (0B11111111<<bBitLength);
  byte aMask = (0B11111111<<aBitLength);
  pixel.red &= rMask;
  pixel.green &= gMask;
  pixel.blue &= bMask;
  pixel.alpha &= aMask;
  return pixelOut;
}
uint32_t ImageObject::ConvertPixelStruct_to_Binary(PixelStruct pixel, uint8_t rBitLength, uint8_t gBitLength, uint8_t bBitLength, uint8_t aBitLength)
{
  uint32_t pixelBinaryOut;
  PixelStruct pixelTemp = filterPixelValues(PixelStruct pixelIn, rBitLength, gBitLength, bBitLength, aBitLength);
  //work on me!
  return pixelBinaryOut;
}
