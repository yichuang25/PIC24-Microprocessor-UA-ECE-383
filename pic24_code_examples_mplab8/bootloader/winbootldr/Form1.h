#pragma once

/*
Adopted from C code orginally provided by Microchip.
This version written by R. Reese (reese@ece.msstate.edu)
Mississippi State University

*/

#include "dataXfer.h"
#include <stdio.h>
#include <vcclr.h>

namespace winbootldr {

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::IO;
using namespace System::Runtime::InteropServices;


enum class States {
  NULL_STATE,
  RESET_LOW,
  RESET_HIGH,
  PROGRAM
};


/// <summary>
/// Summary for Form1
///
/// WARNING: If you change the name of this class, you will need to change the
///          'Resource File Name' property for the managed resource compiler tool
///          associated with all .resx files this class depends on.  Otherwise,
///          the designers will not be able to interact properly with localized
///          resources associated with this form.
/// </summary>
public ref class Form1 : public System::Windows::Forms::Form {

private:
  Int16 status_tics;
  Int32        MaxConsoleChars;
  Boolean status_clear;
  Boolean comOpen;
  String^ hexFile;
  String^ textFile;
  bootloader^ loader;
  String^ noPort;
  Int16 general_ticks;
  States mystates;
  Boolean blAndExit;
  StreamWriter^ LogFileStream;

private:
  System::Windows::Forms::StatusStrip^  statusStripBottom;
  System::Windows::Forms::ToolStripStatusLabel^  statusStripLabel;
  System::Windows::Forms::TextBox^  sendBox;
  System::Windows::Forms::TextBox^  debugOut;
  System::Windows::Forms::ComboBox^  comBox;
  System::Windows::Forms::ComboBox^  baudBox;
  System::Windows::Forms::CheckBox^  checkBoxComOpen;
  System::Windows::Forms::MenuStrip^  topMenuStrip;
  System::Windows::Forms::ToolStripMenuItem^  toolStripMainMenu;
  System::Windows::Forms::ToolStripMenuItem^  browseForHexFileToolStripMenuItem;
  System::Windows::Forms::ToolStripMenuItem^  exitToolStripMenuItem;
  System::Windows::Forms::OpenFileDialog^  openHexFileDialog;
  System::Windows::Forms::TextBox^  deviceTextBox;
  System::IO::Ports::SerialPort^  serialPort1;
  System::Windows::Forms::TextBox^  mainOutput;
  System::Windows::Forms::Button^  sendButton;
  System::Windows::Forms::Timer^  comTimer;
  System::ComponentModel::IContainer^  components;
  System::Windows::Forms::TextBox^  hexFileNameBox;
  System::Windows::Forms::Button^  hexFileButton;
  System::Windows::Forms::Button^  programHexButton;
  System::Windows::Forms::ToolStripMenuItem^  helpToolStripMenuItem;
  System::Windows::Forms::ToolStripMenuItem^  aboutToolStripMenuItem;
  System::Windows::Forms::Button^  sendCRbutton;
  System::Windows::Forms::ToolStripMenuItem^  sendTextFileToolStripMenuItem;
  System::Windows::Forms::ToolStripMenuItem^  loadDeviceFileToolStripMenuItem;
  System::Windows::Forms::OpenFileDialog^  openDevFileDialog;
  System::Windows::Forms::CheckBox^  checkBoxRTS;
  System::Windows::Forms::Button^  buttonProgramWRTS;
  System::Windows::Forms::Timer^  timer1;

  System::Windows::Forms::CheckBox^  checkBoxLogging;
  System::Windows::Forms::CheckBox^  pgmConfigBox1;
  System::Windows::Forms::ListView^  listViewDataXfer;
  System::Windows::Forms::ColumnHeader^  columnHeaderIndex;
  System::Windows::Forms::ColumnHeader^  columnHeaderName;
  System::Windows::Forms::ColumnHeader^  columnHeaderValue;
  System::Windows::Forms::ColumnHeader^  columnHeaderDesc;
  System::Windows::Forms::ColumnHeader^  columnHeaderSize;
  System::Windows::Forms::ColumnHeader^  columnHeaderFormat;


public:
  Form1(Boolean blAndExit) {
    InitializeComponent();
    // Initialize data transfer list view:
    // 1. Create and initialize each item
    cli::array<System::Windows::Forms::ListViewItem^>^ alvi =
      gcnew cli::array<System::Windows::Forms::ListViewItem^>(MAX_NUM_XFER_VARS);
    for (int i = 0; i < MAX_NUM_XFER_VARS; i++) {
      // Create a listview item with 6 fields
      alvi[i] = gcnew System::Windows::Forms::ListViewItem(
      gcnew cli::array<System::String^>(6) {
        L"", L"", L"", i.ToString(), L"", L""
      });
    }
    // 2. Add them to the list view
    listViewDataXfer->Items->AddRange(alvi);

    // Use an encoding that will pass binary data through without modification.
    // I hope.
    serialPort1->Encoding = Encoding::GetEncoding(1252);

    //
    //TODO: Add the constructor code here
    //
    initDataXfer();
    general_ticks = 0;
    mystates = States::NULL_STATE;
    noPort = String::Concat("NoPorts","");
    comOpen = false;
    status_clear = false;
    status_tics = 0;
    this->timer1->Enabled = true;
    this->timer1->Interval = 100;
    this->comTimer->Interval = 50;
    this->comTimer->Enabled = true;
    writeToDebugOutput("Started...");
    showStatus("");
    getPortNames();
    baudBox->SelectedIndex = 0;
    loader = gcnew bootloader;
    loader->debugOutLocal = debugOut;

    initSettings();
    loadDeviceFile();

    this->blAndExit = blAndExit;
    if (blAndExit) {
      // Continue only if the COM port opens
      checkBoxComOpen->Checked = true;
      if (checkBoxComOpen->Checked) {
        // MCLR and program, using the state machine
        mystates = States::RESET_LOW;
        // When the program finishes, the state machine
        // will see blAndExit true, close the comm port,
        // and exit.
      }
    }
  }

private:
  String^ getSettingsPath(void) {
    String^ appFolder;
    String^ settingsFolder;

    appFolder = System::Environment::GetFolderPath (System::Environment::SpecialFolder::LocalApplicationData);
    settingsFolder = System::IO::Path::Combine(appFolder, "BullyBootloader");
    if (!System::IO::Directory::Exists(settingsFolder)) {
      System::IO::Directory::CreateDirectory(settingsFolder);
    }
    return(System::IO::Path::Combine(settingsFolder,"bootloaderProps.txt"));
  }

private:
  System::Void initSettings(void) {
    String^ settingsFile;
    StreamReader^ sr;
    array<String^>^ parts;

    settingsFile = getSettingsPath();

    if (!System::IO::File::Exists(settingsFile)) {
      return;
    }
    writeToDebugOutput(String::Concat("Reading settings from: ",  settingsFile,  "\r\n"));
    try {
      sr = gcnew StreamReader( settingsFile );
    } catch ( Exception^ e ) {
      writeToDebugOutput(String::Concat("Exception while reading settings file: ",e->Message,"\r\n"));
      return;
    }

    try {
      String^ textLine;
      int index;
      while ( (textLine = sr->ReadLine()) != nullptr ) {
        parts = textLine->Split(',');
        if (parts->Length != 2) continue;
        if (!String::Compare(parts[0],"COM")) {
          index = comBox->FindString(parts[1]);
          comBox->SelectedIndex = index;
        }
        if (!String::Compare(parts[0],"BAUD")) {
          index = baudBox->FindString(parts[1]);
          baudBox->SelectedIndex = index;
        }
        if (!String::Compare(parts[0],"CONFIG")) {
          if (!String::Compare(parts[1],"TRUE"))pgmConfigBox1->Checked = true;
          else pgmConfigBox1->Checked = false;
        }
        if (!String::Compare(parts[0],"HEXFILE")) {
          hexFile = parts[1];
          hexFileNameBox->Text = parts[1];
        }
      }

    } catch ( Exception^ e ) {
      writeToDebugOutput(String::Concat("Exception while reading settings file: ",e->Message,"\r\n"));
      return;
    }
    sr->Close();
    writeToDebugOutput("Settings loaded\r\n");
  }

private:
  System::Void writeSettings(void) {
    String^ settingsFile;

    settingsFile = getSettingsPath();
    writeToDebugOutput(String::Concat("Settings File",  settingsFile,  "\r\n"));
    StreamWriter^ sw;
    try {
      sw = gcnew StreamWriter( settingsFile );
    } catch ( Exception^ e ) {
      writeToDebugOutput(String::Concat("Exception while creating settings file: ",e->Message,"\r\n"));
      return;
    }
    sw->Write(String::Concat("COM,",(String ^)comBox->SelectedItem));
    sw->WriteLine();
    sw->Write(String::Concat("BAUD,",(String ^)baudBox->SelectedItem));
    sw->WriteLine();
    if (pgmConfigBox1->Checked)  sw->Write("CONFIG,TRUE");
    else sw->Write("CONFIG,FALSE");
    sw->WriteLine();
    if (hexFile != nullptr)
      sw->Write(String::Concat("HEXFILE,",hexFile));
    sw->WriteLine();

    sw->Close();
  }

private:
  System::Boolean openComPort(void) {
    this->serialPort1->BaudRate =  Convert::ToInt32(baudBox->SelectedItem);
    // If no port is selected (for example, a com port loaded
    // from the settings file no longer exists), give up.
    if ((Object ^)comBox->SelectedItem == nullptr) {
      writeToDebugOutput("No COM port selected\n");
      return false;
    }
    this->serialPort1->PortName= (String ^)comBox->SelectedItem;
    try {
      this->serialPort1->Open();
      showStatus("Com Opened");
      serialPort1->ReadTimeout = 2000;  //2 second timeout for read
      comOpen = true;
      this->comTimer->Enabled = true;   //ensure the timer is on
      return true;
    } catch ( Exception^ e ) {
      writeToDebugOutput(String::Concat("Exception during serial port open: ",e->Message,"\r\n"));
    }
    return false;
  }

private:
  System::Boolean closeComPort(void) {
    try {
      this->serialPort1->Close();
      comOpen = false;
      showStatus("Com Closed");
      return true;
    } catch ( Exception^ e ) {
      writeToDebugOutput(String::Concat("Exception during serial port close: ",e->Message,"\r\n"));
    }
    return false;
  }


private:
  System::Void getPortNames(void) {
    int i;

    array<String ^> ^ names;
    names = System::IO::Ports::SerialPort::GetPortNames();
    if (names->Length == 0) {
      comBox->Items->Add(noPort);
    } else {
      for (i=0; i< names->Length; i++) {
        //writeToDebugOutput((String ^)names->GetValue(i));
        comBox->Items->Add((String ^)names->GetValue(i));
      }
    }
    comBox->SelectedIndex = 0;
    comBox->Refresh();
  }

protected:
  /// <summary>
  /// Clean up any resources being used.
  /// </summary>
  ~Form1() {
    writeSettings();
    if (components) {
      delete components;
    }
  }

private:
  /// <summary>
  /// Required designer variable.
  /// </summary>

  #pragma region Windows Form Designer generated code
  /// <summary>
  /// Required method for Designer support - do not modify
  /// the contents of this method with the code editor.
  /// </summary>
  void InitializeComponent(void) {
    this->components = (gcnew System::ComponentModel::Container());
    this->serialPort1 = (gcnew System::IO::Ports::SerialPort(this->components));
    this->mainOutput = (gcnew System::Windows::Forms::TextBox());
    this->sendButton = (gcnew System::Windows::Forms::Button());
    this->comTimer = (gcnew System::Windows::Forms::Timer(this->components));
    this->statusStripBottom = (gcnew System::Windows::Forms::StatusStrip());
    this->statusStripLabel = (gcnew System::Windows::Forms::ToolStripStatusLabel());
    this->sendBox = (gcnew System::Windows::Forms::TextBox());
    this->debugOut = (gcnew System::Windows::Forms::TextBox());
    this->comBox = (gcnew System::Windows::Forms::ComboBox());
    this->baudBox = (gcnew System::Windows::Forms::ComboBox());
    this->checkBoxComOpen = (gcnew System::Windows::Forms::CheckBox());
    this->topMenuStrip = (gcnew System::Windows::Forms::MenuStrip());
    this->toolStripMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
    this->browseForHexFileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
    this->sendTextFileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
    this->loadDeviceFileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
    this->helpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
    this->aboutToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
    this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
    this->openHexFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
    this->deviceTextBox = (gcnew System::Windows::Forms::TextBox());
    this->hexFileNameBox = (gcnew System::Windows::Forms::TextBox());
    this->hexFileButton = (gcnew System::Windows::Forms::Button());
    this->programHexButton = (gcnew System::Windows::Forms::Button());
    this->sendCRbutton = (gcnew System::Windows::Forms::Button());
    this->openDevFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
    this->checkBoxRTS = (gcnew System::Windows::Forms::CheckBox());
    this->buttonProgramWRTS = (gcnew System::Windows::Forms::Button());
    this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
    this->checkBoxLogging = (gcnew System::Windows::Forms::CheckBox());
    this->pgmConfigBox1 = (gcnew System::Windows::Forms::CheckBox());
    this->listViewDataXfer = (gcnew System::Windows::Forms::ListView());
    this->columnHeaderValue = (gcnew System::Windows::Forms::ColumnHeader());
    this->columnHeaderName = (gcnew System::Windows::Forms::ColumnHeader());
    this->columnHeaderDesc = (gcnew System::Windows::Forms::ColumnHeader());
    this->columnHeaderIndex = (gcnew System::Windows::Forms::ColumnHeader());
    this->columnHeaderSize = (gcnew System::Windows::Forms::ColumnHeader());
    this->columnHeaderFormat = (gcnew System::Windows::Forms::ColumnHeader());
    this->statusStripBottom->SuspendLayout();
    this->topMenuStrip->SuspendLayout();
    this->SuspendLayout();
    //
    // serialPort1
    //
    this->serialPort1->BaudRate = 57600;
    this->serialPort1->PortName = L"COM4";
    //
    // mainOutput
    //
    this->mainOutput->AcceptsReturn = true;
    this->mainOutput->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
                               | System::Windows::Forms::AnchorStyles::Left)
                               | System::Windows::Forms::AnchorStyles::Right));
    this->mainOutput->Location = System::Drawing::Point(0, 56);
    this->mainOutput->Multiline = true;
    this->mainOutput->Name = L"mainOutput";
    this->mainOutput->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
    this->mainOutput->Size = System::Drawing::Size(547, 269);
    this->mainOutput->TabIndex = 0;
    this->mainOutput->TextChanged += gcnew System::EventHandler(this, &Form1::mainOutput_TextChanged);
    this->mainOutput->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::mainOutput_keyPressed);
    //
    // sendButton
    //
    this->sendButton->Location = System::Drawing::Point(0, 27);
    this->sendButton->Name = L"sendButton";
    this->sendButton->Size = System::Drawing::Size(41, 23);
    this->sendButton->TabIndex = 2;
    this->sendButton->Text = L"Send";
    this->sendButton->UseVisualStyleBackColor = true;
    this->sendButton->Click += gcnew System::EventHandler(this, &Form1::sendButton_Click);
    //
    // comTimer
    //
    this->comTimer->Tick += gcnew System::EventHandler(this, &Form1::comTimer_Tick);
    //
    // statusStripBottom
    //
    this->statusStripBottom->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {
      this->statusStripLabel
    });
    this->statusStripBottom->Location = System::Drawing::Point(0, 595);
    this->statusStripBottom->Name = L"statusStripBottom";
    this->statusStripBottom->Size = System::Drawing::Size(547, 22);
    this->statusStripBottom->TabIndex = 3;
    this->statusStripBottom->Text = L"statusStrip1";
    //
    // statusStripLabel
    //
    this->statusStripLabel->Name = L"statusStripLabel";
    this->statusStripLabel->Size = System::Drawing::Size(118, 17);
    this->statusStripLabel->Text = L"toolStripStatusLabel1";
    //
    // sendBox
    //
    this->sendBox->Location = System::Drawing::Point(47, 30);
    this->sendBox->Name = L"sendBox";
    this->sendBox->Size = System::Drawing::Size(106, 20);
    this->sendBox->TabIndex = 4;
    //
    // debugOut
    //
    this->debugOut->AcceptsReturn = true;
    this->debugOut->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
                             | System::Windows::Forms::AnchorStyles::Right));
    this->debugOut->Location = System::Drawing::Point(0, 417);
    this->debugOut->Multiline = true;
    this->debugOut->Name = L"debugOut";
    this->debugOut->ReadOnly = true;
    this->debugOut->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
    this->debugOut->Size = System::Drawing::Size(547, 59);
    this->debugOut->TabIndex = 5;
    //
    // comBox
    //
    this->comBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
    this->comBox->FormattingEnabled = true;
    this->comBox->Location = System::Drawing::Point(3, 329);
    this->comBox->Name = L"comBox";
    this->comBox->Size = System::Drawing::Size(64, 21);
    this->comBox->TabIndex = 6;
    //
    // baudBox
    //
    this->baudBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
    this->baudBox->FormattingEnabled = true;
    this->baudBox->Items->AddRange(gcnew cli::array< System::Object^  >(9) {
      L"2400", L"4800", L"9600", L"19200", L"28800", L"38400",
      L"57600", L"115200", L"230400"
    });
    this->baudBox->Location = System::Drawing::Point(73, 329);
    this->baudBox->Name = L"baudBox";
    this->baudBox->Size = System::Drawing::Size(80, 21);
    this->baudBox->TabIndex = 7;
    this->baudBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::baudBox_SelectedIndexChanged);
    //
    // checkBoxComOpen
    //
    this->checkBoxComOpen->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
    this->checkBoxComOpen->AutoSize = true;
    this->checkBoxComOpen->Location = System::Drawing::Point(159, 331);
    this->checkBoxComOpen->Name = L"checkBoxComOpen";
    this->checkBoxComOpen->Size = System::Drawing::Size(73, 17);
    this->checkBoxComOpen->TabIndex = 8;
    this->checkBoxComOpen->Text = L"OpenCom";
    this->checkBoxComOpen->UseVisualStyleBackColor = true;
    this->checkBoxComOpen->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBoxComOpen_CheckedChanged);
    //
    // topMenuStrip
    //
    this->topMenuStrip->AllowDrop = true;
    this->topMenuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {
      this->toolStripMainMenu
    });
    this->topMenuStrip->Location = System::Drawing::Point(0, 0);
    this->topMenuStrip->Name = L"topMenuStrip";
    this->topMenuStrip->Size = System::Drawing::Size(547, 24);
    this->topMenuStrip->TabIndex = 9;
    this->topMenuStrip->Text = L"menuStrip1";
    //
    // toolStripMainMenu
    //
    this->toolStripMainMenu->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {
      this->browseForHexFileToolStripMenuItem,
           this->sendTextFileToolStripMenuItem, this->loadDeviceFileToolStripMenuItem, this->helpToolStripMenuItem, this->aboutToolStripMenuItem,
           this->exitToolStripMenuItem
    });
    this->toolStripMainMenu->Name = L"toolStripMainMenu";
    this->toolStripMainMenu->Size = System::Drawing::Size(46, 20);
    this->toolStripMainMenu->Text = L"Main";
    //
    // browseForHexFileToolStripMenuItem
    //
    this->browseForHexFileToolStripMenuItem->Name = L"browseForHexFileToolStripMenuItem";
    this->browseForHexFileToolStripMenuItem->Size = System::Drawing::Size(159, 22);
    this->browseForHexFileToolStripMenuItem->Text = L"Hex File Browse";
    this->browseForHexFileToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::browseForHexFileToolStripMenuItem_Click);
    //
    // sendTextFileToolStripMenuItem
    //
    this->sendTextFileToolStripMenuItem->Name = L"sendTextFileToolStripMenuItem";
    this->sendTextFileToolStripMenuItem->Size = System::Drawing::Size(159, 22);
    this->sendTextFileToolStripMenuItem->Text = L"Send Text File";
    this->sendTextFileToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::sendTextFileToolStripMenuItem_Click);
    //
    // loadDeviceFileToolStripMenuItem
    //
    this->loadDeviceFileToolStripMenuItem->Name = L"loadDeviceFileToolStripMenuItem";
    this->loadDeviceFileToolStripMenuItem->Size = System::Drawing::Size(159, 22);
    this->loadDeviceFileToolStripMenuItem->Text = L"Load Device File";
    this->loadDeviceFileToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::loadDeviceFileToolStripMenuItem_Click);
    //
    // helpToolStripMenuItem
    //
    this->helpToolStripMenuItem->Name = L"helpToolStripMenuItem";
    this->helpToolStripMenuItem->Size = System::Drawing::Size(159, 22);
    this->helpToolStripMenuItem->Text = L"Help";
    this->helpToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::helpToolStripMenuItem_Click);
    //
    // aboutToolStripMenuItem
    //
    this->aboutToolStripMenuItem->Name = L"aboutToolStripMenuItem";
    this->aboutToolStripMenuItem->Size = System::Drawing::Size(159, 22);
    this->aboutToolStripMenuItem->Text = L"About";
    this->aboutToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::aboutToolStripMenuItem_Click);
    //
    // exitToolStripMenuItem
    //
    this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
    this->exitToolStripMenuItem->Size = System::Drawing::Size(159, 22);
    this->exitToolStripMenuItem->Text = L"Exit";
    this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::exitToolStripMenuItem_Click);
    //
    // openHexFileDialog
    //
    this->openHexFileDialog->DefaultExt = L"hex";
    this->openHexFileDialog->FileName = L"openHexFileDialog";
    //
    // deviceTextBox
    //
    this->deviceTextBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
    this->deviceTextBox->Location = System::Drawing::Point(84, 390);
    this->deviceTextBox->Name = L"deviceTextBox";
    this->deviceTextBox->ReadOnly = true;
    this->deviceTextBox->Size = System::Drawing::Size(257, 20);
    this->deviceTextBox->TabIndex = 11;
    this->deviceTextBox->Text = L"unknown";
    //
    // hexFileNameBox
    //
    this->hexFileNameBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
    this->hexFileNameBox->Location = System::Drawing::Point(64, 361);
    this->hexFileNameBox->Name = L"hexFileNameBox";
    this->hexFileNameBox->ReadOnly = true;
    this->hexFileNameBox->Size = System::Drawing::Size(471, 20);
    this->hexFileNameBox->TabIndex = 12;
    //
    // hexFileButton
    //
    this->hexFileButton->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
    this->hexFileButton->Location = System::Drawing::Point(3, 358);
    this->hexFileButton->Name = L"hexFileButton";
    this->hexFileButton->Size = System::Drawing::Size(55, 23);
    this->hexFileButton->TabIndex = 13;
    this->hexFileButton->Text = L"HexFile";
    this->hexFileButton->UseVisualStyleBackColor = true;
    this->hexFileButton->Click += gcnew System::EventHandler(this, &Form1::hexFileButton_Click);
    //
    // programHexButton
    //
    this->programHexButton->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
    this->programHexButton->Location = System::Drawing::Point(3, 388);
    this->programHexButton->Name = L"programHexButton";
    this->programHexButton->Size = System::Drawing::Size(75, 23);
    this->programHexButton->TabIndex = 14;
    this->programHexButton->Text = L"Program";
    this->programHexButton->UseVisualStyleBackColor = true;
    this->programHexButton->Click += gcnew System::EventHandler(this, &Form1::programHexButton_Click);
    //
    // sendCRbutton
    //
    this->sendCRbutton->Location = System::Drawing::Point(163, 28);
    this->sendCRbutton->Name = L"sendCRbutton";
    this->sendCRbutton->Size = System::Drawing::Size(69, 23);
    this->sendCRbutton->TabIndex = 15;
    this->sendCRbutton->Text = L"Send&&\\n";
    this->sendCRbutton->UseVisualStyleBackColor = true;
    this->sendCRbutton->Click += gcnew System::EventHandler(this, &Form1::sendCRbutton_Click);
    //
    // openDevFileDialog
    //
    this->openDevFileDialog->FileName = L"openDevFileDialog";
    //
    // checkBoxRTS
    //
    this->checkBoxRTS->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
    this->checkBoxRTS->AutoSize = true;
    this->checkBoxRTS->Location = System::Drawing::Point(238, 331);
    this->checkBoxRTS->Name = L"checkBoxRTS";
    this->checkBoxRTS->Size = System::Drawing::Size(63, 17);
    this->checkBoxRTS->TabIndex = 16;
    this->checkBoxRTS->Text = L"MCLR#";
    this->checkBoxRTS->UseVisualStyleBackColor = true;
    this->checkBoxRTS->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBoxRTS_CheckedChanged);
    //
    // buttonProgramWRTS
    //
    this->buttonProgramWRTS->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
    this->buttonProgramWRTS->Location = System::Drawing::Point(384, 387);
    this->buttonProgramWRTS->Name = L"buttonProgramWRTS";
    this->buttonProgramWRTS->Size = System::Drawing::Size(105, 23);
    this->buttonProgramWRTS->TabIndex = 17;
    this->buttonProgramWRTS->Text = L"MCLR# and Prgrm";
    this->buttonProgramWRTS->UseVisualStyleBackColor = true;
    this->buttonProgramWRTS->Click += gcnew System::EventHandler(this, &Form1::buttonProgramWRTS_Click);
    //
    // timer1
    //
    this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
    //
    // checkBoxLogging
    //
    this->checkBoxLogging->AutoSize = true;
    this->checkBoxLogging->Location = System::Drawing::Point(238, 34);
    this->checkBoxLogging->Name = L"checkBoxLogging";
    this->checkBoxLogging->Size = System::Drawing::Size(106, 17);
    this->checkBoxLogging->TabIndex = 18;
    this->checkBoxLogging->Text = L"Logging Enabled";
    this->checkBoxLogging->UseVisualStyleBackColor = true;
    this->checkBoxLogging->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBoxLogging_CheckedChanged);
    //
    // pgmConfigBox1
    //
    this->pgmConfigBox1->AutoSize = true;
    this->pgmConfigBox1->Location = System::Drawing::Point(350, 34);
    this->pgmConfigBox1->Name = L"pgmConfigBox1";
    this->pgmConfigBox1->Size = System::Drawing::Size(153, 17);
    this->pgmConfigBox1->TabIndex = 19;
    this->pgmConfigBox1->Text = L"Enable Config Bits Prgming";
    this->pgmConfigBox1->UseMnemonic = false;
    this->pgmConfigBox1->UseVisualStyleBackColor = true;
    this->pgmConfigBox1->CheckedChanged += gcnew System::EventHandler(this, &Form1::pgmConfigBox1_CheckedChanged);
    //
    // listViewDataXfer
    //
    this->listViewDataXfer->Alignment = System::Windows::Forms::ListViewAlignment::SnapToGrid;
    this->listViewDataXfer->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
                                     | System::Windows::Forms::AnchorStyles::Right));
    this->listViewDataXfer->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(6) {
      this->columnHeaderValue,
           this->columnHeaderName, this->columnHeaderDesc, this->columnHeaderIndex, this->columnHeaderSize, this->columnHeaderFormat
    });
    this->listViewDataXfer->HeaderStyle = System::Windows::Forms::ColumnHeaderStyle::Nonclickable;
    this->listViewDataXfer->LabelEdit = true;
    this->listViewDataXfer->Location = System::Drawing::Point(0, 482);
    this->listViewDataXfer->Name = L"listViewDataXfer";
    this->listViewDataXfer->Size = System::Drawing::Size(547, 113);
    this->listViewDataXfer->TabIndex = 20;
    this->listViewDataXfer->UseCompatibleStateImageBehavior = false;
    this->listViewDataXfer->View = System::Windows::Forms::View::Details;
    this->listViewDataXfer->AfterLabelEdit += gcnew System::Windows::Forms::LabelEditEventHandler(this, &Form1::listViewDataXfer_AfterLabelEdit);
    //
    // columnHeaderValue
    //
    this->columnHeaderValue->Text = L"Value";
    //
    // columnHeaderName
    //
    this->columnHeaderName->Text = L"Name";
    this->columnHeaderName->Width = 69;
    //
    // columnHeaderDesc
    //
    this->columnHeaderDesc->Text = L"Description";
    this->columnHeaderDesc->Width = 281;
    //
    // columnHeaderIndex
    //
    this->columnHeaderIndex->Text = L"Index";
    this->columnHeaderIndex->Width = 38;
    //
    // columnHeaderSize
    //
    this->columnHeaderSize->Text = L"Size";
    this->columnHeaderSize->Width = 33;
    //
    // columnHeaderFormat
    //
    this->columnHeaderFormat->Text = L"Format";
    //
    // Form1
    //
    this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
    this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
    this->ClientSize = System::Drawing::Size(547, 617);
    this->Controls->Add(this->listViewDataXfer);
    this->Controls->Add(this->pgmConfigBox1);
    this->Controls->Add(this->checkBoxLogging);
    this->Controls->Add(this->buttonProgramWRTS);
    this->Controls->Add(this->checkBoxRTS);
    this->Controls->Add(this->sendCRbutton);
    this->Controls->Add(this->programHexButton);
    this->Controls->Add(this->hexFileButton);
    this->Controls->Add(this->hexFileNameBox);
    this->Controls->Add(this->deviceTextBox);
    this->Controls->Add(this->checkBoxComOpen);
    this->Controls->Add(this->baudBox);
    this->Controls->Add(this->comBox);
    this->Controls->Add(this->debugOut);
    this->Controls->Add(this->sendBox);
    this->Controls->Add(this->statusStripBottom);
    this->Controls->Add(this->topMenuStrip);
    this->Controls->Add(this->sendButton);
    this->Controls->Add(this->mainOutput);
    this->MainMenuStrip = this->topMenuStrip;
    this->Name = L"Form1";
    this->Text = L"PIC24 Bully Bootloader";
    this->statusStripBottom->ResumeLayout(false);
    this->statusStripBottom->PerformLayout();
    this->topMenuStrip->ResumeLayout(false);
    this->topMenuStrip->PerformLayout();
    this->ResumeLayout(false);
    this->PerformLayout();

  }
  #pragma endregion

