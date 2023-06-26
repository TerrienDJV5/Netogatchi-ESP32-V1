//By Terrien-DJV5
//BMIMGmanipulate.cpp
//Date Created: may 30 2023 11:23AM


#include "BMIMGmanipulate.h"
BMIMGmanipulate::BMIMGmanipulate(bool useStaticBuffer) {
  this->staticBufferFlag = useStaticBuffer;
  init();
}

void BMIMGmanipulate::init() {
  //
}

unsigned int BMIMGmanipulate::calculateMinimunByteForBitWidth( unsigned int bitWidth ){
  return (bitWidth%8 + bitWidth)>>3; //(n>>3) === abs(n/8)
}


void BMIMGmanipulate::rotate8x8ImageClockwise(unsigned char imageIN8x8[]){
  unsigned char imageOutput[8];
  memset(imageOutput,0,8);
  //
  imageOutput[ 0 ] = ((imageIN8x8[0]>>7) & B00000001) | ((imageIN8x8[1]>>6) & B00000010);
  imageOutput[ 1 ] = ((imageIN8x8[0]>>6) & B00000001) | ((imageIN8x8[1]>>5) & B00000010);
  imageOutput[ 2 ] = ((imageIN8x8[0]>>5) & B00000001) | ((imageIN8x8[1]>>4) & B00000010);
  imageOutput[ 3 ] = ((imageIN8x8[0]>>4) & B00000001) | ((imageIN8x8[1]>>3) & B00000010);
  imageOutput[ 4 ] = ((imageIN8x8[0]>>3) & B00000001) | ((imageIN8x8[1]>>2) & B00000010);
  imageOutput[ 5 ] = ((imageIN8x8[0]>>2) & B00000001) | ((imageIN8x8[1]>>1) & B00000010);
  imageOutput[ 6 ] = ((imageIN8x8[0]>>1) & B00000001) | ((imageIN8x8[1]   ) & B00000010);
  imageOutput[ 7 ] = ((imageIN8x8[0]   ) & B00000001) | ((imageIN8x8[1]<<1) & B00000010);
  //
  imageOutput[ 0 ] += ((imageIN8x8[2]>>5) & B00000100) | ((imageIN8x8[3]>>4) & B00001000);
  imageOutput[ 1 ] += ((imageIN8x8[2]>>4) & B00000100) | ((imageIN8x8[3]>>3) & B00001000);
  imageOutput[ 2 ] += ((imageIN8x8[2]>>3) & B00000100) | ((imageIN8x8[3]>>2) & B00001000);
  imageOutput[ 3 ] += ((imageIN8x8[2]>>2) & B00000100) | ((imageIN8x8[3]>>1) & B00001000);
  imageOutput[ 4 ] += ((imageIN8x8[2]>>1) & B00000100) | ((imageIN8x8[3]   ) & B00001000);
  imageOutput[ 5 ] += ((imageIN8x8[2]   ) & B00000100) | ((imageIN8x8[3]<<1) & B00001000);
  imageOutput[ 6 ] += ((imageIN8x8[2]<<1) & B00000100) | ((imageIN8x8[3]<<2) & B00001000);
  imageOutput[ 7 ] += ((imageIN8x8[2]<<2) & B00000100) | ((imageIN8x8[3]<<3) & B00001000);
  //
  imageOutput[ 0 ] += ((imageIN8x8[4]>>3) & B00010000) | ((imageIN8x8[5]>>2) & B00100000);
  imageOutput[ 1 ] += ((imageIN8x8[4]>>2) & B00010000) | ((imageIN8x8[5]>>1) & B00100000);
  imageOutput[ 2 ] += ((imageIN8x8[4]>>1) & B00010000) | ((imageIN8x8[5]   ) & B00100000);
  imageOutput[ 3 ] += ((imageIN8x8[4]   ) & B00010000) | ((imageIN8x8[5]<<1) & B00100000);
  imageOutput[ 4 ] += ((imageIN8x8[4]<<1) & B00010000) | ((imageIN8x8[5]<<2) & B00100000);
  imageOutput[ 5 ] += ((imageIN8x8[4]<<2) & B00010000) | ((imageIN8x8[5]<<3) & B00100000);
  imageOutput[ 6 ] += ((imageIN8x8[4]<<3) & B00010000) | ((imageIN8x8[5]<<4) & B00100000);
  imageOutput[ 7 ] += ((imageIN8x8[4]<<4) & B00010000) | ((imageIN8x8[5]<<5) & B00100000);
  //
  imageOutput[ 0 ] += ((imageIN8x8[6]>>1) & B01000000) | ((imageIN8x8[7]   ) & B10000000);
  imageOutput[ 1 ] += ((imageIN8x8[6]   ) & B01000000) | ((imageIN8x8[7]<<1) & B10000000);
  imageOutput[ 2 ] += ((imageIN8x8[6]<<1) & B01000000) | ((imageIN8x8[7]<<2) & B10000000);
  imageOutput[ 3 ] += ((imageIN8x8[6]<<2) & B01000000) | ((imageIN8x8[7]<<3) & B10000000);
  imageOutput[ 4 ] += ((imageIN8x8[6]<<3) & B01000000) | ((imageIN8x8[7]<<4) & B10000000);
  imageOutput[ 5 ] += ((imageIN8x8[6]<<4) & B01000000) | ((imageIN8x8[7]<<5) & B10000000);
  imageOutput[ 6 ] += ((imageIN8x8[6]<<5) & B01000000) | ((imageIN8x8[7]<<6) & B10000000);
  imageOutput[ 7 ] += ((imageIN8x8[6]<<6) & B01000000) | ((imageIN8x8[7]<<7) & B10000000);
  memmove(imageIN8x8, imageOutput, 8);
}




