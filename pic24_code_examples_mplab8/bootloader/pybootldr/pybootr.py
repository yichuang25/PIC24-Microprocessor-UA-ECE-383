#!/usr/bin/env python
# this file is maintained by wxGlade
#
# do not modify code between
#           #begin wxGlade:
# and
#           #end wxGlade
# statements.  wxGlade controls the code between these
# two statements.


import  wx
import  wxSerialConfigDialog
import  serial
import  threading
import  os

from    gui_fcns import *
import  fcns
import  pickit2

#----------------------------------------------------------------------
# Create an own event type, so that GUI updates can be delegated
# this is required as on some platforms only the main thread can
# access the GUI without crashing. wxMutexGuiEnter/wxMutexGuiLeave
# could be used too, but an event is more elegant.

#SERIALRX = wx.NewEventType()
## bind to serial data receive events
#EVT_SERIALRX = wx.PyEventBinder(SERIALRX, 0)

#class SerialRxEvent(wx.PyCommandEvent):
    #eventType = SERIALRX
    #def __init__(self, windowID, data):
        #wx.PyCommandEvent.__init__(self, self.eventType, windowID)
        #self.data = data

    #def Clone(self):
        #self.__class__(self.GetId(), self.data)

#----------------------------------------------------------------------
# create the GUI widget IDs here
#----------------------------------------------------------------------
# GUI IDs for File... menu
ID_OPEN         = wx.NewId()
ID_CLEAR        = wx.NewId()
ID_SAVEAS       = wx.NewId()
ID_SHOW_LOG     = wx.NewId()
ID_SETTINGS     = wx.NewId()
ID_TERM         = wx.NewId()
ID_EXIT         = wx.NewId()
# GUI IDs for Help menu
ID_HELP         = wx.NewId()
ID_ABOUT        = wx.NewId()
# GUI IDs for PicKit2 notebook tab
ID_TBTN_EN_PK2  = wx.NewId()
ID_BTN_QRY_PK2  = wx.NewId()
ID_CHBX_VDD     = wx.NewId()
ID_CHBX_MCLR    = wx.NewId()
ID_BTN_WR_HEX   = wx.NewId()
ID_BTN_RD_2_HEX = wx.NewId()
ID_SPIN_VDD     = wx.NewId()


NEWLINE_CR      = 0
NEWLINE_LF      = 1
NEWLINE_CRLF    = 2

class TerminalSetup:
    """Placeholder for various terminal settings. Used to pass the
       options to the TerminalSettingsDialog."""
    def __init__(self):
        self.echo = False
        self.unprintable = False
        self.newline = NEWLINE_CRLF

class TerminalSettingsDialog(wx.Dialog):
    """Simple dialog with common terminal settings like echo, newline mode."""
    
    def __init__(self, *args, **kwds):
        self.settings = kwds['settings']
        del kwds['settings']
        # begin wxGlade: TerminalSettingsDialog.__init__
        kwds["style"] = wx.DEFAULT_DIALOG_STYLE
        wx.Dialog.__init__(self, *args, **kwds)
        self.sizer_4_staticbox = wx.StaticBox(self, -1, "Input/Output")
        self.checkbox_echo = wx.CheckBox(self, -1, "Local Echo")
        self.checkbox_unprintable = wx.CheckBox(self, -1, "Show unprintable characters")
        self.radio_box_newline = wx.RadioBox(self, -1, "Newline Handling", choices=["CR only", "LF only", "CR+LF"], majorDimension=0, style=wx.RA_SPECIFY_ROWS)
        self.button_ok = wx.Button(self, -1, "OK")
        self.button_cancel = wx.Button(self, -1, "Cancel")

        self.__set_properties()
        self.__do_layout()
        # end wxGlade
        self.__attach_events()
        self.checkbox_echo.SetValue(self.settings.echo)
        self.checkbox_unprintable.SetValue(self.settings.unprintable)
        self.radio_box_newline.SetSelection(self.settings.newline)

    def __set_properties(self):
        # begin wxGlade: TerminalSettingsDialog.__set_properties
        self.SetTitle("Terminal Settings")
        self.radio_box_newline.SetSelection(0)
        self.button_ok.SetDefault()
        # end wxGlade

    def __do_layout(self):
        # begin wxGlade: TerminalSettingsDialog.__do_layout
        sizer_2 = wx.BoxSizer(wx.VERTICAL)
        sizer_3 = wx.BoxSizer(wx.HORIZONTAL)
        sizer_4 = wx.StaticBoxSizer(self.sizer_4_staticbox, wx.VERTICAL)
        sizer_4.Add(self.checkbox_echo, 0, wx.ALL, 4)
        sizer_4.Add(self.checkbox_unprintable, 0, wx.ALL, 4)
        sizer_4.Add(self.radio_box_newline, 0, 0, 0)
        sizer_2.Add(sizer_4, 0, wx.EXPAND, 0)
        sizer_3.Add(self.button_ok, 0, 0, 0)
        sizer_3.Add(self.button_cancel, 0, 0, 0)
        sizer_2.Add(sizer_3, 0, wx.ALL|wx.ALIGN_RIGHT, 4)
        self.SetSizer(sizer_2)
        sizer_2.Fit(self)
        self.Layout()
        # end wxGlade

    def __attach_events(self):
        self.Bind(EVT_BUTTON, self.OnOK, id = self.button_ok.GetId())
        self.Bind(EVT_BUTTON, self.OnCancel, id = self.button_cancel.GetId())
    
    def OnOK(self, events):
        """Update data wil new values and close dialog."""
        self.settings.echo = self.checkbox_echo.GetValue()
        self.settings.unprintable = self.checkbox_unprintable.GetValue()
        self.settings.newline = self.radio_box_newline.GetSelection()
        self.EndModal(wx.ID_OK)
    
    def OnCancel(self, events):
        """Do not update data but close dialog."""
        self.EndModal(wx.ID_CANCEL)