private:
  System::Void scheduleStatusClear() {
    status_clear = true;
    status_tics = 0;
  }
private:
  System::Void showStatus (String^ s) {
    this->statusStripLabel->Text = s;
    scheduleStatusClear();
    statusStripBottom->Update();
  }

private:
  /// When the text box fills up, begin removing characters from the
  /// beginning to make room to new characters added to the end.
  System::Void pruneOutput(System::Windows::Forms::TextBox^ outBox) {
    // Compute the number of characters over the limit
    Int32 i32_overChars = outBox->TextLength - ((Int32) (outBox->MaxLength*0.9));
    if  (i32_overChars > 0) {
      //We need to prune this text.
      //stop the timer while we do this
      comTimer->Enabled = false;
      // Delete characters from the beginning of the text box
      outBox->Text = outBox->Text->Remove(0, i32_overChars);
      comTimer->Enabled = true;
    }
  }

private:
  System::Void writeToMainOutput(String^ message) {
    if (LogFileStream != nullptr) {
      LogFileStream->Write(String::Concat(message, "\r\n"));
      LogFileStream->Flush();
    }
    pruneOutput(mainOutput);
    this->mainOutput->AppendText(String::Concat(message, "\r\n"));
  }

  System::Void writeToMainOutput(String^ message, System::Boolean linefeed) {
    pruneOutput(mainOutput);
    if (linefeed) {
      if (LogFileStream != nullptr) {
        LogFileStream->Write(String::Concat(message, "\r\n"));
        LogFileStream->Flush();
      }
      this->mainOutput->AppendText(String::Concat(message, "\r\n"));
    } else {
      if (LogFileStream != nullptr) {
        LogFileStream->Write(message);
        LogFileStream->Flush();
      }
      this->mainOutput->AppendText(message);
    }
  }