unsigned char* BMIMGmanipulate::rotate16x16ImageClockwise(unsigned char imageIN16x16[2*16]){
  static unsigned char imageOutput[2*16];
  memset(imageOutput,0,2*16);
  unsigned char sectorX0Y0[8];
  unsigned char sectorX1Y0[8];
  unsigned char sectorX0Y1[8];
  unsigned char sectorX1Y1[8];
  
  for (uint16_t index = 0; index < 8; index++) {
    memcpy(&sectorX0Y0[ index*1 ], &imageIN16x16[ index*2+0 ], 1);
    memcpy(&sectorX1Y0[ index*1 ], &imageIN16x16[ index*2+1 ], 1);
    memcpy(&sectorX0Y1[ index*1 ], &imageIN16x16[ index*2+1*16 ], 1);
    memcpy(&sectorX1Y1[ index*1 ], &imageIN16x16[ index*2+1*16+1 ], 1);
  }
  
  rotate8x8ImageClockwise( sectorX0Y0 );
  rotate8x8ImageClockwise( sectorX1Y0 );
  rotate8x8ImageClockwise( sectorX0Y1 );
  rotate8x8ImageClockwise( sectorX1Y1 );
  
  //Rotate Sectors
  unsigned char sectorStorage[8];
  memmove(sectorStorage, sectorX1Y0, 8);
  memmove(sectorX1Y0, sectorX0Y0, 8);
  memmove(sectorX0Y0, sectorX0Y1, 8);
  memmove(sectorX0Y1, sectorX1Y1, 8);
  memmove(sectorX1Y1, sectorStorage, 8);
  
  for (uint16_t index = 0; index < 8; index++) {
    memcpy(&imageOutput[ index*2+0 ], &sectorX0Y0[ index*1 ], 1);
    memcpy(&imageOutput[ index*2+1 ], &sectorX1Y0[ index*1 ], 1);
    memcpy(&imageOutput[ index*2+1*16 ], &sectorX0Y1[ index*1 ], 1);
    memcpy(&imageOutput[ index*2+1*16+1 ], &sectorX1Y1[ index*1 ], 1);
  }
  return imageOutput;
}


