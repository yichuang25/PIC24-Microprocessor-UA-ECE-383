#!/usr/bin/env python
#
# python object to control the PicKit2 device
# from Microchip
#
# relies on the pk2cmd utility available from www.microchip.com
#

import    os
import    subprocess
import    re
import    devices

reDeviceID = re.compile("\s*Device\s*ID\s*=\s*(?P<id>[0-9A-F]{4})")

#-A Vdd setpoint exceeds maximum for this device of 3.6V
reMaxVolt  = re.compile(".*maximum.*\sof\s(?P<MaxV>\d.\d)V")

#-A Vdd setpoint below PICkit 2 minimum of 2.5V
reMinVolt  = re.compile(".*minimum.*\sof\s(?P<MinV>\d.\d)V")
  
  
class PicKit2:
    """Initiate communications with the attached PicKit2
        First find the pk2cmd utility, connect to pickit2
        and save some useful info about our setup.
       """
    def __init__(self):
      self.__device = None
      self.__deviceID = None
      self.__lastCmd = None
      self.__lastStatus = None
      self.__lastStatusMsg = None
      self.__powerTarget = False
      self.__holdInReset = True
      self.__voltage = 3.3
      self.__minVoltage = None
      self.__maxVoltage = None
      self.__queryDevice()
      
    def __queryDevice(self):
      r = self.__doCmd("-I", device="PIC24HJ32GP202")
      for line in r:
        m = reDeviceID.match(line)
        if m is not None:
          self.__deviceID = m.groupdict()['id']
          print "PicKit2.queryDevice FOUND DeviceID = ", self.__deviceID
          self.devicesCatalog = devices.DevicesFile('./devices.txt')
          self.__device = self.devicesCatalog.getDeviceFromDeviceID(self.__deviceID)
          # Now, see if we can determine the PicKit2 min/max voltages
          # for this devices
          
          # first, request Vdd=100V. Should be too big for anything
          r = self.__doCmd("-A100")
          for line in r:
            m = reMaxVolt.match(line)
            if m is not None:
              self.__maxVoltage = float(m.groupdict()['MaxV'])
              print "PicKit2.queryDevice FOUND maxVoltage = ", self.__maxVoltage
              break
               
          # Now, request Vdd=1V. Should be too small for anything
          r = self.__doCmd("-A1")
          for line in r:
            m = reMinVolt.match(line)
            if m is not None:
              self.__minVoltage = float(m.groupdict()['MinV'])
              print "PicKit2.queryDevice FOUND minVoltage = ", self.__minVoltage
              break
          
          # return the found device to the caller
          return self.__device
      return None
      
    def doCmd(self, cmdstr):
      return self.__doCmd(cmdstr, device="PIC24HJ32GP202")
      
    def __doCmd(self, cmdstr, device=None):
      flags = ""
      if self.isPowered():
        flags = flags + " -T -A" + str(self.__voltage)
      if self.isHoldInReset() is False:
        flags = flags + " -R"
        
      if device is None:
        device = "-P" + self.__device
      else:
        device = "-P" + device
      
      self.__lastCmd = "pk2cmd " + device + " " + flags + " " + cmdstr  
      print "PicKit2.doCmd:  "+ self.__lastCmd
      fcmd = subprocess.Popen(self.__lastCmd, shell=True, stdout=subprocess.PIPE).stdout
      r = []
      for line in fcmd.readlines():
        #print '  ',line
        r.append(line)
  
      retval = fcmd.close()
   
      if retval is None:
        retval = 0

      retnum = (retval & 0xFF00)>>8
      #print "pk2cmd returned ", retval, retnum            
      
      self.__lastStatus = retnum
      if (retnum==0):
        self.__lastStatusMsg = "Success"
      elif (retnum==5):
        self.__lastStatusMsg = "pk2cmd: Vdd or Vpp voltage error"
      elif (retnum==7):
        self.__lastStatusMsg =  "pk2cmd: Operation failed.  No detailed failure info available."
      elif (retnum==10):
        self.__lastStatusMsg =  "pk2cmd: No programmer found"
      elif (retnum==11):
        self.__lastStatusMsg =  "pk2cmd: Wrong programmer firmware version"
      elif (retnum==15):
        self.__lastStatusMsg =  "pk2cmd: File could not be opened"
      elif (retnum==24):
        self.__lastStatusMsg =  "pk2cmd: PK2DeviceFile.dat could not be found or opened."
      elif (retnum==28):
        self.__lastStatusMsg =  "pk2cmd: Firmware update failed"
      elif (retnum==34):
        self.__lastStatusMsg =  "pk2cmd: Program or verify operation failed"
      elif (retnum==36):
        self.__lastStatusMsg =  "pk2cmd: Invalid command line arguments"
      elif (retnum==37):
        self.__lastStatusMsg =  "pk2cmd: Invalid .HEX file"
      else:
        self.__lastStatusMsg =  "pk2cmd: UNKNOWN RETURN STATUS"
      return r
                    
    def getLastCommand(self):
      return self.__lastCmd
    
    def getLastStatus(self):
      return self.__lastStatus
    
    def getLastStatusMsg(self):
      return self.__lastStatusMsg
    
    def getConnectedDevice(self):
      if self.__device is None:
        return "Unknown."
      else:
        return self.__device
    
    def getConnectedDeviceID(self):
      if self.__device is None:
        return "Unknown."
      else:
        return self.__deviceID
    
    def getQueriedDevice(self):
      return self.__queryDevice()
    
    def getVoltage(self):
      return self.__voltage

    def getMaxVoltage(self):
      return self.__maxVoltage

    def getMinVoltage(self):
      return self.__minVoltage

    def setVoltage(self, v):
      if (v <= self.__maxVoltage) and (v >= self.__minVoltage):
        if (v != self.__voltage):
          self.__voltage = v
          if self.isPowered():
            self.refresh()
      
    def setPowered(self, b):
      if b:
        if not self.__powerTarget:
          self.__powerTarget = True
          self.refresh()
      else:
        if self.__powerTarget:
          self.__powerTarget = False
          self.refresh()
    
    def setHoldInReset(self, b):
      if b:
        if not self.__holdInReset:
          self.__holdInReset = True
          self.refresh()
      else:
        if self.__holdInReset:
          self.__holdInReset = False
          self.refresh()
    
    def isPowered(self):
      return self.__powerTarget
    
    def isHoldInReset(self):
      return self.__holdInReset

    # does a device query, but will put the voltage/MCLR
    # into the correct state
    def refresh(self):
      self.__doCmd("-I")

    def run(self):
      if not self.__powerTarget or self.__holdInReset:
        self.__powerTarget = True
        self.__holdInReset = False
        self.refresh()
      
    def halt(self):
      if (~(self.__powerTarget & self.__holdInReset)):
        self.__powerTarget = True
        self.__holdInReset = True
        self.refresh()
    
    def programHexFile(self, fstr):
      if os.path.isfile(fstr):
        self.__doCmd("-F" + fstr + " -M")
