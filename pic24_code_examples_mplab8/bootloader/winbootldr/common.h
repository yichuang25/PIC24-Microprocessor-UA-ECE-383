#pragma once


namespace winbootldr {

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::IO;


enum class eFamily {
  dsPIC30F = 0,
  dsPIC33F,
  PIC24H,
  PIC24F,
  PIC24FK,
  dsPIC33E,
  PIC24E
};

enum class eType {
  Program = 0,
  EEProm,
  Configuration
};

#define PM_SIZE 1536 /* Max: 144KB/3/32=1536 PM rows for 30F. */
#define EE_SIZE 128 /* 4KB/2/16=128 EE rows */
#define CM_SIZE 12
#define PM30F_ROW_SIZE 32

/*
64*8*3 is the size of an entire page (1536 bytes) for PM33F
This is too large for the PIC24HJ202 to keep in memory, so need to
cut down the row size to one half that size.
*/
//#define PM33F_ROW_SIZE 64*4
#define PM33F_ROW_SIZE_LARGE 64*8
#define PM33F_ROW_SIZE_SMALL 64*4   //for devices with < 2K SRAM
#define PIC24FK_ROW_SIZE 32*4       //PIC24FK only has 32 instructions per row


#define EE30F_ROW_SIZE 16

#define COMMAND_NACK     0x00
#define COMMAND_ACK      0x01
#define COMMAND_READ_PM  0x02
#define COMMAND_WRITE_PM 0x03
#define COMMAND_READ_EE  0x04
#define COMMAND_WRITE_EE 0x05
#define COMMAND_READ_CM  0x06
#define COMMAND_WRITE_CM 0x07
#define COMMAND_RESET    0x08
#define COMMAND_READ_ID  0x09
#define COMMAND_READ_VERSION  0x011
#define COMMAND_POR_RESET  0x13

#define BUFFER_SIZE         4096
#define READ_BUFFER_TIMEOUT 1000

#define PROGRAM_START 0xC00



public ref class common  {

public:
  System::Windows::Forms::TextBox^  debugOutLocal;

public:
  Byte GetAsciiHexDigit (Byte val) {
    if (val < 10) return('0'+val);
    else return('A' + (val-10));
  }



public:
  Void ToAsciiHex(Byte val, array<Byte>^ p, Int32 base) {
    Byte digit;

    digit = (val >> 4) & 0x0F;
    p[base]=GetAsciiHexDigit(digit);
    digit = val & 0x0F;
    p[base+1]=GetAsciiHexDigit(digit);
  }

public:
  Byte AsciiDigit2Byte(Byte c) {
    c = c & 0x7F;
    if (c > 'Z') c = c - 0x20;
    if (c > '9') c = (c - 'A') + 0xA;
    else c = c - '0';
    return(c);
  }

public:
  Byte Hex2ToByte(array<Byte>^ p, Int32 base) {
    Byte val;

    val = AsciiDigit2Byte(p[base]);
    val = (val << 4) + AsciiDigit2Byte(p[base+1]);

    return val;
  }

public:
  UInt16 Hex4ToUInt16 (array<Byte>^ p,Int32 base ) {
    UInt16 val;

    val = Hex2ToByte(p,base);
    val = val << 8;
    val = val + Hex2ToByte(p,base+2);
    return val;
  }

public:
  UInt32 Hex8ToUInt32 (array<Byte>^ p,Int32 base ) {
    UInt32 val;

    val = Hex4ToUInt16(p,base);
    val = val << 16;
    val = val + Hex4ToUInt16(p,base+4);
    return val;
  }

  Boolean ComRead(System::IO::Ports::SerialPort^ Port, array<Byte>^ Buffer,
                  UInt32 base, UInt32 count) {
    UInt32 i;
    for (i=0; i<count; i++) {
      try {
        Buffer[base]=Port->ReadByte();
      } catch ( Exception^ e ) {
        debugOutLocal->AppendText(String::Concat("Exception during serial port read: ",e->Message,"\r\n"));
        return false;
      }
      base++;
    }
    return true;
  }

  Boolean ComWrite(System::IO::Ports::SerialPort^ Port, array<Byte>^ Buffer,
                   UInt32 base, UInt32 count) {
    try {
      Port->Write(Buffer,base,count);
    } catch ( Exception^ e ) {
      debugOutLocal->AppendText(String::Concat("Exception during serial port write: ",e->Message,"\r\n"));
      return false;
    }
    return true;
  }




};



}