unsigned char* BMIMGmanipulate::rotate32x32ImageClockwise(unsigned char imageIN32x32[4*32]){
  static unsigned char imageOutput[4*32];
  memset(imageOutput,0,4*32);
  unsigned char sectorX0Y0[32];
  unsigned char sectorX1Y0[32];
  unsigned char sectorX0Y1[32];
  unsigned char sectorX1Y1[32];
  
  for (uint16_t index = 0; index < 16; index++) {
    memcpy(&sectorX0Y0[ index*2 ], &imageIN32x32[ index*4+0 ], 2);
    memcpy(&sectorX1Y0[ index*2 ], &imageIN32x32[ index*4+2 ], 2);
    memcpy(&sectorX0Y1[ index*2 ], &imageIN32x32[ index*4+2*32 ], 2);
    memcpy(&sectorX1Y1[ index*2 ], &imageIN32x32[ index*4+2*32+2 ], 2);
  }
  
  memmove(sectorX0Y0, rotate16x16ImageClockwise( sectorX0Y0 ), 32);
  memmove(sectorX1Y0, rotate16x16ImageClockwise( sectorX1Y0 ), 32);
  memmove(sectorX0Y1, rotate16x16ImageClockwise( sectorX0Y1 ), 32);
  memmove(sectorX1Y1, rotate16x16ImageClockwise( sectorX1Y1 ), 32);
  
  //Rotate Sectors
  //unsigned char sectorStorage[32];
  unsigned char *sectorStorage;
  sectorStorage = (unsigned char *)malloc(32);
  memmove(sectorStorage, sectorX1Y0, 32);
  memmove(sectorX1Y0, sectorX0Y0, 32);
  memmove(sectorX0Y0, sectorX0Y1, 32);
  memmove(sectorX0Y1, sectorX1Y1, 32);
  memmove(sectorX1Y1, sectorStorage, 32);
  free(sectorStorage);
  
  for (uint16_t index = 0; index < 16; index++) {
    memcpy(&imageOutput[ index*4+0 ], &sectorX0Y0[ index*2 ], 2);
    memcpy(&imageOutput[ index*4+2 ], &sectorX1Y0[ index*2 ], 2);
    memcpy(&imageOutput[ index*4+2*32 ], &sectorX0Y1[ index*2 ], 2);
    memcpy(&imageOutput[ index*4+2*32+2 ], &sectorX1Y1[ index*2 ], 2);
  }
  
  return imageOutput;
}


unsigned char* BMIMGmanipulate::rotate64x64ImageClockwise(unsigned char imageIN64x64[8*64]){
  const unsigned int imageByteWidth = 8;
  const unsigned int imageByteHeight = 64;
  const unsigned int fullImageByteSize = imageByteWidth*imageByteHeight;
  
  const unsigned int sectorByteWidth = imageByteWidth>>1;
  const unsigned int sectorByteHeight = imageByteHeight>>1;
  const unsigned int sectorByteSize = sectorByteWidth*sectorByteHeight;
  Serial.println(F("Track64-001"));
  static unsigned char imageOutput[fullImageByteSize];
  memset(imageOutput,0,fullImageByteSize);
  unsigned char sectorX0Y0[ sectorByteSize ];
  unsigned char sectorX1Y0[ sectorByteSize ];
  unsigned char sectorX0Y1[ sectorByteSize ];
  unsigned char sectorX1Y1[ sectorByteSize ];
  Serial.println(F("Track64-002"));
  for (uint16_t index = 0; index < sectorByteHeight; index++) {
    memcpy(&sectorX0Y0[ index*sectorByteWidth ], &imageIN64x64[ index*imageByteWidth ], sectorByteWidth);
    memcpy(&sectorX1Y0[ index*sectorByteWidth ], &imageIN64x64[ index*imageByteWidth+sectorByteWidth ], sectorByteWidth);
    memcpy(&sectorX0Y1[ index*sectorByteWidth ], &imageIN64x64[ index*imageByteWidth+sectorByteSize*2 ], sectorByteWidth);
    memcpy(&sectorX1Y1[ index*sectorByteWidth ], &imageIN64x64[ index*imageByteWidth+sectorByteSize*2+sectorByteWidth ], sectorByteWidth);
  }
  Serial.println(F("Track64-003"));
  memmove(sectorX0Y0, rotate32x32ImageClockwise( sectorX0Y0 ), sectorByteSize );
  memmove(sectorX1Y0, rotate32x32ImageClockwise( sectorX1Y0 ), sectorByteSize );
  memmove(sectorX0Y1, rotate32x32ImageClockwise( sectorX0Y1 ), sectorByteSize );
  memmove(sectorX1Y1, rotate32x32ImageClockwise( sectorX1Y1 ), sectorByteSize );
  
  //Rotate Sectors
  Serial.println(F("Track64-004"));
  unsigned char *sectorStorage;
  sectorStorage = (unsigned char *)malloc(sectorByteSize);
  memmove(sectorStorage, sectorX1Y0, sectorByteSize );
  memmove(sectorX1Y0, sectorX0Y0, sectorByteSize );
  memmove(sectorX0Y0, sectorX0Y1, sectorByteSize );
  memmove(sectorX0Y1, sectorX1Y1, sectorByteSize );
  memmove(sectorX1Y1, sectorStorage, sectorByteSize );
  free(sectorStorage);
  
  Serial.println(F("Track64-005"));
  for (uint16_t index = 0; index < sectorByteHeight; index++) {
    memcpy(&imageOutput[ index*imageByteWidth ], &sectorX0Y0[ index*sectorByteWidth ], sectorByteWidth);
    memcpy(&imageOutput[ index*imageByteWidth+sectorByteWidth ], &sectorX1Y0[ index*sectorByteWidth ], sectorByteWidth);
    memcpy(&imageOutput[ index*imageByteWidth+sectorByteSize*2 ], &sectorX0Y1[ index*sectorByteWidth ], sectorByteWidth);
    memcpy(&imageOutput[ index*imageByteWidth+sectorByteSize*2+sectorByteWidth ], &sectorX1Y1[ index*sectorByteWidth ], sectorByteWidth);
  }
  Serial.println("Track64-Return64x64");
  return imageOutput;
}



