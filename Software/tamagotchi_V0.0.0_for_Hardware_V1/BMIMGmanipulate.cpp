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
unsigned char BMIMGmanipulate::getRShiftAND( unsigned char valueIN, byte rshiftCount, byte andValue ){
  return ((valueIN>>rshiftCount) & andValue);
}
bool BMIMGmanipulate::get8x8imageBit(unsigned char imageIN8x8[], byte xIN, byte yIN){
  byte x = xIN & B00000111;//3Bits
  byte y = yIN & B00000111;//3Bits
  byte xSel = (B00000001 << x);
  return ( (imageIN8x8[ y ] & xSel) == xSel);
}
void BMIMGmanipulate::set8x8imageBit(unsigned char imageIN8x8[], byte xIN, byte yIN, bool newState){
  byte x = xIN & B00000111;//3Bits
  byte y = yIN & B00000111;//3Bits
  byte xSel = (B00000001 << x);
  if (newState == true){
    imageIN8x8[y] = imageIN8x8[y] | xSel;
  }else{
    imageIN8x8[y] = imageIN8x8[y] & ~xSel;
  }
}
byte BMIMGmanipulate::ringRShift_Byte(byte initValue, byte shiftOffsetIN){//B00000001 -> B10000000
  byte shiftOffset = shiftOffsetIN & B00000111;
  byte cache;
  cache = initValue << (7-shiftOffset);
  return ((initValue >> shiftOffset) | cache);
}

void BMIMGmanipulate::rowRShift8x8DecFunc(unsigned char rowRShift8[], unsigned char initValue, byte shiftOffsetIN){
  byte shiftOffset = shiftOffsetIN & B00000111;
  rowRShift8[(0+shiftOffset)%8] = initValue>>(7);
  rowRShift8[(1+shiftOffset)%8] = initValue>>(6);
  rowRShift8[(2+shiftOffset)%8] = initValue>>(5);
  rowRShift8[(3+shiftOffset)%8] = initValue>>(4);
  rowRShift8[(4+shiftOffset)%8] = initValue>>(3);
  rowRShift8[(5+shiftOffset)%8] = initValue>>(2);
  rowRShift8[(6+shiftOffset)%8] = initValue>>(1);
  rowRShift8[(7+shiftOffset)%8] = initValue>>(0);
}
void BMIMGmanipulate::rowLShift8x8DecFunc(unsigned char rowLShift8[], unsigned char initValue, byte shiftOffsetIN){
  byte shiftOffset = shiftOffsetIN & B00000111;
  rowLShift8[(0+shiftOffset)%8] = initValue<<(7);
  rowLShift8[(1+shiftOffset)%8] = initValue<<(6);
  rowLShift8[(2+shiftOffset)%8] = initValue<<(5);
  rowLShift8[(3+shiftOffset)%8] = initValue<<(4);
  rowLShift8[(4+shiftOffset)%8] = initValue<<(3);
  rowLShift8[(5+shiftOffset)%8] = initValue<<(2);
  rowLShift8[(6+shiftOffset)%8] = initValue<<(1);
  rowLShift8[(7+shiftOffset)%8] = initValue<<(0);
}
void BMIMGmanipulate::rowRShift8x8IncFunc(unsigned char rowRShift8[], unsigned char initValue, byte shiftOffsetIN){
  byte shiftOffset = shiftOffsetIN & B00000111;
  rowRShift8[(0+shiftOffset)%8] = initValue>>(0);
  rowRShift8[(1+shiftOffset)%8] = initValue>>(1);
  rowRShift8[(2+shiftOffset)%8] = initValue>>(2);
  rowRShift8[(3+shiftOffset)%8] = initValue>>(3);
  rowRShift8[(4+shiftOffset)%8] = initValue>>(4);
  rowRShift8[(5+shiftOffset)%8] = initValue>>(5);
  rowRShift8[(6+shiftOffset)%8] = initValue>>(6);
  rowRShift8[(7+shiftOffset)%8] = initValue>>(7);
}
void BMIMGmanipulate::rowLShift8x8IncFunc(unsigned char rowLShift8[], unsigned char initValue, byte shiftOffsetIN){
  byte shiftOffset = shiftOffsetIN & B00000111;
  rowLShift8[(0+shiftOffset)%8] = initValue<<(0);
  rowLShift8[(1+shiftOffset)%8] = initValue<<(1);
  rowLShift8[(2+shiftOffset)%8] = initValue<<(2);
  rowLShift8[(3+shiftOffset)%8] = initValue<<(3);
  rowLShift8[(4+shiftOffset)%8] = initValue<<(4);
  rowLShift8[(5+shiftOffset)%8] = initValue<<(5);
  rowLShift8[(6+shiftOffset)%8] = initValue<<(6);
  rowLShift8[(7+shiftOffset)%8] = initValue<<(7);
}