private:
  System::Void HexFileBrowse(void) {
    openHexFileDialog->CheckFileExists = true;
    openHexFileDialog->CheckPathExists = true;
    openHexFileDialog->DefaultExt = "hex";
    openHexFileDialog->DereferenceLinks = true;
    openHexFileDialog->Filter = "Hex files (*.hex)|*.hex|All files (*.*)|*.*";
    openHexFileDialog->Multiselect = false;
    openHexFileDialog->RestoreDirectory = true;
    openHexFileDialog->ShowHelp = false;
    openHexFileDialog->ShowReadOnly = false;
    openHexFileDialog->Title = "Select a Hex file to open";
    openHexFileDialog->ValidateNames = true;
    if (openHexFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
      hexFile = openHexFileDialog->FileName;
      hexFileNameBox->Text = openHexFileDialog->FileName;
      writeToDebugOutput(String::Concat("Hex file is: ", hexFile));
    }
  }

private:
  System::Void writeToDebugOutput(String^ message) {
    pruneOutput(debugOut);
    this->debugOut->AppendText(String::Concat(message, "\r\n"));
  }

private:
  /// Converts a System::String to a C-style char*. This allocates memory to
  /// store the resulting string, which must be deleted.
  /// \param s String to be converted.
  /// \return Pointer to the convered string. Be sure to delete this when
  ///         finished with it.
  char* stringToC(String^ s) {
    // Using a CString produces the error
    // "An unhandled exception of type 'System.TypeInitializationException' occurred in Unknown Module."
    // the the program is run. I tried changing the project to use the ATL
    // library (staticlly linked) but this didn't help. So, I'm avoiding
    // the use of CString.
    // Taken from http://support.microsoft.com/kb/311259, method 2.
    // However, this forces use of /clr:oldSyntax
    char* pc = (char*)(void*) Marshal::StringToHGlobalAnsi(s);
    return pc;
  }