///*
void BMIMGmanipulate::rotate128x128ImageClockwise(unsigned char imageInput[]){
  unsigned int imageByteWidth = 16;
  unsigned int imageByteHeight = 128;
  unsigned int fullImageByteSize = imageByteWidth*imageByteHeight;
  
  unsigned int sectorByteWidth = imageByteWidth>>1;
  unsigned int sectorByteHeight = imageByteHeight>>1;
  unsigned int sectorByteSize = sectorByteWidth*sectorByteHeight;
  
  //unsigned char sectorX0Y0[ sectorByteSize ];
  //unsigned char sectorX1Y0[ sectorByteSize ];
  //unsigned char sectorX0Y1[ sectorByteSize ];
  //unsigned char sectorX1Y1[ sectorByteSize ];
  
  unsigned char* sectorX0Y0;
  unsigned char* sectorX1Y0;
  unsigned char* sectorX0Y1;
  unsigned char* sectorX1Y1;
  
  sectorX0Y0 = (unsigned char*)malloc( sectorByteSize );
  sectorX1Y0 = (unsigned char*)malloc( sectorByteSize );
  sectorX0Y1 = (unsigned char*)malloc( sectorByteSize );
  sectorX1Y1 = (unsigned char*)malloc( sectorByteSize );
  
  for (uint16_t index = 0; index < sectorByteHeight; index++) {
    memcpy(&sectorX0Y0[ index*sectorByteWidth ], &imageInput[ index*imageByteWidth ], sectorByteWidth);
    memcpy(&sectorX1Y0[ index*sectorByteWidth ], &imageInput[ index*imageByteWidth+sectorByteWidth ], sectorByteWidth);
    memcpy(&sectorX0Y1[ index*sectorByteWidth ], &imageInput[ index*imageByteWidth+sectorByteSize*2 ], sectorByteWidth);
    memcpy(&sectorX1Y1[ index*sectorByteWidth ], &imageInput[ index*imageByteWidth+sectorByteSize*2+sectorByteWidth ], sectorByteWidth);
  }
  //Serial.println("Track128-001");
  //Serial.println(ESP.getFreeHeap());
  memmove(sectorX0Y0, rotate64x64ImageClockwise( sectorX0Y0 ), sectorByteSize );
  memmove(sectorX1Y0, rotate64x64ImageClockwise( sectorX1Y0 ), sectorByteSize );
  memmove(sectorX0Y1, rotate64x64ImageClockwise( sectorX0Y1 ), sectorByteSize );
  memmove(sectorX1Y1, rotate64x64ImageClockwise( sectorX1Y1 ), sectorByteSize );
  
  //Rotate Sectors
  //Serial.println("Track128-002");
  //Serial.println(ESP.getFreeHeap());
  unsigned char *sectorStorage;
  sectorStorage = (unsigned char *)malloc(sectorByteSize);
  memmove(sectorStorage, sectorX1Y0, sectorByteSize );
  memmove(sectorX1Y0, sectorX0Y0, sectorByteSize );
  memmove(sectorX0Y0, sectorX0Y1, sectorByteSize );
  memmove(sectorX0Y1, sectorX1Y1, sectorByteSize );
  memmove(sectorX1Y1, sectorStorage, sectorByteSize );
  free(sectorStorage);
  
  //Serial.println("Track128-003");
  //Serial.println(ESP.getFreeHeap());
  for (uint16_t index = 0; index < sectorByteHeight; index++) {
    memcpy(&imageInput[ index*imageByteWidth ], &sectorX0Y0[ index*sectorByteWidth ], sectorByteWidth);
    memcpy(&imageInput[ index*imageByteWidth+sectorByteWidth ], &sectorX1Y0[ index*sectorByteWidth ], sectorByteWidth);
    memcpy(&imageInput[ index*imageByteWidth+sectorByteSize*2 ], &sectorX0Y1[ index*sectorByteWidth ], sectorByteWidth);
    memcpy(&imageInput[ index*imageByteWidth+sectorByteSize*2+sectorByteWidth ], &sectorX1Y1[ index*sectorByteWidth ], sectorByteWidth);
  }
  free(sectorX0Y0);
  free(sectorX1Y0);
  free(sectorX0Y1);
  free(sectorX1Y1);
  
}
//*/






