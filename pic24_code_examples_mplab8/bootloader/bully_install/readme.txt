



http://themech.net/2008/08/how-to-create-a-simple-msi-installer-using-wix/

To build, install Wix 3.0 or later

then:
c:\Users\Robert B. Reese\Documents\zane_project\bully_install>candle bully.wxs
Microsoft (R) Windows Installer Xml Compiler version 3.0.4721.0
Copyright (C) Microsoft Corporation. All rights reserved.

bully.wxs
c:\Users\Robert B. Reese\Documents\zane_project\bully_install\bully.wxs(21) : warning CNDL1113 :
onent 'BullyBootloaderEXE' rather than parent file 'MainExecutable'. To eliminate this warning, y
yBootloaderEXE', (2) make file 'MainExecutable' the keypath of component 'BullyBootloaderEXE', or

c:\Users\Robert B. Reese\Documents\zane_project\bully_install>

c:\Users\Robert B. Reese\Documents\zane_project\bully_install>light.exe bully.wixobj -ext WixUIExtension
Microsoft (R) Windows Installer Xml Linker version 3.0.4721.0
Copyright (C) Microsoft Corporation. All rights reserved.