private:
  // Send a string to the COM port, escaping any characters as necessary.
  System::Void sendStringToCom (String^ s) {
    char* cs = stringToC(s);
    // The st_len includes the NULL teminator; don't send that.
    for (size_t i = 0; cs[i] != 0; i++) {
      outCharXfer(cs[i]);
    }
    Marshal::FreeHGlobal((System::IntPtr) cs);
  }

public:
  /// A low-level routine which sends a string to the COM port with NO escaping
  /// or other interpretation.
  System::Void sendCharToCom(Char c) {
    try {
      serialPort1->Write(c.ToString());
    } catch ( Exception^ e ) {
      writeToDebugOutput(String::Concat("Exception during serial port write: ",e->Message,"\r\n"));
    }
  }

private:
  System::Void mainOutput_TextChanged(System::Object^  sender, System::EventArgs^  e) {
  }

private:
  System::Void sendButton_Click(System::Object^  sender, System::EventArgs^  e) {
    sendStringToCom(sendBox->Text);
  }

private:
  System::Void comTimer_Tick(System::Object^  sender, System::EventArgs^  e) {
    if (status_clear) {
      status_tics++;
      if (status_tics > 100) {
        status_tics = 0;
        status_clear = false;
        this->statusStripLabel->Text = "";
      }
    }
    if (comOpen) {
      String^ temp = nullptr;
      try {
        while (serialPort1->BytesToRead > 0) {
          // Read and parse a byte from the serial port
          Byte b = serialPort1->ReadByte();
          System::UInt16 u16_index;
          System::Char c;
          if (receiveVar(b, c, u16_index)) {
            // If this is a variable, print it
            if (u16_index == CHAR_RECEIVED_INDEX) {
              if (c == 0x0A) {
                //writeToMainOutput("");
                if (temp != nullptr) writeToMainOutput(temp);
                temp = nullptr;
              } else if (c != 0x0D) {
                //echo
                //writeToMainOutput(Convert::ToString(Convert::ToChar(b)), false);
                temp = temp + c;
              }
            } else {
              // Gather data to place in the data transfer list control
              XFER_VAR* pxv = xferVar + u16_index;
              String^ s_varName = gcnew String(pxv->psz_name);
              String^ s_varDesc = gcnew String(pxv->psz_desc);
              String^ s_varFormat = gcnew String(pxv->psz_format);
              System::UInt16 u16_size = pxv->u8_size + 1;
              char psz_buf[256];
              formatVar(u16_index, psz_buf);

              // Place it in the control
              ListViewItem::ListViewSubItemCollection^ sic =
                listViewDataXfer->Items[u16_index]->SubItems;
              sic[0]->Text = gcnew String(psz_buf);
              sic[1]->Text = s_varName;
              sic[2]->Text = s_varDesc;
              sic[4]->Text = u16_size.ToString();
              sic[5]->Text = s_varFormat;
            }
          }
        }
        if (temp != nullptr) writeToMainOutput(temp, false);
      } catch ( Exception^ e ) {
        writeToDebugOutput(String::Concat("Exception during serial port read: ",e->Message,"\r\n"));
        //serial port hosed, close it
        checkBoxComOpen->Checked = !closeComPort();
      }
    }
  }

