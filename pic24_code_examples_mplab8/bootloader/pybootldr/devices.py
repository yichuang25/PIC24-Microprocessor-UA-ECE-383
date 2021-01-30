#! /usr/bin/env python

import  fcns

class DevicesFile:
  
  def __init__(self, fstr):
    self.filename = fstr
    self.d = self.readDevicesFile(fstr)
    
  # read the devices.txt file and return a dictionary
  def readDevicesFile( self, fstr ):
    lines = fcns.openfile( fstr, mode = "r" )
    d = {}
    for e in lines:
        dlist = e.split(",")
        if (dlist[0][0] != "#"):
          d[dlist[0].upper()] = dlist[1:]
    return d
  
  def getDeviceFromDeviceID(self, devID):
    for e in self.d:
      if self.getDeviceID(e).upper() == devID.upper():
        return e
    
  def getDeviceID(self, devString):
    return self.d[devString.upper()][0]
  
  def getProcessID(self, devString):
    return int(self.d[devString.upper()][1])
  
  def getFamilyName(self, devString):
    return self.d[devString.upper()][2]
  
  def getConfigHexAddress(self, devString):
    return self.d[devString.upper()][3]
  
  def hasSmallRAM(self, devString):
    if self.d[devString.upper()][4] == "1":
      return True
    else:
      return False
  
  def test_openfile():
    """
    Test function for trying out our code shenanigans before using them in
    the actual application.
    """

    lines = openfile("./devices.txt")
    string = stringify(lines)
    print string

  if __name__ == '__main__':
    test_openfile()