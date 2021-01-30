#pragma once

namespace winbootldr {
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::IO;



public ref class mem_cMemRow : public common {
public:
  array<UInt16>^   m_Data;

private:
  array<Byte>^     m_pBuffer;
  UInt32           m_Address;
  Boolean          m_bEmpty;
  eType            m_eType;
  Int32            m_RowNumber;
  eFamily          m_eFamily;
  Int32            m_RowSize;





public:
  mem_cMemRow(eType Type, UInt32 StartAddr, Int32 RowNumber, eFamily Family, UInt32 pm33f_rowsize) {
    Int32 Size;
    UInt32 i;

    m_RowNumber = RowNumber;
    m_eFamily    = Family;
    m_eType     = Type;
    m_bEmpty    = true;
    m_Data = gcnew array<UInt16>(pm33f_rowsize*2);

    if (m_eType == eType::Program) {
      if (m_eFamily == eFamily::dsPIC30F) {
        m_RowSize = PM30F_ROW_SIZE;
      } else {
        m_RowSize = pm33f_rowsize;
      }
    } else {
      m_RowSize = EE30F_ROW_SIZE;
    }

    if (m_eType == eType::Program) {
      Size = m_RowSize * 3;
      m_Address = StartAddr + RowNumber * m_RowSize * 2;
    }
    if (m_eType == eType::EEProm) {
      Size = m_RowSize * 2;
      m_Address = StartAddr + RowNumber * m_RowSize * 2;
    }
    if (m_eType == eType::Configuration) {
      Size = 3;
      m_Address = StartAddr + RowNumber * 2;
    }

    m_pBuffer   = gcnew array<Byte>(Size);
    for (i=0; i<pm33f_rowsize*2; i++) {
      m_Data[i] = 0xFFFF;

    }
    //memset(m_Data, 0xFFFF, sizeof(unsigned short)*PM33F_ROW_SIZE*2);

  }

public:
  Byte mem_cMemRow::GetRowByte(Int32 index) {
    return(m_pBuffer[index]);
  }

public:
  void mem_cMemRow::SetRowByte(Int32 index, Byte b) {
    m_pBuffer[index] = b;
  }

public:
  Int32 mem_cMemRow::GetRowSize(void) {
    return(m_RowSize);
  }

public:
  eType mem_cMemRow::GetRowType(void) {
    return(m_eType);
  }

public:
  UInt32 mem_cMemRow::GetRowAddress(void) {
    return(m_Address);
  }
public:
  Boolean mem_cMemRow::GetRowEmpty(void) {
    return(m_bEmpty);
  }

public:
  Int32 mem_cMemRow::GetRowNumber(void) {
    return(m_RowNumber);
  }



public:
  Boolean mem_cMemRow::InsertData(UInt32 Address, array<Byte>^ pData, Int32 base) {

    if (Address < m_Address) {
      return false;
    }

    if ((m_eType == eType::Program) && (Address >= (m_Address + m_RowSize * 2))) {
      return false;
    }

    if ((m_eType == eType::EEProm) && (Address >= (m_Address + m_RowSize * 2))) {
      return false;
    }

    if ((m_eType == eType::Configuration) && (Address >= (m_Address + 2))) {
      return false;
    }

    m_bEmpty    = false;

    //sscanf(pData, "%4hx", &(m_Data[Address - m_Address]));
    m_Data[Address - m_Address] =  Hex4ToUInt16(pData, base);
    return true;
  }

public:
  System::Void mem_cMemRow::FormatData(void) {
    if (m_bEmpty == true) {
      return;
    }

    if (m_eType == eType::Program) {
      for (int Count = 0; Count < m_RowSize; Count += 1) {
        m_pBuffer[0 + Count * 3] = (m_Data[Count * 2]     >> 8) & 0xFF;
        m_pBuffer[1 + Count * 3] = (m_Data[Count * 2])          & 0xFF;
        m_pBuffer[2 + Count * 3] = (m_Data[Count * 2 + 1] >> 8) & 0xFF;
      }
    } else if (m_eType == eType::Configuration) {
      m_pBuffer[0] = (m_Data[0]  >> 8) & 0xFF;
      m_pBuffer[1] = (m_Data[0])       & 0xFF;
      m_pBuffer[2] = (m_Data[1]  >> 8) & 0xFF;
    } else {
      for (int Count = 0; Count < m_RowSize; Count++) {
        m_pBuffer[0 + Count * 2] = (m_Data[Count * 2] >> 8) & 0xFF;
        m_pBuffer[1 + Count * 2] = (m_Data[Count * 2])      & 0xFF;
      }
    }
  }


  /*
     The COM TIMER must be disabled before calling this function!
     Do not want the serial port being read by another function
  */

public:
  System::Void mem_cMemRow::SendData(System::IO::Ports::SerialPort^ Port,
                                     System::Windows::Forms::TextBox^  myDebugOut) {
    array<Byte>^ Buffer = {0,0,0,0};
    //disable timer while doing this because we do not want the timer
    //routine reading the serial port
    debugOutLocal = myDebugOut;

    if ((m_bEmpty == true) && (m_eType != eType::Configuration)) {
      return;
    }
    //PIC24F configuration bits are in program memory.
    if ((m_eType == eType::Configuration) && (m_eFamily == eFamily::PIC24F)) return;

    while (Buffer[0] != COMMAND_ACK) {
      if (m_eType == eType::Program) {
        Buffer[0] = COMMAND_WRITE_PM;
        Buffer[1] = (m_Address)       & 0xFF;
        Buffer[2] = (m_Address >> 8)  & 0xFF;
        Buffer[3] = (m_Address >> 16) & 0xFF;

        if (ComWrite(Port,Buffer,0,4)==false) return;  //WriteCommBlock(Port, Buffer, 4);
        if (ComWrite(Port,m_pBuffer, 0, m_RowSize * 3) ==false) return;    //WriteCommBlock(Port, m_pBuffer, m_RowSize * 3);
        //printf("Mem Address: %x\n",m_Address);
        int i;
        i = m_Address;
        myDebugOut->AppendText("Mem Address: 0x");
        myDebugOut->AppendText(Convert::ToString(i,16));
        myDebugOut->AppendText("\r\n");

      } else if (m_eType == eType::EEProm) {
        Buffer[0] = COMMAND_WRITE_EE;
        Buffer[1] = (m_Address)       & 0xFF;
        Buffer[2] = (m_Address >> 8)  & 0xFF;
        Buffer[3] = (m_Address >> 16) & 0xFF;

        if (ComWrite(Port,Buffer,0,4) ==false) return;  //WriteCommBlock(Port, Buffer, 4);
        if (ComWrite(Port,m_pBuffer, 0, m_RowSize * 2)==false) return; //WriteCommBlock(Port, m_pBuffer, m_RowSize * 2);
      } else if ((m_eType == eType::Configuration) && (m_RowNumber == 0) ) {
        Buffer[0] = COMMAND_WRITE_CM;
        if (m_bEmpty) Buffer[1] = 1; //Buffer[1] = (char)(m_bEmpty)& 0xFF;
        else Buffer[1] = 0;
        Buffer[2] = m_pBuffer[0];
        Buffer[3] = m_pBuffer[1];

        if (ComWrite(Port,Buffer,0,4) ==false) return;    //WriteCommBlock(Port, Buffer, 4);

      } else if ((m_eType == eType::Configuration) && (m_RowNumber != 0)) {
        if ((m_eFamily == eFamily::dsPIC30F) && (m_RowNumber == 7)) {
          return;
        }
        if (m_bEmpty)Buffer[0] = 1;   //Buffer[0] = (char)(m_bEmpty)& 0xFF;
        else Buffer[0] = 0;
        Buffer[1] = m_pBuffer[0];
        Buffer[2] = m_pBuffer[1];

        if (ComWrite(Port,Buffer,0,3)==false) return;  //WriteCommBlock(Port, Buffer, 3);

      }

      else {
        myDebugOut->AppendText("Error: Unknown memory type in mem_cMemRow::SendData\r\n");
        return;
      }
      //ReceiveData(Port, Buffer, 1);
      if (ComRead(Port,Buffer,0,1)==false) return;
    } //end while
  }

  //Read a row for verification purposes
public:
  Boolean  mem_cMemRow::ReadData(System::IO::Ports::SerialPort^ Port,
                                 System::Windows::Forms::TextBox^  myDebugOut) {
    array<Byte>^ Buffer = {0,0,0,0};
    //disable timer while doing this because we do not want the timer
    //routine reading the serial port
    debugOutLocal = myDebugOut;

    if ((m_bEmpty == true) && (m_eType != eType::Configuration)) {
      return true;
    }

    if (m_eType == eType::Program) {
      Buffer[0] = COMMAND_READ_PM;
      Buffer[1] = (m_Address)       & 0xFF;
      Buffer[2] = (m_Address >> 8)  & 0xFF;
      Buffer[3] = (m_Address >> 16) & 0xFF;

      if (ComWrite(Port,Buffer,0,4)==false) return false;  //WriteCommBlock(Port, Buffer, 4);
      if (ComRead(Port,m_pBuffer, 0, m_RowSize * 3) ==false) return false;    //ReceiveData(pComDev, Buffer, RowSize * 3);
      //printf("Mem Address: %x\n",m_Address);
      int i;
      i = m_Address;
      myDebugOut->AppendText("Mem Address: 0x");
      myDebugOut->AppendText(Convert::ToString(i,16));
      myDebugOut->AppendText("\r\n");
      return true;
    }
    return false;
  }



};



}