private:
  System::Void checkBoxComOpen_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
    if (checkBoxComOpen->Checked) {
      checkBoxComOpen->Checked = openComPort();

    } else {
      if (comOpen) checkBoxComOpen->Checked= !closeComPort();
    }
  }

private:
  System::Void exitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
    closeComPort();
    this->Close();
  }

private:
  System::Void browseForHexFileToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
    HexFileBrowse();
  }


private:
  System::Void hexFileButton_Click(System::Object^  sender, System::EventArgs^  e) {
    HexFileBrowse();
  }

private:
  System::Void doProgram() {
    device^ devptr;
    // Record start time
    DateTime d1 = DateTime::Now;
    comTimer->Enabled = false;  //turn off com Timer during programming
    if (hexFile == nullptr) {
      writeToDebugOutput("You must select a hex file before programming!");
      comTimer->Enabled = true;  //turn com Timer back on
      return;
    }
    devptr = loader->ReadDevice(serialPort1, debugOut);
    if (devptr == nullptr) {
      deviceTextBox->Text = "notFound";
      writeToDebugOutput("Unable to connect to device, giving up.");
      comTimer->Enabled = true;  //turn com Timer back on
      return;
    } else deviceTextBox->Text = String::Concat(devptr->name, ", Rev: ",Convert::ToString(devptr->revision,16));

    loader->ProgramHexFile(hexFile,serialPort1, debugOut, pgmConfigBox1->Checked );
    pruneOutput(debugOut);
    comTimer->Enabled = true;  //turn com Timer back on
    // Record end time
    TimeSpan ts = TimeSpan(DateTime::Now - d1);
    String^ msg = "Completed in " + ts.ToString() + "s.";
    writeToDebugOutput(msg);
    // If we're bootloading then exiting, close the comm port
    // and quit after a successfull bootload.
    if (blAndExit) {
      closeComPort();
      // Quit
      this->Close();
    }
  }

