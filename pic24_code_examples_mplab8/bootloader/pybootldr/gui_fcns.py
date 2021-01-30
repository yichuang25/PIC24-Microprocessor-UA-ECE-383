#!/usr/bin/env python
#
# gui_fcns.py --> gui setup and helper functions for pybootr.py

import  wx
import  wxSerialConfigDialog
import  wx.html

from    LogHtmlWindow import *

import  os
import  serial
import  threading
import  subprocess

import  fcns
import  pickit2

#----------------------------------------------------------------------
# Create an own event type, so that GUI updates can be delegated
# this is required as on some platforms only the main thread can
# access the GUI without crashing. wxMutexGuiEnter/wxMutexGuiLeave
# could be used too, but an event is more elegant.

SERIALRX = wx.NewEventType()
# bind to serial data receive events
EVT_SERIALRX = wx.PyEventBinder(SERIALRX, 0)

class SerialRxEvent(wx.PyCommandEvent):
    eventType = SERIALRX
    def __init__(self, windowID, data):
        wx.PyCommandEvent.__init__(self, self.eventType, windowID)
        self.data = data

    def Clone(self):
        self.__class__(self.GetId(), self.data)

def getOurPicKit2():
  global PICKIT2
  try:
    PICKIT2
  except NameError:
    # pk2 does not exist
    PICKIT2 = pickit2.PicKit2()
  return PICKIT2
  
# gui version to program a hex file providing a progress dialog
def guiProgressHexFile(f, filename, read=False):
  PICKIT2 = getOurPicKit2()
  
  device = PICKIT2.getConnectedDevice()  
  devstr = "-P"+device  
  cmdstr = "pk2cmd " + devstr + " " + " -F" + filename + " -M"

  shortFileName = os.path.basename(filename)
  if not read:
    progstr = "Programming %s\n    into\n%s\n\nPlease be patient.\nThis can take some time."
    cmdstr = "pk2cmd " + devstr + " " + " -F" + filename + " -M"
  else:
    progstr = "Filling %s\n   with data from\n%s\n\nPlease be patient.\nThis can take some time."
    cmdstr = "pk2cmd " + devstr + " " + " -GF" + filename

  dialog = wx.ProgressDialog ( 'Progress',
         progstr % (shortFileName,device),
         maximum=100, style = wx.PD_APP_MODAL | wx.PD_ELAPSED_TIME )
  proc = subprocess.Popen(cmdstr, shell=True, stdout=subprocess.PIPE)
  i=0
  while (proc.poll() is None):
    o = proc.stdout.read(1)
    i=i+1
    dialog.Update(i, progstr % (shortFileName,device))
    
  PICKIT2.refresh()  
  dialog.Destroy()