void BMIMGmanipulate::rowRShift8x8_ShiftArrayFunc(unsigned char rowRShift8[], byte shiftx8_IN[], unsigned char initValue){
  rowRShift8[0] = initValue>>(shiftx8_IN[0]);
  rowRShift8[1] = initValue>>(shiftx8_IN[1]);
  rowRShift8[2] = initValue>>(shiftx8_IN[2]);
  rowRShift8[3] = initValue>>(shiftx8_IN[3]);
  rowRShift8[4] = initValue>>(shiftx8_IN[4]);
  rowRShift8[5] = initValue>>(shiftx8_IN[5]);
  rowRShift8[6] = initValue>>(shiftx8_IN[6]);
  rowRShift8[7] = initValue>>(shiftx8_IN[7]);
}
void BMIMGmanipulate::rowLShift8x8_ShiftArrayFunc(unsigned char rowLShift8[], byte shiftx8_IN[], unsigned char initValue){
  rowLShift8[0] = initValue<<(shiftx8_IN[0]);
  rowLShift8[1] = initValue<<(shiftx8_IN[1]);
  rowLShift8[2] = initValue<<(shiftx8_IN[2]);
  rowLShift8[3] = initValue<<(shiftx8_IN[3]);
  rowLShift8[4] = initValue<<(shiftx8_IN[4]);
  rowLShift8[5] = initValue<<(shiftx8_IN[5]);
  rowLShift8[6] = initValue<<(shiftx8_IN[6]);
  rowLShift8[7] = initValue<<(shiftx8_IN[7]);
}

void BMIMGmanipulate::row8xUnsignedChar_logicArrayFunc(unsigned char outputx8[], byte shiftx8_IN[], char logicTypex8_IN[], unsigned char initValue){
  for (uint16_t index = 0; index < 8; index++) {
    if (logicTypex8_IN[ index ] == ' '){
      outputx8[ index ] = initValue;
      continue;
    }
    if (logicTypex8_IN[ index ] == 'R'){//>>
      outputx8[ index ] = initValue>>(shiftx8_IN[ index ]);
      continue;
    }
    if (logicTypex8_IN[ index ] == 'L'){//<<
      outputx8[ index ] = initValue<<(shiftx8_IN[ index ]);
      continue;
    }
  }
}

void BMIMGmanipulate::logicOP_UnsignedCharX1_logicArrayFunc(unsigned char &output, unsigned char &input0, unsigned char &input1, logicTypes logicType_IN){
  if (logicType_IN == passThrough){
    output = input0;
  }
  if (logicType_IN == bitwiseRShift){//>>
    output = input0 >> input1;
  }
  if (logicType_IN == bitwiseLShift){//<<
    output = input0 << input1;
  }
  if (logicType_IN == bitwiseAND){
    output = input0 & input1;
  }
  if (logicType_IN == bitwiseOR){
    output = input0 | input1;
  }
  if (logicType_IN == bitwiseXOR){
    output = input0 ^ input1;
  }
  if (logicType_IN == bitSelectNOT){//Selective Bitwise Not
    //input0 is the Value
    //input1 is the select
    output = ~(input0 & input1) | (input0 & (~input1));
  }
  if (logicType_IN == inputSwap){//Swap inputs 0 and inputs 1 and leave Output Alone
    unsigned char temp;
    temp = input0;
    input0 = input1;
    input1 = temp;
  }
}