# end of class TerminalSettingsDialog


class TerminalFrame(wx.Frame):
    """Simple terminal program for wxPython"""
    
    def __init__(self, *args, **kwds):
        self.serial = serial.Serial()
        self.serial.timeout = 0.5   #make sure that the alive event can be checked from time to time
        self.settings = TerminalSetup() #placeholder for the settings
        self.thread = None
        self.alive = threading.Event()
        self.log = LogHtmlWindow(None, "html LOG window")
        self.TitleStr = "BULLY's PyBootLoader for "
        self.TitleDevStr = "PIC24"
        # begin wxGlade: TerminalFrame.__init__
        kwds["style"] = wx.DEFAULT_FRAME_STYLE
        wx.Frame.__init__(self, *args, **kwds)
        self.notebook_1 = wx.Notebook(self, -1, style=0)
        self.notebook_1_pane_4 = wx.Panel(self.notebook_1, -1)
        self.notebook_1_pane_3 = wx.Panel(self.notebook_1, -1)
        self.notebook_1_pane_2 = wx.Panel(self.notebook_1, -1)
        self.sizer_12_staticbox = wx.StaticBox(self.notebook_1_pane_4, -1, "Vdd/MCLR# Control")
        self.sizer_1_staticbox = wx.StaticBox(self, -1, "")
        self.notebook_1_pane_1 = wx.Panel(self.notebook_1, -1)
        
        # Menu Bar
        self.frame_terminal_menubar = wx.MenuBar()
        wxglade_tmp_menu = wx.Menu()
        wxglade_tmp_menu.Append(ID_OPEN, "&Open...", "Select hex file for loading", wx.ITEM_NORMAL)
        wxglade_tmp_menu.Append(ID_CLEAR, "&Clear session", "Clear the session window", wx.ITEM_NORMAL)
        wxglade_tmp_menu.Append(ID_SAVEAS, "&Save Text As...", "Save session text to file", wx.ITEM_NORMAL)
        wxglade_tmp_menu.Append(ID_SHOW_LOG, "Show log window", "Show log of communications", wx.ITEM_CHECK)
        wxglade_tmp_menu.AppendSeparator()
        wxglade_tmp_menu.Append(ID_SETTINGS, "&Port Settings...", "Change serial port settings", wx.ITEM_NORMAL)
        wxglade_tmp_menu.Append(ID_TERM, "&Terminal Settings...", "Change the session settings", wx.ITEM_NORMAL)
        wxglade_tmp_menu.AppendSeparator()
        wxglade_tmp_menu.Append(ID_EXIT, "E&xit", "Quit this gnarly app", wx.ITEM_NORMAL)
        self.frame_terminal_menubar.Append(wxglade_tmp_menu, "&File")
        wxglade_tmp_menu = wx.Menu()
        wxglade_tmp_menu.Append(ID_HELP, "Help", "Not yet implemented. Sorry!", wx.ITEM_NORMAL)
        wxglade_tmp_menu.AppendSeparator()
        wxglade_tmp_menu.Append(ID_ABOUT, "About", "Learn more about Bully's PyBootLoader", wx.ITEM_NORMAL)
        self.frame_terminal_menubar.Append(wxglade_tmp_menu, "Help")
        self.SetMenuBar(self.frame_terminal_menubar)
        # Menu Bar end
        self.frame_terminal_statusbar = self.CreateStatusBar(3, 0)
        self.text_ctrl_output = wx.TextCtrl(self, -1, "", style=wx.TE_MULTILINE|wx.TE_READONLY|wx.TE_RICH)
        self.label_4 = wx.StaticText(self.notebook_1_pane_1, -1, "This tab will eventually contain some nice serial RX/TX tools like RealTerm")
        self.label_3 = wx.StaticText(self.notebook_1_pane_2, -1, "This tab will eventually contain the controls for the bootloader and process", style=wx.ALIGN_CENTRE)
        self.label_5 = wx.StaticText(self.notebook_1_pane_3, -1, "This tab will eventually contain tools to control I2C transactions\nwith the ECE4723 firmware")
        self.tbtn_Pk2EnablePk2 = wx.ToggleButton(self.notebook_1_pane_4, ID_TBTN_EN_PK2, "Enable PicKit2")
        self.btn_Pk2QueryTarget = wx.Button(self.notebook_1_pane_4, ID_BTN_QRY_PK2, "Query for attached target")
        self.spinctrl_Vdd = wx.SpinCtrl(self.notebook_1_pane_4, ID_SPIN_VDD, "35", min=25, max=36)
        self.label_2 = wx.StaticText(self.notebook_1_pane_4, -1, "Volts")
        self.btn_Pk2ProgramHex = wx.Button(self.notebook_1_pane_4, ID_BTN_WR_HEX, "Program .HEX file to device")
        self.btn_Pk2ReadHex = wx.Button(self.notebook_1_pane_4, ID_BTN_RD_2_HEX, "Read device into .HEX file")
        self.chbx_Pk2Vdd = wx.CheckBox(self.notebook_1_pane_4, ID_CHBX_VDD, "Vdd")
        self.chbx_Pk2Mclr = wx.CheckBox(self.notebook_1_pane_4, ID_CHBX_MCLR, "Release MCLR#")
        self.slider_Pk2Vdd = wx.Slider(self.notebook_1_pane_4, -1, 35, 25, 50, style=wx.SL_HORIZONTAL|wx.SL_AUTOTICKS|wx.SL_LABELS|wx.SL_RIGHT)

        self.__set_properties()
        self.__do_layout()
        # end wxGlade
        self.__createGuiLists()
        self.__attach_events()          #register events
        self.OnPortSettings(None)       #call setup dialog on startup, opens port
        if not self.alive.isSet():
            self.Close()

    def StartThread(self):
        """Start the receiver thread"""        
        self.thread = threading.Thread(target=self.ComPortThread)
        self.thread.setDaemon(1)
        self.thread.start()
        self.alive.set()

    def StopThread(self):
        """Stop the receiver thread, wait util it's finished."""
        if self.thread is not None:
            self.alive.clear()          #clear alive event for thread
            self.thread.join()          #wait until thread has finished
            self.thread = None

    def __set_properties(self):
        # begin wxGlade: TerminalFrame.__set_properties
        self.SetTitle("BULLY's PyBootloader for PIC24")
        _icon = wx.EmptyIcon()
        _icon.CopyFromBitmap(wx.Bitmap("./mstate_100.jpg", wx.BITMAP_TYPE_ANY))
        self.SetIcon(_icon)
        self.SetSize((613, 987))
        self.frame_terminal_statusbar.SetStatusWidths([400, 100, -1])
        # statusbar fields
        frame_terminal_statusbar_fields = ["Welcome to BULLY's PyBootloader", "", ""]
        for i in range(len(frame_terminal_statusbar_fields)):
            self.frame_terminal_statusbar.SetStatusText(frame_terminal_statusbar_fields[i], i)
        self.tbtn_Pk2EnablePk2.SetToolTipString("Looks for and enables attached PicKit2 programmer")
        self.btn_Pk2QueryTarget.SetToolTipString("Queries PicKit2 to see what MCU target is currently attached")
        self.btn_Pk2QueryTarget.Enable(False)
        self.btn_Pk2ProgramHex.SetToolTipString("Select a .HEX file and program attached target")
        self.btn_Pk2ProgramHex.Enable(False)
        self.btn_Pk2ReadHex.SetToolTipString("Reads the program and configuration memory from attached target and places the results into a .HEX file")
        self.btn_Pk2ReadHex.Enable(False)
        self.chbx_Pk2Vdd.SetToolTipString("Power the attached target from PicKit2")
        self.chbx_Pk2Vdd.Enable(False)
        self.chbx_Pk2Mclr.SetToolTipString("Have PicKit2 release MCLR# so device will come out of reset")
        self.chbx_Pk2Mclr.Enable(False)
        self.slider_Pk2Vdd.SetMinSize((170, 40))
        self.slider_Pk2Vdd.Enable(False)
        self.notebook_1.SetMinSize((580,80))
        # end wxGlade

    def __do_layout(self):
        # begin wxGlade: TerminalFrame.__do_layout
        sizer_1 = wx.StaticBoxSizer(self.sizer_1_staticbox, wx.HORIZONTAL)
        sizer_5 = wx.BoxSizer(wx.VERTICAL)
        sizer_11 = wx.BoxSizer(wx.VERTICAL)
        sizer_12 = wx.StaticBoxSizer(self.sizer_12_staticbox, wx.VERTICAL)
        sizer_16 = wx.BoxSizer(wx.HORIZONTAL)
        sizer_17 = wx.BoxSizer(wx.HORIZONTAL)
        sizer_13 = wx.BoxSizer(wx.HORIZONTAL)
        grid_sizer_1 = wx.GridSizer(3, 3, 0, 0)
        sizer_14 = wx.BoxSizer(wx.VERTICAL)
        sizer_15 = wx.BoxSizer(wx.HORIZONTAL)
        sizer_19 = wx.BoxSizer(wx.HORIZONTAL)
        sizer_18 = wx.BoxSizer(wx.HORIZONTAL)
        sizer_6 = wx.BoxSizer(wx.HORIZONTAL)
        sizer_5.Add(self.text_ctrl_output, 5, wx.EXPAND, 0)
        sizer_6.Add(self.label_4, 0, 0, 0)
        self.notebook_1_pane_1.SetSizer(sizer_6)
        sizer_18.Add(self.label_3, 0, 0, 0)
        self.notebook_1_pane_2.SetSizer(sizer_18)
        sizer_19.Add(self.label_5, 0, 0, 0)
        self.notebook_1_pane_3.SetSizer(sizer_19)
        sizer_11.Add(self.tbtn_Pk2EnablePk2, 0, 0, 0)
        grid_sizer_1.Add(self.btn_Pk2QueryTarget, 0, wx.ALIGN_CENTER_VERTICAL, 0)
        grid_sizer_1.Add((20, 20), 0, 0, 0)
        sizer_15.Add(self.spinctrl_Vdd, 0, wx.ALIGN_CENTER_VERTICAL, 0)
        sizer_15.Add(self.label_2, 0, wx.ALIGN_CENTER_VERTICAL, 0)
        sizer_14.Add(sizer_15, 1, wx.EXPAND, 0)
        grid_sizer_1.Add(sizer_14, 1, wx.EXPAND, 0)
        grid_sizer_1.Add(self.btn_Pk2ProgramHex, 0, wx.EXPAND, 0)
        grid_sizer_1.Add((20, 20), 0, 0, 0)
        grid_sizer_1.Add((20, 20), 0, 0, 0)
        grid_sizer_1.Add(self.btn_Pk2ReadHex, 0, wx.EXPAND, 0)
        grid_sizer_1.Add((20, 20), 0, 0, 0)
        grid_sizer_1.Add((20, 20), 0, 0, 0)
        sizer_11.Add(grid_sizer_1, 1, wx.EXPAND, 0)
        sizer_13.Add(self.chbx_Pk2Vdd, 1, wx.ALIGN_CENTER_VERTICAL, 0)
        sizer_13.Add(self.chbx_Pk2Mclr, 1, wx.ALIGN_CENTER_VERTICAL, 0)
        sizer_12.Add(sizer_13, 1, wx.EXPAND, 0)
        sizer_17.Add(self.slider_Pk2Vdd, 0, wx.EXPAND, 0)
        sizer_16.Add(sizer_17, 1, wx.EXPAND, 0)
        sizer_12.Add(sizer_16, 1, wx.EXPAND, 0)
        sizer_11.Add(sizer_12, 1, wx.EXPAND, 0)
        self.notebook_1_pane_4.SetSizer(sizer_11)
        self.notebook_1.AddPage(self.notebook_1_pane_1, "Serial")
        self.notebook_1.AddPage(self.notebook_1_pane_2, "Bootloader")
        self.notebook_1.AddPage(self.notebook_1_pane_3, "I2C")
        self.notebook_1.AddPage(self.notebook_1_pane_4, "PicKit2")
        sizer_5.Add(self.notebook_1, 3, 0, 0)
        sizer_1.Add(sizer_5, 1, wx.EXPAND, 0)
        self.SetSizer(sizer_1)
        self.Layout()
        # end wxGlade

    def __attach_events(self):
        # Menu events
        self.Bind(wx.EVT_MENU, self.OnClear, id = ID_CLEAR)
        self.Bind(wx.EVT_MENU, self.OnSaveAs, id = ID_SAVEAS)
        self.Bind(wx.EVT_MENU, self.OnExit, id = ID_EXIT)
        self.Bind(wx.EVT_MENU, self.OnPortSettings, id = ID_SETTINGS)
        self.Bind(wx.EVT_MENU, self.OnTermSettings, id = ID_TERM)
        self.text_ctrl_output.Bind(wx.EVT_CHAR, self.OnKey)        
        self.Bind(EVT_SERIALRX, self.OnSerialRead)
        self.Bind(wx.EVT_CLOSE, self.OnClose)
        self.Bind(wx.EVT_MENU, self.OnLogWindowBtnPress, id = ID_SHOW_LOG)
        self.Bind(wx.EVT_MENU, self.OnAboutBox, id = ID_ABOUT)

        # PicKit2 notebook tab events
        self.Bind(wx.EVT_CHECKBOX, self.UpdateVddMclr, id = ID_CHBX_VDD)
        self.Bind(wx.EVT_CHECKBOX, self.UpdateVddMclr, id = ID_CHBX_MCLR)
        self.Bind(wx.EVT_SPINCTRL, self.UpdateVddMclr, id = ID_SPIN_VDD)
        self.Bind(wx.EVT_BUTTON, self.OnWriteHexBtn, id = ID_BTN_WR_HEX)
        self.Bind(wx.EVT_BUTTON, self.OnReadHexBtn, id = ID_BTN_RD_2_HEX)
        self.Bind(wx.EVT_TOGGLEBUTTON, self.OnEnablePk2, id = ID_TBTN_EN_PK2)

    def __createGuiLists(self):
        self.listVddMclrGuiObjs = []
        self.listVddMclrGuiObjs.append( self.btn_Pk2QueryTarget )
        self.listVddMclrGuiObjs.append( self.spinctrl_Vdd )
        self.listVddMclrGuiObjs.append( self.btn_Pk2ProgramHex )
        self.listVddMclrGuiObjs.append( self.btn_Pk2ReadHex )
        self.listVddMclrGuiObjs.append( self.chbx_Pk2Vdd )
        self.listVddMclrGuiObjs.append( self.chbx_Pk2Mclr )
        self.listVddMclrGuiObjs.append( self.slider_Pk2Vdd )

    def UpdateFrameTitle(self):
        self.SetTitle(self.TitleStr+self.TitleDevStr)

    def OnEnablePk2(self, event):
        if self.tbtn_Pk2EnablePk2.GetValue():
            for obj in self.listVddMclrGuiObjs:
                obj.Enable()
        else:
            for obj in self.listVddMclrGuiObjs:
                obj.Disable()
            

    def OnWriteHexBtn(self, event):
        global PICKIT2
        PICKIT2 = getOurPicKit2()
        self.TitleDevStr = PICKIT2.getConnectedDevice()
        self.UpdateFrameTitle()

        filename = None
        dlg = wx.FileDialog(None, "Select HEX file...", os.getcwd(), "*.hex", "*.hex",  wx.FD_OPEN)
        if dlg.ShowModal() ==  wx.ID_OK:
            filename = dlg.GetPath()
        dlg.Destroy()      
        if filename is not None:
            self.log.putStatusText("%s -> %s" % (filename, PICKIT2.getConnectedDevice()) )
            updateStatusBar(self, "%s -> %s" % (filename, PICKIT2.getConnectedDevice()) )
            #PICKIT2.programHexFile(filename)
            guiProgressHexFile(self, filename)
            updateStatusBar(self, "Programming complete." )
                         
    def OnReadHexBtn(self, event):
      global PICKIT2
      PICKIT2 = getOurPicKit2()
      self.TitleDevStr = PICKIT2.getConnectedDevice()
      self.UpdateFrameTitle()
            
      filename = None
      dlg = wx.FileDialog(None, "Select HEX file...", os.getcwd(), "*.hex",
              "*.hex",  wx.FD_SAVE|wx.FD_OVERWRITE_PROMPT)
      if dlg.ShowModal() ==  wx.ID_OK:
          filename = dlg.GetPath()
      dlg.Destroy()      
      if filename is not None:
        if os.path.exists(filename):
          dlg = wx.MessageDialog(None, "That file already exists!", "File Exists",
              wx.ICON_HAND | wx.OK)
          if dlg.ShowModal() ==  wx.ID_OK:
              dlg.Destroy()

        self.log.putStatusText("%s -> %s" % (PICKIT2.getConnectedDevice(), filename) )
        updateStatusBar(self, "%s -> %s" % (PICKIT2.getConnectedDevice(), filename) )
        #PICKIT2.programHexFile(filename)
        guiProgressHexFile(self, filename, read=True)
        updateStatusBar(self, "Device read complete." )
                         
                         
    def OnLogWindowBtnPress(self, event):
      if self.frame_terminal_menubar.IsChecked(ID_SHOW_LOG):
        self.log.Show()
      else:
        self.log.Hide()
    
    # something changed in the voltage/Vdd/MCLR area, so update our PicKit2
    def UpdateVddMclr(self, event):
      global PICKIT2
      PICKIT2 = getOurPicKit2()       
      self.TitleDevStr = PICKIT2.getConnectedDevice()
      self.UpdateFrameTitle()
      
      """update our PicKit2 voltage"""
      PICKIT2.setVoltage( self.spinctrl_Vdd.GetValue()/10.0 )
      
      """update state based on user-checked Vdd/Mclr checkboxes"""
      if (self.chbx_Vdd.IsChecked()):
        self.spinctrl_Vdd.Disable()
        if (self.chbx_MCLR.IsChecked()):
          PICKIT2.run()
        else:
          PICKIT2.halt()
      else:
        self.spinctrl_Vdd.Enable()        
        if (self.chbx_MCLR.IsChecked()):
          PICKIT2.setHoldInReset(True)
          PICKIT2.setPowered(False)
        else:
          PICKIT2.setHoldInReset(True)
          PICKIT2.setPowered(False)
        
    def OnExit(self, event):
        """Menu point Exit"""
        self.Close()

    def OnClose(self, event):
        """Called on application shutdown."""
        self.StopThread()               #stop reader thread
        self.serial.close()             #cleanup
        self.log.Destroy()        #close the LogWindow
        self.Destroy()                  #close windows, exit app

    def OnSaveAs(self, event):
        """Save contents of output window."""
        filename = None
        dlg = wx.FileDialog(None, "Save Text As...", ".", "", "Text File|*.txt|All Files|*",  wx.SAVE)
        if dlg.ShowModal() ==  wx.ID_OK:
            filename = dlg.GetPath()
        dlg.Destroy()
        
        if filename is not None:
            f = file(filename, 'w')
            text = self.text_ctrl_output.GetValue()
            if type(text) == unicode:
                text = text.encode("latin1")    #hm, is that a good asumption?
            f.write(text)
            f.close()
    
    def OnClear(self, event):
        """Clear contents of output window."""
        self.text_ctrl_output.Clear()
    
    def OnAboutBox(self, event):
        description = """Bully's PyBootLoader is the definitive
        bootloader application for the Microchip(TM) catalog of
        microcontrollers.  Bully's PyBootLoader programs
        Microchip microcontroller devices using the serial/RS-232
        interface with target bootloader firmware.
        Bully's PyBootLoader can also program and can control
        Microchip microcontroller targets using the PicKit2 via
        the pk2cmd utility.
        
        Bully's PyBootLoader runs under Python and wxPython
        on Windows(TM), Linux, and MacOSX(TM) mahcines."""
        
        licence = """Bully's PyBootLoader is free software; you can redistribute it
and/or modify it under the terms of the following license:
=============================================================
        
"Copyright (c) 2008 Robert B. Reese, Bryan A. Jones, J. W. Bruce ("AUTHORS")"
All rights reserved.
(R. Reese, reese_AT_ece.msstate.edu, Mississippi State University)
(B. A. Jones, bjones_AT_ece.msstate.edu, Mississippi State University)
(J. W. Bruce, jwbruce_AT_ece.msstate.edu, Mississippi State University)
 
Permission to use, copy, modify, and distribute this software and its
documentation for any purpose, without fee, and without written agreement is
hereby granted, provided that the above copyright notice, the following
two paragraphs and the authors appear in all copies of this software.
        
IN NO EVENT SHALL THE "AUTHORS" BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE "AUTHORS"
HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
        
THE "AUTHORS" SPECIFICALLY DISCLAIMS ANY WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
ON AN "AS IS" BASIS, AND THE "AUTHORS" HAS NO OBLIGATION TO
PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
        """
        info = wx.AboutDialogInfo()
        info.SetIcon(wx.Icon('./msstate_120.jpg', wx.BITMAP_TYPE_JPEG))
        info.SetName("Bully's PyBootLoader")
        info.SetVersion('1.0')
        info.SetDescription(description)
        info.SetCopyright('(C) 2008 J.W. Bruce')
        info.SetWebSite('http://www.ece.msstate.edu')
        info.SetLicence(licence)
        info.AddDeveloper('J.W. Bruce')
        info.AddDeveloper('Bryan A. Jones')
        info.AddDeveloper('Robert B. Reese')
        info.AddDocWriter('J.W. Bruce')
        info.AddDocWriter('Bryan A. Jones')        
        info.AddArtist('Bob Reese')
        wx.AboutBox(info)    
    
    def OnPortSettings(self, event=None):
        """Show the portsettings dialog. The reader thread is stopped for the
           settings change."""
        if event is not None:           #will be none when called on startup
            self.StopThread()
            self.serial.close()
        ok = False
        # fix the default baudrate for 57k6
        self.serial.baudrate = 57600
        while not ok:
            dialog_serial_cfg = wxSerialConfigDialog.SerialConfigDialog(None, -1, "",
                show=wxSerialConfigDialog.SHOW_BAUDRATE,
                serial=self.serial
            )
            result = dialog_serial_cfg.ShowModal()
            dialog_serial_cfg.Destroy()
            #open port if not called on startup, open it on startup and OK too
            if result == wx.ID_OK or event is not None:
                try:
                    self.serial.open()
                except serial.SerialException, e:
                    dlg = wx.MessageDialog(None, str(e), "Serial Port Error", wx.OK | wx.ICON_ERROR)
                    dlg.ShowModal()
                    dlg.Destroy()
                else:
                    self.StartThread()
                    self.frame_terminal_statusbar.SetStatusText("Opened %s" % (
                        self.serial.portstr,
                        ), 0)
                    self.frame_terminal_statusbar.SetStatusText("%s [%s%s%s]" % (
                        self.serial.baudrate,
                        self.serial.bytesize,
                        self.serial.parity,
                        self.serial.stopbits,
                        ), 1)
                    ok = True
            else:
                #on startup, dialog aborted
                self.alive.clear()
                ok = True

    def OnTermSettings(self, event):
        """Menu point Terminal Settings. Show the settings dialog
           with the current terminal settings"""
        dialog = TerminalSettingsDialog(None, -1, "", settings=self.settings)
        result = dialog.ShowModal()
        dialog.Destroy()
        
    def OnKey(self, event):
        """Key event handler. if the key is in the ASCII range, write it to the serial port.
           Newline handling and local echo is also done here."""
        code = event.GetKeyCode()
        if code < 256:                          #is it printable?
            if code == 13:                      #is it a newline? (check for CR which is the RETURN key)
                if self.settings.echo:          #do echo if needed
                    self.text_ctrl_output.AppendText('\n')
                if self.settings.newline == NEWLINE_CR:
                    self.serial.write('\r')     #send CR
                elif self.settings.newline == NEWLINE_LF:
                    self.serial.write('\n')     #send LF
                elif self.settings.newline == NEWLINE_CRLF:
                    self.serial.write('\r\n')   #send CR+LF
            else:
                char = chr(code)
                if self.settings.echo:          #do echo if needed
                    self.text_ctrl_output.WriteText(char)
                self.serial.write(char)         #send the charcater
                self.log.putInText(char)
        else:
            print "Extra Key:", code

    def OnSerialRead(self, event):
        """Handle input from the serial port."""
        text = event.data
        if self.settings.unprintable:
            text = ''.join([(c >= ' ') and c or '<%d>' % ord(c)  for c in text])
        self.text_ctrl_output.AppendText(text)
        self.log.putOutText(text)

    def ComPortThread(self):
        """Thread that handles the incomming traffic. Does the basic input
           transformation (newlines) and generates an SerialRxEvent"""
        while self.alive.isSet():               #loop while alive event is true
            text = self.serial.read(1)          #read one, with timout
            if text:                            #check if not timeout
                n = self.serial.inWaiting()     #look if there is more to read
                if n:
                    text = text + self.serial.read(n) #get it
                #newline transformation
                if self.settings.newline == NEWLINE_CR:
                    text = text.replace('\r', '\n')
                elif self.settings.newline == NEWLINE_LF:
                    pass
                elif self.settings.newline == NEWLINE_CRLF:
                    text = text.replace('\r\n', '\n')
                event = SerialRxEvent(self.GetId(), text)
                self.GetEventHandler().AddPendingEvent(event)
                #~ self.OnSerialRead(text)         #output text in window
            
