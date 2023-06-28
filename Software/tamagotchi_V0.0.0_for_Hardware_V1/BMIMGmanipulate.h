//By Terrien-DJV5
//BMIMGmanipulate.h
//Date Created: may 30 2023 11:23AM

#ifndef MY_BMIMGMANIPULATE_H
#define MY_BMIMGMANIPULATE_H
#include <Arduino.h>
class BMIMGmanipulate {
  private:
    enum logicTypes {
      passThrough,
      bitwiseRShift,
      bitwiseLShift,
      bitwiseAND,
      bitwiseOR,
      bitwiseXOR,
      bitSelectNOT,
      inputSwap,
    };
    logicTypes variable = passThrough;
    
    bool staticBufferFlag;
    unsigned char *mainBuffer;//[(16)*128] //128x128px
    
    unsigned int calculateMinimunByteForBitWidth( unsigned int bitWidth );
    unsigned char getRShiftAND( unsigned char valueIN, byte rshiftCount, byte andValue );
    bool get8x8imageBit(unsigned char imageIN8x8[], byte xIN, byte yIN);
    void set8x8imageBit(unsigned char imageIN8x8[], byte xIN, byte yIN, bool newState);
    byte ringRShift_Byte(byte initValue, byte shiftOffsetIN);
    
    void arrayx8_IndexShiftoffset_unsignedChar_Func(unsigned char inputx8[], byte shiftOffsetIN, bool incFlag = true);
    
    void rowRShift8x8_ShiftArrayFunc(unsigned char rowRShift8[], const byte shiftx8_IN[], unsigned char initValue);
    void rowLShift8x8_ShiftArrayFunc(unsigned char rowLShift8[], const byte shiftx8_IN[], unsigned char initValue);
    
    void rowRShift8x8DecFunc(unsigned char rowRShift8[], unsigned char initValue, byte shiftOffsetIN = 0);
    void rowLShift8x8DecFunc(unsigned char rowLShift8[], unsigned char initValue, byte shiftOffsetIN = 0);
    void rowRShift8x8IncFunc(unsigned char rowRShift8[], unsigned char initValue, byte shiftOffsetIN = 0);
    void rowLShift8x8IncFunc(unsigned char rowLShift8[], unsigned char initValue, byte shiftOffsetIN = 0);
    
    void logicOP_UnsignedCharX1_logicArrayFunc(unsigned char &output, unsigned char &input0, unsigned char &input1, const logicTypes logicType_IN);
    void logicOP_UnsignedCharX8_logicArrayFunc(unsigned char arrayx8_Output[], unsigned char arrayx8_Input0[], unsigned char arrayx8_Input1[], const logicTypes logicTypex8_IN[]);
    
    void rotate8x8ImageClockwise(unsigned char imageIN8x8[]);
    void rotate16x16ImageClockwise(unsigned char imageInput[]);
    void rotate32x32ImageClockwise(unsigned char imageInput[]);
    void rotate64x64ImageClockwise(unsigned char imageInput[]);
    void rotate128x128ImageClockwise(unsigned char imageInput[]);
    void rotate_ImageClockwise_recursionFunc(unsigned char imageInput[], unsigned int imageByteWidth, unsigned int imageByteHeight );
    
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