void BMIMGmanipulate::logicOP_UnsignedCharX8_logicArrayFunc(unsigned char arrayx8_Output[], unsigned char arrayx8_Input0[], unsigned char arrayx8_Input1[], logicTypes logicTypex8_IN[]){
  logicOP_UnsignedCharX1_logicArrayFunc(arrayx8_Output[ 0 ], arrayx8_Input0[ 0 ], arrayx8_Input1[ 0 ], logicTypex8_IN[ 0 ]);
  logicOP_UnsignedCharX1_logicArrayFunc(arrayx8_Output[ 1 ], arrayx8_Input0[ 1 ], arrayx8_Input1[ 1 ], logicTypex8_IN[ 1 ]);
  logicOP_UnsignedCharX1_logicArrayFunc(arrayx8_Output[ 2 ], arrayx8_Input0[ 2 ], arrayx8_Input1[ 2 ], logicTypex8_IN[ 2 ]);
  logicOP_UnsignedCharX1_logicArrayFunc(arrayx8_Output[ 3 ], arrayx8_Input0[ 3 ], arrayx8_Input1[ 3 ], logicTypex8_IN[ 3 ]);
  logicOP_UnsignedCharX1_logicArrayFunc(arrayx8_Output[ 4 ], arrayx8_Input0[ 4 ], arrayx8_Input1[ 4 ], logicTypex8_IN[ 4 ]);
  logicOP_UnsignedCharX1_logicArrayFunc(arrayx8_Output[ 5 ], arrayx8_Input0[ 5 ], arrayx8_Input1[ 5 ], logicTypex8_IN[ 5 ]);
  logicOP_UnsignedCharX1_logicArrayFunc(arrayx8_Output[ 6 ], arrayx8_Input0[ 6 ], arrayx8_Input1[ 6 ], logicTypex8_IN[ 6 ]);
  logicOP_UnsignedCharX1_logicArrayFunc(arrayx8_Output[ 7 ], arrayx8_Input0[ 7 ], arrayx8_Input1[ 7 ], logicTypex8_IN[ 7 ]);
}



