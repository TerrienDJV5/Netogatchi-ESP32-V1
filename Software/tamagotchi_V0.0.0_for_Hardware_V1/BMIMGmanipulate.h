//By Terrien-DJV5
//BMIMGmanipulate.h
//Date Created: may 30 2023 11:23AM

#ifndef MY_BMIMGMANIPULATE_H
#define MY_BMIMGMANIPULATE_H
#include <Arduino.h>
class BMIMGmanipulate {
  private:
    bool staticBufferFlag;
    unsigned char *mainBuffer;//[(16)*128] //128x128px
    
    unsigned int calculateMinimunByteForBitWidth( unsigned int bitWidth );
    unsigned char* rotate8x8ImageClockwise(unsigned char imageIN8x8[8]);
    unsigned char* rotate16x16ImageClockwise(unsigned char imageIN16x16[2*16]);
    unsigned char* rotate32x32ImageClockwise(unsigned char imageIN32x32[4*32]);
    unsigned char* rotate64x64ImageClockwise(unsigned char imageIN64x64[8*64]);
    void rotate128x128ImageClockwise(unsigned char imageInput[]);
    
  public:
    BMIMGmanipulate(bool useStaticBuffer);
    void init();
    
    void resizeBitImage(unsigned char imgOut[], unsigned char imgIn[], uint16_t imgOutPxWidth, uint16_t imgOutPxHeight, uint16_t imgInPxWidth, uint16_t imgInPxHeight);
    void cropBitImagefromBitImage(unsigned char imageOutput[], unsigned char imageInput[], uint16_t imgOutputPxWidth, uint16_t imgOutputPxHeight, uint16_t imgInputCutXpos, uint16_t imgInputCutYpos, uint16_t imgInputPxWidth, uint16_t imgInputPxHeight);
    void rotateBitImageClockwise(unsigned char imageOUT[], unsigned char imageIN[], uint16_t imgInPxWidth, uint16_t imgInPxHeight); //rotates 90 deg clockwise //new Px Width and Height will be flipped
    
    
};
#endif




/*
//Pre Defined Functions
*/
//https://www.tutorialspoint.com/arduino/arduino_passing_arrays_to_functions.htm
/*
void rotate128x128ImageClockwise(unsigned char []);

void resizeBitImage(unsigned char [], unsigned char [], uint16_t, uint16_t, uint16_t, uint16_t);
void cropBitImagefromBitImage(unsigned char[], unsigned char[], uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);
void rotateBitImageClockwise(unsigned char [], unsigned char [], uint16_t, uint16_t);//rotates 90 deg clockwise //new Px Width and Height will be flipped
//*/
/*
//Pre Defined Functions (End)
*/