//https://www.tutorialspoint.com/arduino/arduino_passing_arrays_to_functions.htm
void BMIMGmanipulate::resizeBitImage(unsigned char imgOut[], unsigned char imgIn[], uint16_t imgOutPxWidth, uint16_t imgOutPxHeight, uint16_t imgInPxWidth, uint16_t imgInPxHeight){
  uint16_t inputImgbyteWidth = calculateMinimunByteForBitWidth( imgInPxWidth );//((imgInPxWidth+imgInPxWidth%8)>>3);
  uint16_t outputImgbyteWidth = calculateMinimunByteForBitWidth( imgOutPxWidth );//((imgOutPxWidth+imgOutPxWidth%8)>>3);
  imgOut = (unsigned char *) realloc(imgOut, outputImgbyteWidth*imgOutPxHeight);
  memset(imgOut,0,outputImgbyteWidth*imgOutPxHeight);
  for (uint16_t pyBitIndex = 0; pyBitIndex < imgInPxHeight; pyBitIndex++) {
    for (uint16_t pxByteIndex = 0; pxByteIndex < inputImgbyteWidth; pxByteIndex++) {
      imgOut[pyBitIndex*outputImgbyteWidth + pxByteIndex] = imgIn[(pyBitIndex)*inputImgbyteWidth + (pxByteIndex)];
    }
  }
}


void BMIMGmanipulate::cropBitImagefromBitImage(unsigned char imageOutput[], unsigned char imageInput[], uint16_t imgOutputPxWidth, uint16_t imgOutputPxHeight, uint16_t imgInputCutXpos, uint16_t imgInputCutYpos, uint16_t imgInputPxWidth, uint16_t imgInputPxHeight){
  uint16_t outputbyteWidth = calculateMinimunByteForBitWidth( imgOutputPxWidth );//((imgOutputPxWidth+imgOutputPxWidth%8)>>3);
  uint16_t inputbyteWidth = calculateMinimunByteForBitWidth( imgInputPxWidth );//((imgInputPxWidth+imgInputPxWidth%8)>>3);
  
  imageOutput = (unsigned char *) realloc(imageOutput, outputbyteWidth*imgOutputPxHeight);
  for (uint16_t pyBitIndex = 0; pyBitIndex < imgOutputPxHeight; pyBitIndex++) {
    for (uint16_t pxByteIndex = 0; pxByteIndex < outputbyteWidth; pxByteIndex++) {
      imageOutput[pyBitIndex*outputbyteWidth + pxByteIndex] = imageInput[(imgInputCutYpos + pyBitIndex)*inputbyteWidth + (imgInputCutXpos + pxByteIndex)];
    }
  }
}