void BMIMGmanipulate::rotate8x8ImageClockwise(unsigned char imageIN8x8[]){
  unsigned char* imageOutput;
  imageOutput = (unsigned char*)calloc(8, sizeof(unsigned char));
  memset(imageOutput,0,8);
  byte* cS;//columnSelect
  cS = (byte*)calloc(8, sizeof(byte));
  cS[0] = B00000001;cS[1] = B00000010;cS[2] = B00000100;cS[3] = B00001000;cS[4] = B00010000;cS[5] = B00100000;cS[6] = B01000000;cS[7] = B10000000;
  unsigned char* rowValues;
  unsigned char* arrayCache0;
  unsigned char* arrayCache1;
  byte* shift8Values;
  logicTypes* shift8TypeValues;
  rowValues = (unsigned char*)calloc(8, sizeof(unsigned char));
  arrayCache0 = (unsigned char*)calloc(8, sizeof(unsigned char));
  arrayCache1 = (unsigned char*)calloc(8, sizeof(unsigned char));
  shift8Values = (byte*)calloc(8, sizeof(byte));
  shift8TypeValues = (logicTypes*)calloc(8, sizeof(logicTypes));
  memset(rowValues,0,8);
  rowRShift8x8DecFunc(rowValues, imageIN8x8[0], 0);
  rowLShift8x8IncFunc(rowValues, imageIN8x8[0], 0);
  rowRShift8x8_ShiftArrayFunc(rowValues, shift8Values, imageIN8x8[0]);
  rowLShift8x8_ShiftArrayFunc(rowValues, shift8Values, imageIN8x8[0]);

  memset(arrayCache0, imageIN8x8[0], 8);
  memcpy(arrayCache1, (const unsigned char[]){7,6,5,4,3,2,1,0}, 8*sizeof(unsigned char));
  memcpy(shift8TypeValues, (const logicTypes[]){bitwiseRShift,bitwiseRShift,bitwiseRShift,bitwiseRShift,bitwiseRShift,bitwiseRShift,bitwiseRShift,passThrough}, 8*sizeof(logicTypes));
  logicOP_UnsignedCharX8_logicArrayFunc(rowValues, arrayCache0, arrayCache1, shift8TypeValues);
  memset(arrayCache1, 0B00000001, 8);
  memcpy(shift8TypeValues, (const logicTypes[]){bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND}, 8*sizeof(logicTypes));
  logicOP_UnsignedCharX8_logicArrayFunc(rowValues, rowValues, arrayCache1, shift8TypeValues);
  memcpy(shift8TypeValues, (const logicTypes []){bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR}, 8*sizeof(logicTypes));
  logicOP_UnsignedCharX8_logicArrayFunc(imageOutput, imageOutput, rowValues, shift8TypeValues);
  
  memset(arrayCache0, imageIN8x8[1], 8);
  memcpy(arrayCache1, (const unsigned char[]){6,5,4,3,2,1,0,1}, 8*sizeof(unsigned char));
  memcpy(shift8TypeValues, (const logicTypes[]){bitwiseRShift,bitwiseRShift,bitwiseRShift,bitwiseRShift,bitwiseRShift,bitwiseRShift,passThrough,bitwiseLShift}, 8*sizeof(logicTypes));
  logicOP_UnsignedCharX8_logicArrayFunc(rowValues, arrayCache0, arrayCache1, shift8TypeValues);
  memset(arrayCache1, 0B00000010, 8);
  memcpy(shift8TypeValues, (const logicTypes[]){bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND}, 8*sizeof(logicTypes));
  logicOP_UnsignedCharX8_logicArrayFunc(rowValues, rowValues, arrayCache1, shift8TypeValues);
  memcpy(shift8TypeValues, (const logicTypes[]){bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR}, 8*sizeof(logicTypes));
  logicOP_UnsignedCharX8_logicArrayFunc(imageOutput, imageOutput, rowValues, shift8TypeValues);
  

  memset(arrayCache0, imageIN8x8[2], 8);
  memcpy(arrayCache1, (const unsigned char[]){5,4,3,2,1,0,1,2}, 8*sizeof(unsigned char));
  memcpy(shift8TypeValues, (const logicTypes[]){bitwiseRShift,bitwiseRShift,bitwiseRShift,bitwiseRShift,bitwiseRShift,passThrough,bitwiseLShift,bitwiseLShift}, 8*sizeof(logicTypes));
  logicOP_UnsignedCharX8_logicArrayFunc(rowValues, arrayCache0, arrayCache1, shift8TypeValues);
  memset(arrayCache1, 0B00000100, 8);
  memcpy(shift8TypeValues, (const logicTypes[]){bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND}, 8*sizeof(logicTypes));
  logicOP_UnsignedCharX8_logicArrayFunc(rowValues, rowValues, arrayCache1, shift8TypeValues);
  memcpy(shift8TypeValues, (const logicTypes[]){bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR}, 8*sizeof(logicTypes));
  logicOP_UnsignedCharX8_logicArrayFunc(imageOutput, imageOutput, rowValues, shift8TypeValues);

  memset(arrayCache0, imageIN8x8[3], 8);
  memcpy(arrayCache1, (const unsigned char[]){4,3,2,1,0,1,2,3}, 8*sizeof(unsigned char));
  memcpy(shift8TypeValues, (const logicTypes[]){bitwiseRShift,bitwiseRShift,bitwiseRShift,bitwiseRShift,passThrough,bitwiseLShift,bitwiseLShift,bitwiseLShift}, 8*sizeof(logicTypes));
  logicOP_UnsignedCharX8_logicArrayFunc(rowValues, arrayCache0, arrayCache1, shift8TypeValues);
  memset(arrayCache1, 0B00001000, 8);
  memcpy(shift8TypeValues, (const logicTypes[]){bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND}, 8*sizeof(logicTypes));
  logicOP_UnsignedCharX8_logicArrayFunc(rowValues, rowValues, arrayCache1, shift8TypeValues);
  memcpy(shift8TypeValues, (const logicTypes[]){bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR}, 8*sizeof(logicTypes));
  logicOP_UnsignedCharX8_logicArrayFunc(imageOutput, imageOutput, rowValues, shift8TypeValues);


  memset(arrayCache0, imageIN8x8[4], 8);
  memcpy(arrayCache1, (const unsigned char[]){3,2,1,0,1,2,3,4}, 8*sizeof(unsigned char));
  memcpy(shift8TypeValues, (const logicTypes[]){bitwiseRShift,bitwiseRShift,bitwiseRShift,passThrough,bitwiseLShift,bitwiseLShift,bitwiseLShift,bitwiseLShift}, 8*sizeof(logicTypes));
  logicOP_UnsignedCharX8_logicArrayFunc(rowValues, arrayCache0, arrayCache1, shift8TypeValues);
  memset(arrayCache1, 0B00010000, 8);
  memcpy(shift8TypeValues, (const logicTypes[]){bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND}, 8*sizeof(logicTypes));
  logicOP_UnsignedCharX8_logicArrayFunc(rowValues, rowValues, arrayCache1, shift8TypeValues);
  memcpy(shift8TypeValues, (const logicTypes[]){bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR}, 8*sizeof(logicTypes));
  logicOP_UnsignedCharX8_logicArrayFunc(imageOutput, imageOutput, rowValues, shift8TypeValues);

  memset(arrayCache0, imageIN8x8[5], 8);
  memcpy(arrayCache1, (const unsigned char[]){2,1,0,1,2,3,4,5}, 8*sizeof(unsigned char));
  memcpy(shift8TypeValues, (const logicTypes[]){bitwiseRShift,bitwiseRShift,passThrough,bitwiseLShift,bitwiseLShift,bitwiseLShift,bitwiseLShift,bitwiseLShift}, 8*sizeof(logicTypes));
  logicOP_UnsignedCharX8_logicArrayFunc(rowValues, arrayCache0, arrayCache1, shift8TypeValues);
  memset(arrayCache1, 0B00100000, 8);
  memcpy(shift8TypeValues, (const logicTypes[]){bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND}, 8*sizeof(logicTypes));
  logicOP_UnsignedCharX8_logicArrayFunc(rowValues, rowValues, arrayCache1, shift8TypeValues);
  memcpy(shift8TypeValues, (const logicTypes[]){bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR}, 8*sizeof(logicTypes));
  logicOP_UnsignedCharX8_logicArrayFunc(imageOutput, imageOutput, rowValues, shift8TypeValues);


  memset(arrayCache0, imageIN8x8[6], 8);
  memcpy(arrayCache1, (const unsigned char[]){1,0,1,2,3,4,5,6}, 8*sizeof(unsigned char));
  memcpy(shift8TypeValues, (const char[]){bitwiseRShift,passThrough,bitwiseLShift,bitwiseLShift,bitwiseLShift,bitwiseLShift,bitwiseLShift,bitwiseLShift}, 8*sizeof(logicTypes));
  logicOP_UnsignedCharX8_logicArrayFunc(rowValues, arrayCache0, arrayCache1, shift8TypeValues);
  memset(arrayCache1, 0B01000000, 8);
  memcpy(shift8TypeValues, (const logicTypes[]){bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND}, 8*sizeof(logicTypes));
  logicOP_UnsignedCharX8_logicArrayFunc(rowValues, rowValues, arrayCache1, shift8TypeValues);
  memcpy(shift8TypeValues, (const logicTypes[]){bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR}, 8*sizeof(logicTypes));
  logicOP_UnsignedCharX8_logicArrayFunc(imageOutput, imageOutput, rowValues, shift8TypeValues);

  memset(arrayCache0, imageIN8x8[7], 8);
  memcpy(arrayCache1, (const unsigned char[]){0,1,2,3,4,5,6,7}, 8*sizeof(unsigned char));
  memcpy(shift8TypeValues, (const logicTypes[]){passThrough,bitwiseLShift,bitwiseLShift,bitwiseLShift,bitwiseLShift,bitwiseLShift,bitwiseLShift,bitwiseLShift}, 8*sizeof(logicTypes));
  logicOP_UnsignedCharX8_logicArrayFunc(rowValues, arrayCache0, arrayCache1, shift8TypeValues);
  memset(arrayCache1, 0B10000000, 8);
  memcpy(shift8TypeValues, (const logicTypes[]){bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND,bitwiseAND}, 8*sizeof(logicTypes));
  logicOP_UnsignedCharX8_logicArrayFunc(rowValues, rowValues, arrayCache1, shift8TypeValues);
  memcpy(shift8TypeValues, (const logicTypes[]){bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR,bitwiseOR}, 8*sizeof(logicTypes));
  logicOP_UnsignedCharX8_logicArrayFunc(imageOutput, imageOutput, rowValues, shift8TypeValues);
  

  
  
  /*
  //
  imageOutput[ 0 ] = getRShiftAND( imageIN8x8[0], 7, cS[0] ) | ((imageIN8x8[1]>>6) & B00000010);
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
  //*/
  memmove(imageIN8x8, imageOutput, 8);
  free(imageOutput);
  free(cS);
  free(rowValues);
  free(arrayCache0);
  free(arrayCache1);
  free(shift8Values);
  free(shift8TypeValues);
  
}



