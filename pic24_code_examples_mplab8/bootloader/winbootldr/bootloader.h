#pragma once

/*
 Adopted from C code orginally provided by Microchip.
 This version written by R. Reese (reese@ece.msstate.edu)
 Mississippi State University

*/

namespace winbootldr {

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::IO;
using namespace System::Text;


public ref class device {

public:
  String ^ name;
  Int32 Id;
  eFamily fname;
  UInt32 ConfigPage;
  UInt32 ConfigWord;
  Int32 revision;
  Int32 ProcessID;
  Boolean SmallRAM;


public:

  device(String^ a_name, Int32 a_Id, Int32 a_PID, eFamily a_fname, Int32 a_num, Boolean a_SmallRAM) {

    Id = a_Id;
    name = a_name;
    fname = a_fname;
    ProcessID = a_PID;
    //ConfigPage = a_num;
    //start address of memory page with config bits, only used for PIC24F
    ConfigPage = (a_num/1024)*1024;
    ConfigWord = a_num;
    revision = 0;
    SmallRAM = a_SmallRAM;
  }
};

public ref class bootloader : public common {
private:
  Boolean          m_ConfigBitEnable;
  int m_FirmwareVersion;



public:
  bootloader(void) {

    devices = gcnew System::Collections::ArrayList();
    DeviceFound = false;
    CurrentDevice = nullptr;
    m_ConfigBitEnable = true;
    m_FirmwareVersion = 0;
    //initDevices();    //this is obsolete, read the device file instead.
  }

  System::Void EnableConfigBitProgramming() {
    m_ConfigBitEnable = true;
  }

  System::Void DisableConfigBitProgramming() {
    m_ConfigBitEnable = false;
  }


  device^ findDeviceById(UInt32 a_Id) {
    IEnumerator^ myEnum = devices->GetEnumerator();
    while ( myEnum->MoveNext() ) {
      device^ a_device = safe_cast<device^> (myEnum->Current);
      if (a_device->Id == a_Id) return(a_device);

    }
    return nullptr;
  }



  device^ ReadDevice(System::IO::Ports::SerialPort^ Port,
                     System::Windows::Forms::TextBox^  myDebugOut) {

    array<Byte>^ Buffer = gcnew array<Byte>(BUFFER_SIZE);
    UInt16  DeviceId = 0;
    UInt16  Revision = 0;
    UInt16  ProcessID = 0;
    device^  a_device;

    Buffer[0] = COMMAND_READ_ID;
    debugOutLocal = myDebugOut;

    myDebugOut->AppendText("\r\nReading Target Device ID\r\n");

    if (ComWrite(Port,Buffer,0,1)== false) return nullptr;  //WriteCommBlock(pComDev, Buffer, 1);


    if (ComRead(Port, Buffer, 0, 8) == false) return nullptr;//ReceiveData(pComDev, Buffer, 8);

    myDebugOut->AppendText("Received data in ReadDevice\r\n");

    DeviceId  = ((Buffer[1] << 8)&0xFF00) | (Buffer[0]&0x00FF);
    ProcessID = (Buffer[5] >> 4) & 0x0F;
    Revision = ((Buffer[5] << 8)&0xFF00) | (Buffer[4]&0x00FF);

    IEnumerator^ myEnum = devices->GetEnumerator();
    while ( myEnum->MoveNext() ) {
      a_device = safe_cast<device^> (myEnum->Current);
      //only check the device id.
      if ( (a_device->Id == DeviceId) &&  (a_device->ProcessID == ProcessID)) {
        //found device
        CurrentDevice = a_device;
        //save revision
        a_device->revision = Revision;
        return a_device;
      }

    }
    myDebugOut->AppendText(String::Concat("Device ID: ",Convert::ToString(DeviceId,16),", Process ID: ",Convert::ToString(ProcessID,16),"\r\n"));
    myDebugOut->AppendText("Unknown PICmicro, cannot program. Check device or check baud rate.\r\n");

    return nullptr;

  }

  //checks if page starts at 0x400, if so, this page definitely clashes with bootloader.
  Boolean CheckAddressClash(UInt32 Address, eFamily Family) {
    if (( (Family == eFamily::PIC24H) ||(Family == eFamily::dsPIC33F)
          ||(Family == eFamily::PIC24E) ||(Family == eFamily::dsPIC33E)
          ||(Family == eFamily::PIC24FK) ||(Family == eFamily::PIC24F)) &&
        (Address == 0x400)) return false;
    return true;
  }

  //ensures that each word between >= 0x200 and <0xC00 is == 0xFFFF or else we clash with bootloader!
  Boolean CheckAddressClash2(UInt32 Address, array<Byte>^ pData, Int32 base, eFamily Family) {
    if ( (Family == eFamily::PIC24H) ||(Family == eFamily::PIC24F)
         ||(Family == eFamily::PIC24E) ||(Family == eFamily::dsPIC33E)
         ||(Family == eFamily::PIC24FK) ||(Family == eFamily::dsPIC33F) ) {
      if (Address >= 0x200 && Address < 0xC00 && Hex4ToUInt16(pData, base) != 0xFFFF)
        return false;
    }
    return true;
  }

  //If PIC24F/PIC24E/dsPIC33E code is located on last page, and configuration bit programming is not
  //enabled, then abort
  Boolean CheckAddressClash3(UInt32 Address, array<Byte>^ pData, Int32 base, eFamily Family,UInt32 ConfigPage,UInt32 ConfigWord) {
    if ( (Family == eFamily::PIC24F) || (Family == eFamily::PIC24E) ||(Family == eFamily::dsPIC33E)) {
      if (m_ConfigBitEnable) return true;
      if (Address >= ConfigPage && Address < (ConfigWord) && Hex4ToUInt16(pData, base) != 0xFFFF)
        return false;
    }

    return true;
  }



  Boolean SkipRowPM(mem_cMemRow^ thisRow, eFamily Family,
                    System::Windows::Forms::TextBox^  myDebugOut) {

    Boolean rval;
    unsigned int i;
    i = thisRow->GetRowAddress();
    rval = false;

    if (this->m_FirmwareVersion >= 3  && i < PROGRAM_START) {
      return true; //do not write any rows below program start
    }

    if ((Family == eFamily::PIC24F || Family == eFamily::PIC24E || Family == eFamily::dsPIC33E)
        && m_ConfigBitEnable == false) {

      //check config bit page
      if ((i >= CurrentDevice->ConfigPage) && (thisRow->GetRowEmpty()==false)) {
        myDebugOut->AppendText("Skipping Mem Row 0x");
        myDebugOut->AppendText(Convert::ToString((int) i,16));
        myDebugOut->AppendText(", on Configuration bit page\r\n");
        rval = true;
      }
    }
    return(rval);
  }

  System::Void GetVersion (System::IO::Ports::SerialPort^ Port, System::Windows::Forms::TextBox^  myDebugOut ) {
    array<Byte>^ Buffer = gcnew array<Byte>(BUFFER_SIZE);
    int major_version;

    myDebugOut->AppendText("Reading Firmware version...\r\n");
    Buffer[0] = COMMAND_READ_VERSION; //Get firmware version

    if (ComWrite(Port,Buffer,0,1)==false) return;  //WriteCommBlock(pComDev, Buffer, 1);

    if (ComRead(Port,Buffer,0,1)==false) return;
    if (Buffer[0] == COMMAND_NACK) {
      //old firmware version
      m_FirmwareVersion = 0;
      myDebugOut->AppendText("Firmware version 0: Config bits always written for PIC24H, but not for PIC24F/PIC24E/dsPIC33E (last page of program memory skipped for these devices). Update to latest firmware to change behavior.\r\n");
      if (CurrentDevice->fname == eFamily::PIC24H ||CurrentDevice->fname == eFamily::dsPIC33F ||CurrentDevice->fname == eFamily::PIC24FK)  m_ConfigBitEnable = true;
      else  m_ConfigBitEnable = false;
      return;
    }
    major_version = Buffer[0];
    m_FirmwareVersion = major_version;
    if (ComRead(Port,Buffer,0,2)==false) return;
    myDebugOut->AppendText("Firmware version: ");
    myDebugOut->AppendText(Convert::ToString((int) major_version,10));
    myDebugOut->AppendText(".");
    myDebugOut->AppendText(Convert::ToString((int) Buffer[0],10));
    if (m_ConfigBitEnable) {
      myDebugOut->AppendText(", Config bits programming ENABLED.");
    } else
      myDebugOut->AppendText(", Config bits programming is DISABLED (last page of program memory skipped on PIC24F).");
    myDebugOut->AppendText(" \r\n");

    if (m_FirmwareVersion >= 3) {
      myDebugOut->AppendText("Firmware version 3.0 or later detected.\r\n");
      myDebugOut->AppendText(String::Concat("No pages below location: ", Convert::ToString((int) PROGRAM_START,16), " will be written.\r\n"));
    }

  }

  System::Void ProgramHexFile (String^ path,System::IO::Ports::SerialPort^ Port,
                               System::Windows::Forms::TextBox^  myDebugOut, bool ConfigBitsEnabled ) {

    eFamily Family;
    array<Byte>^ Buffer = gcnew array<Byte>(BUFFER_SIZE);
    Int32  ExtAddr = 0;
    array<mem_cMemRow^>^ ppMemory = gcnew array<mem_cMemRow^>(PM_SIZE+EE_SIZE+CM_SIZE);
    array<mem_cMemRow^>^ ppMemoryVerify = gcnew array<mem_cMemRow^>(PM_SIZE+EE_SIZE+CM_SIZE);
    UInt32 pm33f_rowsize;

    m_ConfigBitEnable = ConfigBitsEnabled;

    GetVersion(Port, myDebugOut);

    debugOutLocal = myDebugOut;

    Family = CurrentDevice->fname;
    if (CurrentDevice->fname == eFamily::PIC24FK) pm33f_rowsize = PIC24FK_ROW_SIZE;
    else if (CurrentDevice->SmallRAM == true) pm33f_rowsize = PM33F_ROW_SIZE_SMALL;
    else pm33f_rowsize = PM33F_ROW_SIZE_LARGE;

    for (Int32 Row = 0; Row < PM_SIZE; Row++) {
      ppMemory[Row] = gcnew mem_cMemRow(eType::Program, 0x000000, Row, Family, pm33f_rowsize);
      ppMemoryVerify[Row] = gcnew mem_cMemRow(eType::Program, 0x000000, Row, Family, pm33f_rowsize);
    }

    for (Int32 Row = 0; Row < EE_SIZE; Row++) {
      ppMemory[Row + PM_SIZE] = gcnew mem_cMemRow(eType::EEProm, 0x7FF000, Row, Family, pm33f_rowsize);
      ppMemoryVerify[Row + PM_SIZE] = gcnew mem_cMemRow(eType::EEProm, 0x7FF000, Row, Family, pm33f_rowsize);
    }
    for (int Row = 0; Row < CM_SIZE; Row++) {
      ppMemory[Row + PM_SIZE + EE_SIZE] = gcnew mem_cMemRow(eType::Configuration, 0xF80000, Row, Family, pm33f_rowsize);
      ppMemoryVerify[Row + PM_SIZE + EE_SIZE] = gcnew mem_cMemRow(eType::Configuration, 0xF80000, Row, Family, pm33f_rowsize);
    }

    for (Int32 Row = 0; Row < (PM_SIZE); Row++) {
      for (Int32 index = 0; index < (3*ppMemory[Row]->GetRowSize()); index++) {
        ppMemoryVerify[Row]->SetRowByte(index, 255);
        ppMemory[Row]->SetRowByte(index, 255);
      }
    }

    myDebugOut->AppendText("Reading HexFile...\r\n");

    //FileStream^ fs = File::Open( path, FileMode::Open, FileAccess::Read, FileShare::None );
    StreamReader^ sr;
    try {
      sr = gcnew StreamReader( path );
    } catch ( Exception^ e ) {
      myDebugOut->AppendText(String::Concat("Exception while opening hex file: ",e->Message,"\r\n"));
      return;
    }
    try {
      //Read the Hex File
      String^ hexLine;
      Int32 linecnt = 0;

      System::Text::ASCIIEncoding^ encoder = gcnew System::Text::ASCIIEncoding();

      //while ( fs->Read( Buffer, 0, Buffer->Length ) > 0 ){
      while ( (hexLine = sr->ReadLine()) != nullptr ) {
        Byte ByteCount;
        UInt32 Address;
        Byte RecordType;
        UInt32 base;
        array<Byte>^ inBuf = encoder->GetBytes(hexLine);

        base = 1;


        //sscanf(Buffer+1, "%2x%4x%2x", &ByteCount, &Address, &RecordType);
        ByteCount = Hex2ToByte(inBuf,base);
        base +=2;
        Address = Hex4ToUInt16(inBuf,base);
        base +=4;
        RecordType = Hex2ToByte(inBuf,base);
        base+=2;
        //myDebugOut->AppendText(String::Concat(Convert::ToString(linecnt,16),")Cnt: ",ByteCount.ToString()," Addr: ",Convert::ToString(Address,16), " Rec: ",Convert::ToString(RecordType,16),"\r\n"));
        linecnt++;
        if (RecordType == 0) {
          Address = (Address + ExtAddr) / 2;
          if (CheckAddressClash(Address, Family) == false) {
            myDebugOut->AppendText("Program address in Hex file clashes with Bootloader location, aborting. Recompile Hex file with appropriate linker file.\r\n");
            sr->Close();
            return;
          }
          for (Int32 CharCount = 0; CharCount < ByteCount*2; CharCount += 4, Address++) {
            Boolean bInserted = false;

            for (Int32 Row = 0; Row < (PM_SIZE + EE_SIZE + CM_SIZE); Row++) {
              if (CheckAddressClash2(Address, inBuf, base + CharCount,Family) == false) {
                myDebugOut->AppendText("Program data in Hex file clashes with Bootloader program, aborting. Recompile Hex file with appropriate linker file.\r\n");
                sr->Close();
                return;
              }
              if (CheckAddressClash3(Address, inBuf, base + CharCount,Family,CurrentDevice->ConfigPage,CurrentDevice->ConfigWord ) == false) {
                myDebugOut->AppendText("Configuration bit programming not enabled, but data exists on last page of flash! Enable config bit programming or change hex file.\r\n");
                myDebugOut->AppendText("Program data is at location: ");
                myDebugOut->AppendText(Convert::ToString((int) Address,16));
                myDebugOut->AppendText("\r\n");
                sr->Close();
                return;
              }

              if ((bInserted = ppMemory[Row]->InsertData(Address, inBuf, base + CharCount)) == true) {
                ppMemoryVerify[Row]->InsertData(Address, inBuf, base + CharCount);
                break;
              }
            }

            if (bInserted != true) {
              //printf("Bad Hex file: 0x%xAddress out of range\n", Address);
              int i;
              i = Address;
              myDebugOut->AppendText(String::Concat("Bad Hex file: 0x",Convert::ToString(i,16)," address out of range\r\n"));
              sr->Close();
              return;
            }
          }
        } else if (RecordType == 1) {
        } else if (RecordType == 4) {
          //sscanf(Buffer+9, "%4x", &ExtAddr);
          UInt16  k;
          k = Hex4ToUInt16(inBuf,base);
          base +=4;
          ExtAddr = k << 16;
        } else {
          //ErrorExit("Unknown hex record type\n");
          myDebugOut->AppendText(String::Concat("Unknown hex record type: ", Convert::ToString(RecordType,10), "\r\n"));
          sr->Close();
          return;
        }
        //advance to next record

      }//end while
    }//end try
    catch ( Exception^ e ) {
      myDebugOut->AppendText(String::Concat("Exception while reading Hex: ",e->Message,"\r\n"));
      sr->Close();
      return;
    }

    myDebugOut->AppendText("HexFile successfully read.\r\n");
    sr->Close();
    //check for application file/firmware version clash
    if(this->m_FirmwareVersion < 3) {
      mem_cMemRow^ rowzero = ppMemory[0];
      if (rowzero->m_Data[4] == 524 && rowzero->m_Data[5] == 0) {
        myDebugOut->AppendText("Application hex file is linked incorrectly for Bootloader firmware V2.0 and earlier. Either update the bootloader firmware, or relink application using .gld files from 'lkr_v2' folder. Aborting!\r\n");
        return;
      }

    }

    if (this->m_FirmwareVersion < 3) {

      /* Preserve first two locations for bootloader */
      {
        array<Byte>^ Data = gcnew array<Byte>(32);
        Int32         RowSize;
        UInt16 base;

        if (Family == eFamily::dsPIC30F) {
          RowSize = PM30F_ROW_SIZE;
        } else {
          if (CurrentDevice->fname == eFamily::PIC24FK) RowSize = PIC24FK_ROW_SIZE;
          else if (CurrentDevice->SmallRAM == true) RowSize = PM33F_ROW_SIZE_SMALL;
          else RowSize = PM33F_ROW_SIZE_LARGE;
        }

        Buffer[0] = COMMAND_READ_PM;
        Buffer[1] = 0x00;
        Buffer[2] = 0x00;
        Buffer[3] = 0x00;

        if (ComWrite(Port,Buffer,0,4)==false) return;   //WriteCommBlock(pComDev, Buffer, 4);

        myDebugOut->AppendText("\r\nReading Target\r\n");
        if (ComRead(Port, Buffer, 0, RowSize*3)==false) return;   //ReceiveData(pComDev, Buffer, RowSize * 3);

#if 0
        sprintf(Data, "%02x%02x%02x00%02x%02x%02x00",   Buffer[2] & 0xFF,
                Buffer[1] & 0xFF,
                Buffer[0] & 0xFF,
                Buffer[5] & 0xFF,
                Buffer[4] & 0xFF,
                Buffer[3] & 0xFF);
#endif
        base = 0;
        ToAsciiHex(Buffer[2],Data,base);
        base+=2;
        ToAsciiHex(Buffer[1],Data,base);
        base+=2;
        ToAsciiHex(Buffer[0],Data,base);
        base+=2;
        ToAsciiHex(0,Data,base);
        base+=2;
        ToAsciiHex(Buffer[5],Data,base);
        base+=2;
        ToAsciiHex(Buffer[4],Data,base);
        base+=2;
        ToAsciiHex(Buffer[3],Data,base);
        base+=2;
        ToAsciiHex(0,Data,base);
        base+=2;

        ppMemory[0]->InsertData(0x000000, Data, 0);
        ppMemory[0]->InsertData(0x000001, Data, 4);
        ppMemory[0]->InsertData(0x000002, Data, 8);
        ppMemory[0]->InsertData(0x000003, Data, 12);

        ppMemoryVerify[0]->InsertData(0x000000, Data, 0);
        ppMemoryVerify[0]->InsertData(0x000001, Data, 4);
        ppMemoryVerify[0]->InsertData(0x000002, Data, 8);
        ppMemoryVerify[0]->InsertData(0x000003, Data, 12);
      }
    }

    //format memory data before programming
    for (Int32 Row = 0; Row < (PM_SIZE + EE_SIZE + CM_SIZE); Row++) {
      ppMemory[Row]->FormatData();
      ppMemoryVerify[Row]->FormatData();
    }

    myDebugOut->AppendText("Programming Device \r\n");
    for (Int32 Row = 0; Row < (PM_SIZE + EE_SIZE + CM_SIZE); Row++) {
      if (ppMemory[Row]->GetRowType() == eType::Configuration && m_ConfigBitEnable == false) continue;
      if (SkipRowPM(ppMemory[Row], Family, myDebugOut) == false)
        ppMemory[Row]->SendData(Port, myDebugOut);
      if (ppMemory[Row]->GetRowType() == eType::Configuration &&
          ppMemory[Row]->GetRowNumber() == 0 &&
          Family == eFamily::PIC24H )
        myDebugOut->AppendText("Config Bits sent.\r\n");
    }

    myDebugOut->AppendText("Verifying... \r\n");
    Boolean VerifyOk;
    VerifyOk = true;
    //just verify program memory
    Int32 address;
    address = 0;
    for (Int32 Row = 0; Row < (PM_SIZE); Row++) {
      if (SkipRowPM(ppMemory[Row], Family, myDebugOut) == true) continue;
      if (ppMemory[Row]->ReadData(Port, myDebugOut) == true) {
        address = ppMemory[Row]->GetRowAddress();
        for (Int32 index = 0; index < ppMemory[Row]->GetRowSize(); index++) {
          Int32 instrExpected, instrGot;
          instrExpected = ((ppMemoryVerify[Row]->GetRowByte(3*index + 2)) << 16) +
                          ((ppMemoryVerify[Row]->GetRowByte(3*index + 1))  << 8) +
                          ppMemoryVerify[Row]->GetRowByte(3*index + 0);
          instrGot = ((ppMemory[Row]->GetRowByte(3*index + 0)) << 16) +
                     ((ppMemory[Row]->GetRowByte(3*index + 1))  << 8) +
                     ppMemory[Row]->GetRowByte(3*index + 2);

          if (instrExpected != instrGot) {
            VerifyOk = false;
            myDebugOut->AppendText("Verfication failed @address: ");
            myDebugOut->AppendText(Convert::ToString(address,16));
            myDebugOut->AppendText(", expected: ");
            myDebugOut->AppendText(Convert::ToString(instrExpected,16));
            myDebugOut->AppendText("  , got: ");
            myDebugOut->AppendText(Convert::ToString(instrGot,16));
            myDebugOut->AppendText("\r\n");
            break;
          }
          address = address + 2;
        }
      } else {
        myDebugOut->AppendText("Problem reading program memory during verification.\r\n");
      }
      if (VerifyOk == false) break;
    }

    if (VerifyOk == false)
      myDebugOut->AppendText("Verification failed, programmed program memory does not match hex file contents!\r\n");

    //because of the way that the firmware is written, we need to resend the configuration bytes
    //before a device reset if programming the config bits
    if (m_ConfigBitEnable == true) {
      for (Int32 Row = 0; Row < (PM_SIZE + EE_SIZE + CM_SIZE); Row++) {
        if (ppMemory[Row]->GetRowType() == eType::Configuration)
          ppMemory[Row]->SendData(Port, myDebugOut);
      }
    }

    if (m_FirmwareVersion == 0 || m_ConfigBitEnable == true)  Buffer[0] = COMMAND_RESET; //Reset target device
    else Buffer[0] = COMMAND_POR_RESET;

    if (ComWrite(Port,Buffer,0,1)==false) return;  //WriteCommBlock(pComDev, Buffer, 1);

    myDebugOut->AppendText("Done.\r\n");

  }

  System::Void ParseDeviceLine (String^ devline,System::Windows::Forms::TextBox^  myDebugOut) {

    array<String^>^ parts;
    String^ devName;
    String^ famName;
    eFamily Family;
    Int32 DevID;
    Int32 ConfigPage;
    Int32 PID;
    Int32 tmp;
    Boolean SmallRAM;


    parts = devline->Split(',');
    if (parts->Length != 6) {
      //myDebugOut->AppendText("Bad data");
      return;
    }

    //Determine family

    devName = parts[0];
    DevID = Int32::Parse(parts[1],System::Globalization::NumberStyles::HexNumber);
    PID = Int32::Parse(parts[2],System::Globalization::NumberStyles::HexNumber);
    famName = parts[3];
    ConfigPage = Int32::Parse(parts[4],System::Globalization::NumberStyles::HexNumber);
    tmp = Int32::Parse(parts[5],System::Globalization::NumberStyles::HexNumber);
    if (tmp != 0) SmallRAM = true;
    else SmallRAM = false;

    if (!String::Compare(famName,"dsPIC30F", true)) {
      Family = eFamily::dsPIC30F;
    } else if (!String::Compare(famName,"dsPIC33F", true)) {
      Family = eFamily::dsPIC33F;
    } else if (!String::Compare(famName,"PIC24H", true)) {
      Family = eFamily::PIC24H;
    } else if (!String::Compare(famName,"PIC24F", true)) {
      Family = eFamily::PIC24F;
    } else if (!String::Compare(famName,"PIC24FK", true)) {
      Family = eFamily::PIC24FK;
    } else if (!String::Compare(famName,"PIC24E", true)) {
      Family = eFamily::PIC24E;
    } else if (!String::Compare(famName,"dsPIC33E", true)) {
      Family = eFamily::dsPIC33E;
    } else {
      myDebugOut->AppendText(String::Concat("Problem parsing device file, unrecognized family: ",famName,"\r\n"));
      return;
    }
    //myDebugOut->AppendText(String::Concat(devName,",",DevID.ToString(),",",PID.ToString(),"\r\n"));
    AddDevice(devName, DevID, PID, Family,ConfigPage,SmallRAM);
  }

private:
  System::Collections::ArrayList^ devices;

  Boolean DeviceFound;
  device^  CurrentDevice;


  System::Void AddDevice(String^ a_name, Int32 a_Id, Int32 a_PID, eFamily a_fname, Int32 a_num, Boolean a_flag) {
    device^ a_device;

    a_device = gcnew device(a_name, a_Id, a_PID, a_fname, a_num, a_flag);

    devices->Add(a_device);

  }






};

}