# end of class TerminalFrame




class MyApp(wx.App):
    def OnInit(self):
        wx.InitAllImageHandlers()
        frame_terminal = TerminalFrame(None, -1, "")
        self.SetTopWindow(frame_terminal)
        frame_terminal.Show(1)
        return 1

# end of class MyApp

############################################################################
## OTHER STUFF
############################################################################
import  wx
import  wx.html
from    time import localtime, strftime
     
# a quick and dirty class to create a LOG window
# we will use the wx.html.HtmlWindow class to make
# our life easy and create a bit of eye candy
class LogHtmlWindow(wx.Frame):
  IN_TEXT = 0
  OUT_TEXT = 1
  STATUS_TEXT = 2
  
  def __init__(self, parent, title):
    wx.Frame.__init__(self, parent, -1, title)
    self.html = wx.html.HtmlWindow(self)
    self.Bind(wx.EVT_CLOSE, self.OnClose)
    
    if "gtk2" in wx.PlatformInfo:
      self.html.SetStandardFonts()
    self.html.SetPage(
        "<h3>Log started on %s</h3><p>" % 
        strftime("%a, %d %b %Y %H:%M:%S </h1><p>", localtime())
    )
    self.lastWrite = self.IN_TEXT
    self.startInText()

  # don't close the window, just hide it.  The parent will kill
  # us when they close (or they are supposed too)
  def OnClose(self, event):
    self.Hide()
      
  def startInText(self):
    self.html.AppendToPage("<b>")
      
  def endInText(self):    
    self.html.AppendToPage("</b><br>")
    
  def startOutText(self):
    self.html.AppendToPage("<font color=\"green\"><i>")
  
  def endOutText(self):
    self.html.AppendToPage("</i></font><br>")  
  
  def endCurrentText(self):
    if self.lastWrite == self.IN_TEXT:
      self.endInText()
    elif self.lastWrite == self.OUT_TEXT:
      self.endOutText()
    elif self.lastWrite == self.STATUS_TEXT:
      self.endStatusText()
      
  def startStatusText(self):
    self.html.AppendToPage("<font color=\"red\">%s: <b><i>" % 
        (strftime("%a, %d %b %Y %H:%M:%S </h1><p>", localtime()) )
    )
  
  def endStatusText(self):
    self.html.AppendToPage("</i></b></font><br>")
      
      
  # quick method to place INCOMING text into log window
  def putInText(self, s):
    if self.lastWrite != self.IN_TEXT:
      self.endCurrentText()     
    self.lastWrite = self.IN_TEXT
    self.html.AppendToPage("<b>%s</b>" % s )
  
  # quick method to place OUTGOING text into log window
  def putOutText(self, s):
    if self.lastWrite != self.OUT_TEXT:
      self.endCurrentText()
    self.lastWrite = self.OUT_TEXT    
    self.html.AppendToPage("<font color=\"green\"><i>%s</i></font><br>" % s )
    
   # quick method to place STATUS text into log window
  def putStatusText(self, s):
    if self.lastWrite != self.STATUS_TEXT:
      self.endCurrentText()
    self.lastWrite = self.STATUS_TEXT    
    self.html.AppendToPage("<font color=\"red\">%s: <b><i>%s</i></b></font><br>" % 
        (strftime("%a, %d %b %Y %H:%M:%S </h1><p>", localtime()), s)
    )

     
# quick method to place text into the main status text area
def updateStatusBar(f, s):
  f.frame_terminal_statusbar.SetStatusText(s,  0)


############################################################################
if __name__ == "__main__":
    app = MyApp(0)
    app.MainLoop()