private:
  System::Void programHexButton_Click(System::Object^  sender, System::EventArgs^  e) {
    doProgram();
  }

private:
  System::Void aboutToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
    String^ message = "The PIC24 Bully Bootloader, V0.32; comments to reese@ece.msstate.edu";
    String^ caption = "About The PIC24 Bully Bootloader";
    MessageBoxButtons buttons = MessageBoxButtons::OK;
    System::Windows::Forms::DialogResult result;

    // Displays the MessageBox.
    result = MessageBox::Show(this, message, caption, buttons,
                              MessageBoxIcon::Information, MessageBoxDefaultButton::Button1,
                              MessageBoxOptions::RightAlign);
  }

private:
  System::Void helpToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
    writeToDebugOutput("The top output window is for COM port output, the bottom output window is used for bootloader status.");
    writeToDebugOutput("To Program a PIC24 device:");
    writeToDebugOutput("   1. Ensure your PICmicro is programmed with the bootloader firmware, is powered down, and is connected to the serial port of your PC.");
    writeToDebugOutput("   2. Use the HexFile button to browse to a hex file to be programmed.");
    writeToDebugOutput("   3. Select the COM port/baud rate values, and open the com port by left-clicking on the OpenCom checkbox.");
    writeToDebugOutput("   4. Power up (or assert MCLR) on the PICmicro, then click the 'Program' button (must be done within the timeout period of the bootloader firmware, usually about 2 seconds).");
    writeToDebugOutput("      If the RTS# (or DTR#) output of the serial cable is connected to MCLR, then power cycle is not necessary; clicking on the 'MCLR# & Prgrm' button will download the file.");
    writeToDebugOutput("      Careful - the MCLR input of PIC24 micros is not 5 V tolerant, so ensure that the RTS# (or DTR#) high state is 3.3 V or less.");
    writeToDebugOutput("");
    writeToDebugOutput("After the device is programmed, the Send button can be used to send text in the adjacent type-in box to the COM port.");
    writeToDebugOutput("NOTE: Configuration bit programming has not been tested for PIC24E or any dsPIC devices.");
  }