void BMIMGmanipulate::rotate16x16ImageClockwise(unsigned char imageInput[]){
  unsigned int imageByteWidth = 2;
  unsigned int imageByteHeight = 16;
  unsigned int fullImageByteSize = imageByteWidth*imageByteHeight;
  
  unsigned int sectorByteWidth = imageByteWidth>>1;
  unsigned int sectorByteHeight = imageByteHeight>>1;
  unsigned int sectorByteSize = sectorByteWidth*sectorByteHeight;
  
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
  rotate8x8ImageClockwise( sectorX0Y0 );
  rotate8x8ImageClockwise( sectorX1Y0 );
  rotate8x8ImageClockwise( sectorX0Y1 );
  rotate8x8ImageClockwise( sectorX1Y1 );
  
  //Rotate Sectors
  unsigned char *sectorStorage;
  sectorStorage = (unsigned char *)malloc(sectorByteSize);
  memmove(sectorStorage, sectorX1Y0, sectorByteSize );
  memmove(sectorX1Y0, sectorX0Y0, sectorByteSize );
  memmove(sectorX0Y0, sectorX0Y1, sectorByteSize );
  memmove(sectorX0Y1, sectorX1Y1, sectorByteSize );
  memmove(sectorX1Y1, sectorStorage, sectorByteSize );
  free(sectorStorage);
  
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


void BMIMGmanipulate::rotate32x32ImageClockwise(unsigned char imageInput[]){
  unsigned int imageByteWidth = 4;
  unsigned int imageByteHeight = 32;
  unsigned int fullImageByteSize = imageByteWidth*imageByteHeight;
  
  unsigned int sectorByteWidth = imageByteWidth>>1;
  unsigned int sectorByteHeight = imageByteHeight>>1;
  unsigned int sectorByteSize = sectorByteWidth*sectorByteHeight;
  
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
  rotate16x16ImageClockwise( sectorX0Y0 );
  rotate16x16ImageClockwise( sectorX1Y0 );
  rotate16x16ImageClockwise( sectorX0Y1 );
  rotate16x16ImageClockwise( sectorX1Y1 );
  
  //Rotate Sectors
  unsigned char *sectorStorage;
  sectorStorage = (unsigned char *)malloc(sectorByteSize);
  memmove(sectorStorage, sectorX1Y0, sectorByteSize );
  memmove(sectorX1Y0, sectorX0Y0, sectorByteSize );
  memmove(sectorX0Y0, sectorX0Y1, sectorByteSize );
  memmove(sectorX0Y1, sectorX1Y1, sectorByteSize );
  memmove(sectorX1Y1, sectorStorage, sectorByteSize );
  free(sectorStorage);
  
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





void BMIMGmanipulate::rotate64x64ImageClockwise(unsigned char imageInput[]){
  unsigned int imageByteWidth = 8;
  unsigned int imageByteHeight = 64;
  unsigned int fullImageByteSize = imageByteWidth*imageByteHeight;
  
  unsigned int sectorByteWidth = imageByteWidth>>1;
  unsigned int sectorByteHeight = imageByteHeight>>1;
  unsigned int sectorByteSize = sectorByteWidth*sectorByteHeight;
  
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
  rotate32x32ImageClockwise( sectorX0Y0 );
  rotate32x32ImageClockwise( sectorX1Y0 );
  rotate32x32ImageClockwise( sectorX0Y1 );
  rotate32x32ImageClockwise( sectorX1Y1 );
  
  //Rotate Sectors
  unsigned char *sectorStorage;
  sectorStorage = (unsigned char *)malloc(sectorByteSize);
  memmove(sectorStorage, sectorX1Y0, sectorByteSize );
  memmove(sectorX1Y0, sectorX0Y0, sectorByteSize );
  memmove(sectorX0Y0, sectorX0Y1, sectorByteSize );
  memmove(sectorX0Y1, sectorX1Y1, sectorByteSize );
  memmove(sectorX1Y1, sectorStorage, sectorByteSize );
  free(sectorStorage);
  
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


///*
void BMIMGmanipulate::rotate128x128ImageClockwise(unsigned char imageInput[]){
  unsigned int imageByteWidth = 16;
  unsigned int imageByteHeight = 128;
  unsigned int fullImageByteSize = imageByteWidth*imageByteHeight;
  
  unsigned int sectorByteWidth = imageByteWidth>>1;
  unsigned int sectorByteHeight = imageByteHeight>>1;
  unsigned int sectorByteSize = sectorByteWidth*sectorByteHeight;
  
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
  rotate64x64ImageClockwise( sectorX0Y0 );
  rotate64x64ImageClockwise( sectorX1Y0 );
  rotate64x64ImageClockwise( sectorX0Y1 );
  rotate64x64ImageClockwise( sectorX1Y1 );
  
  //Rotate Sectors
  unsigned char *sectorStorage;
  sectorStorage = (unsigned char *)malloc(sectorByteSize);
  memmove(sectorStorage, sectorX1Y0, sectorByteSize );
  memmove(sectorX1Y0, sectorX0Y0, sectorByteSize );
  memmove(sectorX0Y0, sectorX0Y1, sectorByteSize );
  memmove(sectorX0Y1, sectorX1Y1, sectorByteSize );
  memmove(sectorX1Y1, sectorStorage, sectorByteSize );
  free(sectorStorage);
  
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

//890925  ->  //890781  ->  //890829  ->  //892149  ->  //
//59392   ->  //59232   ->  //59232   ->  //59232   ->  //




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
    //imageTMP2 = rotate16x16ImageClockwise( imageTMP1 );
    //memcpy(imageTMP1, imageTMP2, calculateMinimunByteForBitWidth( 16 )*16);
    rotate16x16ImageClockwise( imageTMP1 );
  }
  if ( (imgLargestValue <= 32) and (imgLargestValue > 16) ){
    Serial.println("resized32x32");
    //imageTMP2 = rotate32x32ImageClockwise( imageTMP1 );
    //memcpy(imageTMP1, imageTMP2, calculateMinimunByteForBitWidth( 32 )*32);
    rotate32x32ImageClockwise( imageTMP1 );
  }
  if ( (imgLargestValue <= 64) and (imgLargestValue > 32) ){
    Serial.println("resized64x64");
    //imageTMP2 = rotate64x64ImageClockwise( imageTMP1 );
    //memcpy(imageTMP1, imageTMP2, calculateMinimunByteForBitWidth( 64 )*64);
    rotate64x64ImageClockwise( imageTMP1 );
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
