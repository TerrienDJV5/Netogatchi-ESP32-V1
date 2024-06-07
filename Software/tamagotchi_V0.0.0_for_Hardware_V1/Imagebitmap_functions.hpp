//By Terrien-DJV5
//Imagebitmap_functions.hpp
//Date Created: june 6 2024 7:27PM

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
  
  
  char file_DIB_Header[12 or 64 or 16 or 40 or 52 or 56 or 108 or 124];
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
  
  unsigned int read_len;
  unsigned int avail_len = fileBMP.available();
  bool seekResult;
  
  //read File Header
  (void)fileBMP.seek(0);
  BITMAPFILEHEADER_byteArray_Struct * bitmapHeaderByteArray = new BITMAPFILEHEADER_byteArray_Struct;
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
  /*Free up raw_DIB_Header it is no longer needed!*/
  Serial.println("-----Free Up raw_DIB_Header Memory-----");
  printFreeHeap(Serial);
  free(raw_DIB_Header);
  *raw_DIB_Header = NULL;
  printFreeHeap(Serial);
  Serial.println("-----Free Up aw_DIB_Header Memory Complete-----");
  //Process DIB Header Complete!
    
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
  *raw_Pixel_Array = NULL;
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