private:
  System::Void sendCRbutton_Click(System::Object^  sender, System::EventArgs^  e) {
    sendStringToCom(String::Concat(sendBox->Text,"\n"));
  }

private:
  System::Void sendTextFileToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
    openHexFileDialog->CheckFileExists = true;
    openHexFileDialog->CheckPathExists = true;
    openHexFileDialog->DefaultExt = "txt";
    openHexFileDialog->DereferenceLinks = true;
    openHexFileDialog->Filter = "Text files (*.txt)|*.txt|All files (*.*)|*.*";
    openHexFileDialog->Multiselect = false;
    openHexFileDialog->RestoreDirectory = true;
    openHexFileDialog->ShowHelp = false;
    openHexFileDialog->ShowReadOnly = false;
    openHexFileDialog->Title = "Select a Text file to send to COM port";
    openHexFileDialog->ValidateNames = true;
    if (openHexFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
      textFile = openHexFileDialog->FileName;
      writeToDebugOutput(String::Concat("Sending file to COM port: ",textFile));
      StreamReader^ sr = gcnew StreamReader( textFile );
      try {

        String^ textLine;
        while ( (textLine = sr->ReadLine()) != nullptr ) {
          serialPort1->WriteLine(textLine);
        }
        sr->Close();
      } catch ( Exception^ e ) {
        writeToDebugOutput(String::Concat("Exception while sending Text file: ",e->Message,"\r\n"));
        sr->Close();
      }
    }
  }

private:
  System::Void loadDeviceFile() {
    String^ devFileName = String::Concat(Environment::CurrentDirectory,"\\devices.txt");
    writeToDebugOutput(String::Concat("Loading Device File: ",devFileName));
    StreamReader^ sr;
    try {
      sr = gcnew StreamReader( devFileName );
    } catch ( Exception^ e ) {
      writeToDebugOutput(String::Concat("Exception while opening device file: ",e->Message,"\r\n"));
      return;
    }
    try {
      String^ textLine;
      while ( (textLine = sr->ReadLine()) != nullptr ) {
        loader->ParseDeviceLine(textLine, debugOut );
      }
      sr->Close();
      writeToDebugOutput("Device File loaded\r\n");
    } catch ( Exception^ e ) {
      writeToDebugOutput(String::Concat("Exception while loading Device file: ",e->Message,"\r\n"));
      sr->Close();
    }
  }

private:
  System::Void openLogFile() {
    String^ devFileName = String::Concat(Environment::CurrentDirectory,"\\bullyBootloaderLog.txt");
    writeToDebugOutput(String::Concat("Opening: ",devFileName));

    try {
      LogFileStream = gcnew StreamWriter( devFileName );
    } catch ( Exception^ e ) {
      writeToDebugOutput(String::Concat("Exception while opening log file: ",e->Message,"\r\n"));
      return;
    }
    writeToDebugOutput(String::Concat("Log file opened: ",devFileName,"\r\n"));
  }

private:
  System::Void closeLogFile() {
    String^ devFileName = String::Concat(Environment::CurrentDirectory,"\\bullyBootloaderLog.txt");
    writeToDebugOutput(String::Concat("Closing: ",devFileName));
    try {
      if (LogFileStream != nullptr) LogFileStream->Close();
      LogFileStream = nullptr;
    } catch ( Exception^ e ) {
      writeToDebugOutput(String::Concat("Exception while closing log file: ",e->Message,"\r\n"));
      return;
    }
    writeToDebugOutput(String::Concat("Log file closed: ",devFileName,"\r\n"));
  }

