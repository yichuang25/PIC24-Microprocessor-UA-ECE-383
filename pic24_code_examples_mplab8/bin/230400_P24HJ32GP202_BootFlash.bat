@ECHO OFF
COLOR 4f
ECHO PROGRAMMING PIC24HJ32GP202 BOOTFLASH FOR 230400 BAUDRATE
ECHO.
ECHO.
ECHO PK3CMD -P24HJ32GP202 -E -FC:\Micro\hex\p24HJ32GP202_230400baud_bootldr.hex -M -V3.25 -Y 
ECHO  PK3CMD                                             PICkit3 Cmd line file
ECHO -P24HJ32GP202                                       Choose PiC 24HJ32GP202
ECHO -E							 Erase device before programming
ECHO -FC:\Micro\hex\p24HJ32GP202_230400baud_bootldr.hex  Load hex file
ECHO -M                                                  Program  to bootflash
ECHO -V3.30                                              PICkit3 supplies 3.3V  
ECHO -Y                                                  Verify the programing
ECHO.  
ECHO RUNNING COMMAND
CD C:\Program Files\Microchip\MPLAB IDE\Programmer Utilities\PICkit3\
CALL PK3CMD -P24HJ32GP202 -E  -FC:\Micro\hex\p24HJ32GP202_230400baud_bootldr.hex -M -V3.30 -Y 
ECHO.
PAUSE