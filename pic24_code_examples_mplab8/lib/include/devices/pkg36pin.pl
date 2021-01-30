###package data

## 36 pin package data, variant 1
##
# also dsPIC33E/24E 28-pin packages.


@digio_36pin_1 = ("RA0", "RA1","RA2", "RA3", "RA8",
"RB0", "RB1", "RB2", "RB3", "RB4", "RB5",
"RB6","RB7", "RB8", "RB9", "RB10", "RB11",
"RB12", "RB13", "RB14", "RB15", "RC0","RC1");



%pullups_36pin_1 = (
   "RA0" => "",
   "RA1" => "",
   "RA2" => "",
   "RA3" => "",
   "RA4" => "",
   "RA8" => "",
   "RB0" => "",
   "RB1" => "",
   "RB2" => "",
   "RB3" => "",
   "RB4" => "",
   "RB5" => "",
   "RB6" => "",
   "RB7" => "",
   "RB8" => "",
   "RB9" => "",
   "RB10" => "",
   "RB11" => "",
   "RB12" => "",
   "RB13" => "",
   "RB14" => "",
   "RB15" => "",
   "RC0"  => "",
   "RC1"  => ""   
   );
 
  
    
 %opendrain_36pin_24H_1 = (
   "RA0" => "_ODCA0",
   "RA1" => "_ODCA1",
   "RA2" => "_ODCA2",
   "RA3" => "_ODCA3",
   "RA4" => "_ODCA4",
   "RA8" => "_ODCA8",
   "RB0" => "_ODCB0",
   "RB1" => "_ODCB1",
   "RB2" => "_ODCB2",
   "RB3" => "_ODCB3",
   "RB4" => "_ODCB4",
   "RB5" => "_ODCB5",
   "RB6" => "_ODCB6",
   "RB7" => "_ODCB7",
   "RB8" => "_ODCB8",
   "RB9" => "_ODCB9",
   "RB10" => "_ODCB10",
   "RB11" => "_ODCB11",
   "RB12" => "_ODCB12",
   "RB13" => "_ODCB13",
   "RB14" => "_ODCB14",
   "RB15" => "_ODCB15",
   "RC0"  => "_ODCC0",
   "RC1"  => "_ODCC1"      
   );

  
    
   %analog_36pin_24E_1 = (
   "RA0" => "AN0",
   "RA1" => "AN1",
   "RB0" => "AN2",
   "RB1" => "AN3",
   "RB2" => "AN4",
   "RB3" => "AN5",
   "RC0" => "AN6",
   "RC1" => "AN7",
   );
   
   %RPanalog_36pin_24E_1 = (
   32 => "_ANSB0",
   33 => "_ANSB1",
   34 => "_ANSB2",
   35 => "_ANSB3"
   );
   

   
1;
 
   
   
   