private:
  System::Void loadDeviceFileToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
    openDevFileDialog->CheckFileExists = true;
    openDevFileDialog->CheckPathExists = true;
    openDevFileDialog->DefaultExt = "txt";
    openDevFileDialog->DereferenceLinks = true;
    openDevFileDialog->Filter = "Text files (*.txt)|*.txt|All files (*.*)|*.*";
    openDevFileDialog->Multiselect = false;
    openDevFileDialog->RestoreDirectory = true;
    openDevFileDialog->ShowHelp = false;
    openDevFileDialog->ShowReadOnly = false;
    openDevFileDialog->Title = "Select a device file to load";
    openDevFileDialog->ValidateNames = true;
    if (openDevFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
      textFile = openDevFileDialog->FileName;
      writeToDebugOutput(String::Concat("Loading Device File: ",textFile));
      StreamReader^ sr;
      try {
        sr = gcnew StreamReader( textFile );
      } catch ( Exception^ e ) {
        writeToDebugOutput(String::Concat("Exception while opening device file: ",e->Message,"\r\n"));
        return;
      }

      try {

        String^ textLine;
        while ( (textLine = sr->ReadLine()) != nullptr ) {
          loader->ParseDeviceLine(textLine, debugOut );
        }
        if (sr) sr->Close();
      } catch ( Exception^ e ) {
        writeToDebugOutput(String::Concat("Exception while loading Device file: ",e->Message,"\r\n"));
        if (sr) sr->Close();
      }
    }
  }

private:
  System::Void checkBoxRTS_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
    if (checkBoxRTS->Checked ) {
      if (comOpen)   {
        serialPort1->RtsEnable = true;
        serialPort1->DtrEnable = true;
      }
    } else {
      if (comOpen) {
        serialPort1->RtsEnable = false;
        serialPort1->DtrEnable = false;
      }
    }
  }

private:
  System::Void buttonProgramWRTS_Click(System::Object^  sender, System::EventArgs^  e) {
    mystates = States::RESET_LOW;
  }

private:
  System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
    switch (mystates) {
      case States::RESET_LOW :
        if (comOpen)   {
          serialPort1->RtsEnable = true;
          serialPort1->DtrEnable = true;
        }
        mystates = States::RESET_HIGH;
        break;
      case States::RESET_HIGH :
        if (comOpen)  {
          serialPort1->RtsEnable = false;
          serialPort1->DtrEnable = false;
        }
        mystates = States::PROGRAM;
        break;
      case States::PROGRAM :
        doProgram();
        mystates= States::NULL_STATE;
        break;
      default:
        break;
    }
  }

private:
  System::Void baudBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
    if (comOpen) {
      closeComPort();
      openComPort();
    }
  }

private:
  System::Void checkBoxLogging_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
    if (checkBoxLogging->Checked ) {
      openLogFile();
    } else {
      closeLogFile();
    }
  }

private:
  System::Void pgmConfigBox1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
  }

private:
  System::Void mainOutput_keyPressed(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
    // Convert the keypress to a string then send it
    sendStringToCom(e->KeyChar.ToString());
    // Mark this character as handled, so the edit box
    // doesn't try to display it.
    e->Handled = true;
  }

private:
  DateTime dt_receiveVar;
  /** Receive a character or a variable. Any errors that occur are
  *  reported via outString.
  *  \param c_in  Character just received, which will be processed.
  *  \param c_out Reference to space for storing the received character, if
  *                 a character was received (see u16_index).
  *  \param u16_index An index to the variable, if a variable was received;
  *                     \ref CHAR_RECEIVED_INDEX if a character was received.
  *  \return True if a character or variable was received. True also implies that
  *            u16_index is valid.
  */
  System::Boolean receiveVar(System::Char c_in, System::Char& c_out, System::UInt16& u16_index) {
    TimeSpan ts_maxTime(0, 0, 0, 0, 100);

    // Step the machine
    TimeSpan ts = DateTime::Now - dt_receiveVar;
    dt_receiveVar = DateTime::Now;
    if (ts > ts_maxTime)
      notifyOfTimeout();
    RECEIVE_ERROR re = stepReceiveMachine((char) c_in);
    if (re != ERR_NONE) {
      String^ s = gcnew String(getReceiveErrorString());
      writeToDebugOutput("Data receive error: " + s + "\n");
    }
    if (isReceiveMachineChar()) {
      c_out = getReceiveMachineOutChar();
      u16_index = CHAR_RECEIVED_INDEX;
      return true;
    }
    if (isReceiveMachineData()) {
      u16_index = getReceiveMachineIndex();
      return true;
    }
    return false;
  }

private:
  /// Send edited values back to the PIC.
  System::Void listViewDataXfer_AfterLabelEdit(System::Object^  sender, System::Windows::Forms::LabelEditEventArgs^  e) {
    // Code copied from the MSDN help for AfterLabelEdit.
    // Determine if label is changed by checking for 0.
    if (e->Label == nullptr)
      return;

    // Parse the string to a number. If it can't be converted, reject the edit.
    ASSERT(e->Item < NUM_XFER_VARS);
    XFER_VAR* pXferVar = xferVar + e->Item;
    UInt64 u64_value;
    char* psz_str = stringToC(e->Label->ToString());
    int i_ret = sscanf(psz_str, pXferVar->psz_format, &u64_value);
    Marshal::FreeHGlobal((System::IntPtr) psz_str);
    if ((i_ret == 0) || (i_ret == EOF)) {
      e->CancelEdit = true;
      return;
    }

    // Re-print the scanned data, so that entries like "  5 g" will be cleaned up to "5".
    char psz_buf[200];
    i_ret = formatVar(e->Item, psz_buf);
    ASSERT( (i_ret < sizeof(psz_buf)) && (i_ret >= 0) );
    ListViewItem::ListViewSubItemCollection^ sic =
      listViewDataXfer->Items[e->Item]->SubItems;
    sic[0]->Text = gcnew String(psz_buf);

    // Send it
    uint8_t u8_size = pXferVar->u8_size + 1;
    ASSERT(u8_size <= sizeof(u64_value));
    memcpy(pXferVar->pu8_data, &u64_value, u8_size);
    sendVar(e->Item);
  }
};



/// A class to store a pointer to the form, so C can get to it
/// through the function below.
public ref class Globals {
public:
  static Form1^ form1 = nullptr;
};

/// Called from C, this uses the form to transmit one character.
extern "C" void outChar(unsigned char c) {
  Globals::form1->sendCharToCom(c);
}

}