void BMIMGmanipulate::rotateBitImageClockwise(unsigned char imageOUT[], unsigned char imageIN[], uint16_t imgInPxWidth, uint16_t imgInPxHeight){//rotates 90 deg clockwise //new Px Width and Height will be flipped
  //uint16_t inputbyteWidth = ((imgInPxWidth+imgInPxWidth%8)>>3);
  //uint16_t inputbyteHeight = ((imgInPxHeight+imgInPxHeight%8)>>3);
  bool inputisSquare;
  uint16_t imgLargestValue;
  if (imgInPxWidth==imgInPxHeight){
    inputisSquare = true;
    imgLargestValue = imgInPxWidth;
  }else{
    inputisSquare = false;
    imgLargestValue = imgInPxWidth;
    if (imgInPxWidth < imgInPxHeight){
      imgLargestValue = imgInPxHeight;
    }
  }
  bool justneedtoRotate = false;
  if (inputisSquare == true){
    if ( (imgLargestValue==8)|(imgLargestValue==16)|(imgLargestValue==32)|(imgLargestValue==64)|(imgLargestValue==128) ){
      justneedtoRotate = true;
    }
  }
  
  
  //calculate byte widths and heights
  uint16_t inputbyteWidth = calculateMinimunByteForBitWidth( imgInPxWidth );
  uint16_t inputbyteHeight = calculateMinimunByteForBitWidth( imgInPxHeight );
  Serial.println("rotateClockwise");
  //expand/resize inputimage to either 8x8, 16x16, 32x32, 64x64 or 128x128
  unsigned char* imageTMP1;
  unsigned char* imageTMP2;
  Serial.println(imgLargestValue);
  //imageTMP1 = (unsigned char *) malloc(calculateMinimunByteForBitWidth( imgLargestValue )*imgLargestValue);
  
  if ( (imgLargestValue <= 8) and (imgLargestValue > 0) ){
    Serial.println("rotate8x8");
    imageTMP1 = (unsigned char *) malloc(calculateMinimunByteForBitWidth( 8 )*8);
    //imageTMP2 = (unsigned char *) malloc(calculateMinimunByteForBitWidth( 8 )*8);
    resizeBitImage(imageTMP1, imageIN, 8, 8, imgInPxWidth, imgInPxHeight);
  }
  if ( (imgLargestValue <= 16) and (imgLargestValue > 8) ){
    Serial.println("rotate16x16");
    imageTMP1 = (unsigned char *) malloc(calculateMinimunByteForBitWidth( 16 )*16);
    //imageTMP2 = (unsigned char *) malloc(calculateMinimunByteForBitWidth( 16 )*16);
    resizeBitImage(imageTMP1, imageIN, 16, 16, imgInPxWidth, imgInPxHeight);
  }
  if ( (imgLargestValue <= 32) and (imgLargestValue > 16) ){
    Serial.println("rotate32x32");
    imageTMP1 = (unsigned char *) malloc(calculateMinimunByteForBitWidth( 32 )*32);
    //imageTMP2 = (unsigned char *) malloc(calculateMinimunByteForBitWidth( 32 )*32);
    resizeBitImage(imageTMP1, imageIN, 32, 32, imgInPxWidth, imgInPxHeight);
  }
  if ( (imgLargestValue <= 64) and (imgLargestValue > 32) ){
    Serial.println("rotate64x64");
    imageTMP1 = (unsigned char *) malloc(calculateMinimunByteForBitWidth( 64 )*64);
    //imageTMP2 = (unsigned char *) malloc(calculateMinimunByteForBitWidth( 64 )*64);
    resizeBitImage(imageTMP1, imageIN, 64, 64, imgInPxWidth, imgInPxHeight);
  }
  if ( (imgLargestValue <= 128) and (imgLargestValue > 64) ){
    Serial.println("rotate128x128");
    imageTMP1 = (unsigned char *) malloc(calculateMinimunByteForBitWidth( 128 )*128);
    resizeBitImage(imageTMP1, imageIN, 128, 128, imgInPxWidth, imgInPxHeight);
  }
  
  Serial.println("Resized");
  //rotate expanded image
  
  if ( (imgLargestValue <= 8) and (imgLargestValue > 0) ){
    Serial.println("resized8x8");
    rotate8x8ImageClockwise( imageTMP1 );
    //imageTMP2 = rotate8x8ImageClockwise( imageTMP1 );
    //memcpy(imageTMP1, imageTMP2, calculateMinimunByteForBitWidth( 8 )*8);
  }
  if ( (imgLargestValue <= 16) and (imgLargestValue > 8) ){
    Serial.println("resized16x16");
    imageTMP2 = rotate16x16ImageClockwise( imageTMP1 );
    memcpy(imageTMP1, imageTMP2, calculateMinimunByteForBitWidth( 16 )*16);
  }
  if ( (imgLargestValue <= 32) and (imgLargestValue > 16) ){
    Serial.println("resized32x32");
    imageTMP2 = rotate32x32ImageClockwise( imageTMP1 );
    memcpy(imageTMP1, imageTMP2, calculateMinimunByteForBitWidth( 32 )*32);
  }
  if ( (imgLargestValue <= 64) and (imgLargestValue > 32) ){
    Serial.println("resized64x64");
    imageTMP2 = rotate64x64ImageClockwise( imageTMP1 );
    memcpy(imageTMP1, imageTMP2, calculateMinimunByteForBitWidth( 64 )*64);
  }
  if ( (imgLargestValue <= 128) and (imgLargestValue > 64) ){
    Serial.println("resized128x128");
    rotate128x128ImageClockwise( imageTMP1 );
  }
  Serial.println(ESP.getFreeHeap());
  
  
  Serial.println("Rotated");
  //crop rotated image (besure to calculate image corner location!)
  imageOUT = (unsigned char *) realloc(imageOUT, inputbyteWidth*imgInPxHeight);
  if ( (imgLargestValue <= 8) and (imgLargestValue > 0) ){
    Serial.println("Crop8x8");
    cropBitImagefromBitImage(imageOUT, imageTMP1, imgInPxHeight, imgInPxWidth, 8-inputbyteWidth*8, 0, 8, 8);
  }
  if ( (imgLargestValue <= 16) and (imgLargestValue > 8) ){
    Serial.println("Crop16x16");
    cropBitImagefromBitImage(imageOUT, imageTMP1, imgInPxHeight, imgInPxWidth, 16-inputbyteWidth*8, 0, 16, 16);
  }
  if ( (imgLargestValue <= 32) and (imgLargestValue > 16) ){
    Serial.println("Crop32x32");
    cropBitImagefromBitImage(imageOUT, imageTMP1, imgInPxHeight, imgInPxWidth, 32-inputbyteWidth*8, 0, 32, 32);
  }
  if ( (imgLargestValue <= 64) and (imgLargestValue > 32) ){
    Serial.println("Crop64x64");
    cropBitImagefromBitImage(imageOUT, imageTMP1, imgInPxHeight, imgInPxWidth, 64-inputbyteWidth*8, 0, 64, 64);
  }
  if ( (imgLargestValue <= 128) and (imgLargestValue > 64) ){
    Serial.println("Crop128x128");
    cropBitImagefromBitImage(imageOUT, imageTMP1, imgInPxHeight, imgInPxWidth, 128-inputbyteWidth*8, 0, 128, 128);
  }
  free(imageTMP1);
  Serial.println("Cropped");
  Serial.println("rotateClockwiseComplete");
}
