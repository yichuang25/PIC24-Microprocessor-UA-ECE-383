

require "pic24dev_data.pl";

$rp_max = 100;    #max value for higher end devices



$family_type = 0;   # if 0, then all devices before 24E changes

sub pulldowns_exist {
 if ($family_type == $family_24e || $family_type == $family_33e ) {
   return 1;
   }
   return 0;
}

foreach $devref (@pic24_devices) {
  $devname = $$devref[0];  
  $ioref =$$devref[1];
  $pullref=$$devref[2];
  $odref=$$devref[3];
  $anaref=$$devref[4];
  $rpanaref=$$devref[5];
  $disanaref = $$devref[6];
  $family_type  = -1;
  $family_type = $$devref[7];
  if ($family_type < 0) {
   print "Illegal family!\n";
   exit 0;
   }
  $fname = $devname . "_ports.h";  
  $reverse_anaref = &get_reverse_hash($anaref);
  if (-e $fname) {
   unlink($fname);  
   }
  print "$fname\n";
  open(OUTPUT, ">$fname") || die "Cannot create file $fname";
  
  if ($family_type == $family_24e || $family_type == $family_33e ) {
  #generic way of writing port files. The only arguments used are 
   &printgenericports($anaref, $rpanaref, $family_type);
   &printanalog_disable($disanaref);
  } else {  
    &printpullups($ioref, $pullref);
    if (&pulldowns_exist($family_type)) {
      &printpulldowns($ioref, $pullref);
    }
    &printcninterrupts($ioref, $pullref);
    &printanalog($ioref, $anaref);
    &printopendrain($ioref, $odref);
    &printdigout($ioref, $anaref,$pullref,$odref);
    &printdigoutod($ioref, $anaref,$pullref,$odref);
    &printdigin($ioref, $anaref,$pullref);
    &printrpdigin($rpanaref);
    &printanaout($reverse_anaref);  
    &printanalog_disable($disanaref);
  }
 
  print OUTPUT "#define _PIC24_DIGIO_DEFINED\n";
  close(OUTPUT);
  }
 exit;
 
 
 sub printaport {
 my($anaref,$port,$portnum) = @_;
 my($portname);
 my($anaport);
 
 printf (OUTPUT "#ifdef _R%s%d\n",$port,$portnum);

 printf (OUTPUT "#ifdef _CNPU%s%d\n",$port,$portnum);
printf (OUTPUT "#define ENABLE_R%s%d_PULLUP() _CNPU%s%d = 1\n",$port,$portnum,$port,$portnum);
printf (OUTPUT "#define DISABLE_R%s%d_PULLUP() _CNPU%s%d = 0\n",$port,$portnum,$port,$portnum);
printf (OUTPUT "#endif\n");

printf (OUTPUT "#ifdef _CNPD%s%d\n",$port,$portnum);
printf (OUTPUT "#define ENABLE_R%s%d_PULLDOWN() _CNPD%s%d = 1\n",$port,$portnum,$port,$portnum);
printf (OUTPUT "#define DISABLE_R%s%d_PULLDOWN() _CNPD%s%d = 0\n",$port,$portnum,$port,$portnum);
printf (OUTPUT "#endif\n");

printf (OUTPUT "#ifdef _ANS%s%d\n",$port,$portnum);
printf (OUTPUT "#define ENABLE_R%s%d_ANALOG() _ANS%s%d = 1\n",$port,$portnum,$port,$portnum);
printf (OUTPUT "#define DISABLE_R%s%d_ANALOG() _ANS%s%d = 0\n",$port,$portnum,$port,$portnum);
printf (OUTPUT "#endif\n");

printf (OUTPUT "#ifdef _CNIE%s%d\n",$port,$portnum);
printf (OUTPUT "#define ENABLE_R%s%d_CN_INTERRUPT() _CNIE%s%d = 1\n",$port,$portnum,$port,$portnum);
printf (OUTPUT "#define DISABLE_R%s%d_CN_INTERRUPT() _CNIE%s%d = 0\n",$port,$portnum,$port,$portnum);
printf (OUTPUT "#endif\n");


printf (OUTPUT "static inline void CONFIG_R%s%d_AS_DIG_OUTPUT(){\n",$port,$portnum);
printf (OUTPUT "#ifdef _CNPU%s%d\n",$port,$portnum);
printf (OUTPUT "_CNPU%s%d = 0; //disable pullup\n",$port,$portnum);
printf (OUTPUT "#endif\n");
printf (OUTPUT "#ifdef _CNPD%s%d\n",$port,$portnum);
printf (OUTPUT "_CNPD%s%d = 0; //disable pulldown\n",$port,$portnum);
printf (OUTPUT "#endif\n");
printf (OUTPUT "#ifdef _ODC%s%d\n",$port,$portnum);
printf (OUTPUT "_ODC%s%d = 0;\n",$port,$portnum);
printf (OUTPUT "#endif\n");
printf (OUTPUT "#ifdef _TRIS%s%d\n",$port,$portnum);
printf (OUTPUT " _TRIS%s%d = 0;\n",$port,$portnum);
printf (OUTPUT "#endif\n");
printf (OUTPUT "#ifdef _ANS%s%d\n",$port,$portnum);
printf (OUTPUT " _ANS%s%d = 0;  //disable analog\n",$port,$portnum);
printf (OUTPUT "#endif\n");
printf (OUTPUT "}\n");

printf (OUTPUT "#ifdef _ODC%s%d\n",$port,$portnum);
printf (OUTPUT "#define ENABLE_R%s%d_OPENDRAIN() _ODC%s%d = 1\n",$port,$portnum,$port,$portnum);
printf (OUTPUT "#define DISABLE_R%s%d_OPENDRAIN() _ODC%s%d = 0\n",$port,$portnum,$port,$portnum);
printf (OUTPUT "#define CONFIG_R%s%d_AS_DIG_OD_OUTPUT()  {CONFIG_R%s%d_AS_DIG_OUTPUT();ENABLE_R%s%d_OPENDRAIN();}\n",$port,$portnum,$port,$portnum,$port,$portnum);
printf (OUTPUT "#endif\n");

printf (OUTPUT "static inline void CONFIG_R%s%d_AS_DIG_INPUT(){\n",$port,$portnum);
printf (OUTPUT "#ifdef _CNPU%s%d\n",$port,$portnum);
printf (OUTPUT "_CNPU%s%d = 0; //disable pullup\n",$port,$portnum);
printf (OUTPUT "#endif\n");
printf (OUTPUT "#ifdef _CNPD%s%d\n",$port,$portnum);
printf (OUTPUT "_CNPD%s%d = 0; //disable pulldown\n",$port,$portnum);
printf (OUTPUT "#endif\n");
printf (OUTPUT "#ifdef _TRIS%s%d\n",$port,$portnum);
printf (OUTPUT " _TRIS%s%d = 1;\n",$port,$portnum);
printf (OUTPUT "#endif\n");
printf (OUTPUT "#ifdef _ANS%s%d\n",$port,$portnum);
printf (OUTPUT " _ANS%s%d = 0;  //disable analog\n",$port,$portnum);
printf (OUTPUT "#endif\n");
printf (OUTPUT "}\n");


$portname = sprintf("R%s%d",$port,$portnum);
if (exists($$anaref{$portname})) {
   $anaport = $$anaref{$portname};
   printf(OUTPUT "#define CONFIG_%s_AS_DIGITAL() _ANS%s%d = 0\n",$anaport,$port,$portnum);
   printf(OUTPUT "#define CONFIG_%s_AS_ANALOG() _ANS%s%d = 1\n",$anaport,$port,$portnum);
}
printf (OUTPUT "#endif  //endif #ifdef _R%s%d\n",$port,$portnum);
 
 }
 
 sub printgenericports {
 
 my ($anaref, $rpanaref, $family_type) = @_;
 
 my @ports = ('A','B','C','D','E','F','G');
 my $portname;
 
 foreach $port (@ports) {
    for ($i=0;$i<= 15;$i++) {
	  &printaport($anaref,$port,$i);	  
	}
 
 }
 
  &printrpdigin($rpanaref);
  &printanaout($reverse_anaref);  
 
 }
 
  
  # creating mapping of analog ports to 
 sub get_reverse_hash {
 my ($fwdhashref) = @_;
 my $revhashref;
 my $fwdkey, $fwdval;
  
 #init hash
 
 $revhashref = { };
 foreach $fwdkey (keys(%$fwdhashref)) {
  $fwdval = $$fwdhashref{$fwdkey};
  $$revhashref{$fwdval} = $fwdkey;
 }
 return $revhashref;  
 }
 
 
 &printanalog_disable($disanaref);
 
 sub printanalog_disable{
 my($hashref) = @_;
 my $fwdkey, $fwdval;
 my $newkey;
 
 foreach $fwdkey (keys(%disable_analog_master_list)) {
  $fwdval = $disable_analog_master_list{$fwdkey};
  if (exists($$hashref{$fwdkey})) {
   $newkey = $$hashref{$fwdkey};
   if ($newkey ne "") {
     printf (OUTPUT "#define DISABLE_%s_ANALOG()   CONFIG_%s_AS_DIGITAL() \n",$fwdkey, $newkey);
	 } 
   else {
	 printf (OUTPUT "#define DISABLE_%s_ANALOG()   \n",$fwdkey);
	 }	 
  } 
  else {
   printf (OUTPUT "#define DISABLE_%s_ANALOG()   \n",$fwdkey);
  }
  }
 
 }
 
 sub printanaout {
  my($revhashref) = @_;
  my($i);
  my($pcfg);
  my($digport);
  
  if ($family_type == $family_24e || $family_type == $family_33e) {
    return;
  }
  
  for ($i=0;$i<= 31;$i++) {
    $pcfg = "_PCFG" . $i;
    print OUTPUT "#ifdef $pcfg\n";
    printf (OUTPUT "static inline void CONFIG_AN%d_AS_ANALOG(){\n",$i);
	
	if (exists($$revhashref{$pcfg})) {
	  $digport = $$revhashref{$pcfg};
	  printf (OUTPUT "  CONFIG_%s_AS_DIG_INPUT();\n",$digport);
	}
	printf (OUTPUT "  %s = 0;\n",$pcfg);
	print OUTPUT "}\n"; 
	printf (OUTPUT "static inline void CONFIG_AN%d_AS_DIGITAL(){\n",$i);
	printf (OUTPUT "  %s = 1;\n",$pcfg);
	print OUTPUT "}\n"; 	
    print OUTPUT "#endif\n";  
  }
 }
 
 sub printrpdigin {
 my ($href) = @_;
 my ($i);
 
  for($i=0;$i<$rp_max;$i++){
  
   if (exists($$href{$i})){
     printf(OUTPUT "#define CONFIG_RP%d_AS_DIG_PIN()  %s = 1\n",$i,$$href{$i});
   } else {
      printf(OUTPUT "#define CONFIG_RP%d_AS_DIG_PIN() \n", $i);
     }
  
   }  
 
 }
 
sub printdevice {
  my ($lref, $ioref, $pullref, $odref, $anaref, $rpanaref) = @_;
  my ($device);
  
  print(OUTPUT "#if ");   
 foreach $device (@$lref){
 
 printf(OUTPUT " defined(%s) || ",$device);  
  
  }
 print(OUTPUT " 0 \n");
  
 &printpullups($ioref, $pullref);
 &printcninterrupts($ioref, $pullref);
 &printopendrain($ioref, $odref);
 &printdigout($ioref, $anaref);
 &printdigin($ioref, $anaref);
 &printrpdigin($rpanaref);
  
 print(OUTPUT "#define _PIC24_DIGIO_DEFINED\n");
 print(OUTPUT "#endif\n");
 
 

}


sub printdigout {

 my ($lref, $href, $pullref,$odref) = @_;
 my ($v);
 
  #do output macros   
 foreach $port (@$lref) {  
    $v = substr $port,1;
	$v = "_TRIS" ."$v";
	printf (OUTPUT "static inline void CONFIG_%s_AS_DIG_OUTPUT(){\n",$port);
	if (exists($$pullref{$port})) {
	printf (OUTPUT " DISABLE_%s_PULLUP();\n", $port);
	}
	if (&pulldowns_exist($family_type) && exists($$pullref{$port})) {
	printf (OUTPUT " DISABLE_%s_PULLDOWN();\n", $port);
	}
	
	
	if (exists($$odref{$port})) {
	printf (OUTPUT " DISABLE_%s_OPENDRAIN();\n", $port);
	}
	printf (OUTPUT " %s = 0;\n",$v);
    if (exists($$href{$port})) {
	 if ($family_type == $family_24e || $family_type == $family_33e) {
	  $v = substr $port,1;
	  $v = "_ANS" ."$v";
	 } else {
	  $v = $$href{$port};
	 }	 
	 printf (OUTPUT " %s = 1;\n",$v);
	}	
	printf (OUTPUT "}\n");   
 } 
 }
 
 sub printdigoutod {

 my ($lref, $href, $pullref,$odref) = @_;
 my ($v);
 
  #do output macros   
 foreach $port (@$lref) {  
    $v = substr $port,1;
	$v = "_TRIS" ."$v";
	printf (OUTPUT "static inline void CONFIG_%s_AS_DIG_OD_OUTPUT(){\n",$port);
	if (exists($$pullref{$port})) {
	printf (OUTPUT " DISABLE_%s_PULLUP();\n", $port);
	}
	if (&pulldowns_exist($family_type) && exists($$pullref{$port})) {
	printf (OUTPUT " DISABLE_%s_PULLDOWN();\n", $port);
	}
	if (exists($$odref{$port})) {
	printf (OUTPUT " ENABLE_%s_OPENDRAIN();\n", $port);
	}
	printf (OUTPUT " %s = 0;\n",$v);
    
	if (exists($$href{$port})) {
	 if ($family_type == $family_24e || $family_type == $family_33e) {
	  $v = substr $port,1;
	  $v = "_ANS" ."$v";
	 } else {
	  $v = $$href{$port};
	 }
	 printf (OUTPUT " %s = 1;\n",$v);
	}	
	printf (OUTPUT "}\n");   
 } 
 }
 
 sub printdigin {

 my ($lref, $href,$pullref) = @_;
 my ($v);
 foreach $port (@$lref) {
    $v = substr $port,1;
	$v = "_TRIS" ."$v";
    printf (OUTPUT "static inline void CONFIG_%s_AS_DIG_INPUT(){\n",$port);
	if (exists($$pullref{$port})) {
	printf (OUTPUT " DISABLE_%s_PULLUP();\n", $port);
	}
	if (&pulldowns_exist($family_type) && exists($$pullref{$port})) {
	printf (OUTPUT " DISABLE_%s_PULLDOWN();\n", $port);
	}
	
	printf (OUTPUT " %s = 1;\n",$v);
	if (exists($$href{$port})) {
	 if ($family_type == $family_24e || $family_type == $family_33e) {
	  $v = substr $port,1;
	  $v = "_ANS" ."$v";
	 } else {
	  $v = $$href{$port};
	  }
	 printf (OUTPUT " %s = 1;\n",$v);
	}
	printf (OUTPUT "}\n");
 
 } 
 
 }


sub getPortName {
my ($portName) = @_;
my ($val) = substr $portName,1;
return $val;
}


sub printpullups {

 my ($lref, $href) = @_;
 my ($v);
 my ($pn);
 
 
 foreach $port (@$lref) {
  if (exists($$href{$port})) {
    $v = $$href{$port};
	if ($family_type == $family_24e || $family_type == $family_33e) {
	 $pn = &getPortName($port);
	 printf (OUTPUT "#define ENABLE_%s_PULLUP() _CNPU%s = 1\n",$port,$pn);
     printf (OUTPUT "#define DISABLE_%s_PULLUP() _CNPU%s = 0\n",$port,$pn);  
	
	} else {
	 printf (OUTPUT "#define ENABLE_%s_PULLUP() %sPUE = 1\n",$port,$v);
     printf (OUTPUT "#define DISABLE_%s_PULLUP() %sPUE = 0\n",$port,$v);  
	}
   } 
 }
 }
 
 sub printpulldowns {

 my ($lref, $href) = @_;
 my ($v);
 my ($pn);
 
 
 foreach $port (@$lref) {
  if (exists($$href{$port})) {
    $v = $$href{$port};
	if ($family_type == $family_24e || $family_type == $family_33e) {
	 $pn = &getPortName($port);
	 printf (OUTPUT "#define ENABLE_%s_PULLDOWN() _CNPD%s = 1\n",$port,$pn);
     printf (OUTPUT "#define DISABLE_%s_PULLDOWN() _CNPD%s = 0\n",$port,$pn);  
	
	}	
   } 
 }
 }
 
 sub printcninterrupts {

 my ($lref, $href) = @_;
 my ($v);
 
 foreach $port (@$lref) {
  if (exists($$href{$port})) {
    $v = $$href{$port};
	if ($family_type == $family_24e || $family_type == $family_33e) {
	 $pn = &getPortName($port);
	 printf (OUTPUT "#define ENABLE_%s_CN_INTERRUPT() _CNIE%s = 1\n",$port,$pn);
     printf (OUTPUT "#define DISABLE_%s_CN_INTERRUPT() _CNIE%s = 0\n",$port,$pn); 	
	} else {
	printf (OUTPUT "#define ENABLE_%s_CN_INTERRUPT() %sIE = 1\n",$port,$v);
    printf (OUTPUT "#define DISABLE_%s_CN_INTERRUPT() %sIE = 0\n",$port,$v);  
   } 
   }
 }
 }
 
 sub printanalog {

 my ($lref, $href) = @_;
 my ($v);
 my ($tmp);
 
 foreach $port (@$lref) {
  if (exists($$href{$port})) {
    if ($family_type == $family_24e || $family_type == $family_33e) {
	  $v = substr $port,1;
	  $v = "_ANS" ."$v";	
	  printf (OUTPUT "#define DISABLE_%s_ANALOG() %s = 1\n",$port,$v);
      printf (OUTPUT "#define ENABLE_%s_ANALOG() %s = 0\n",$port,$v);
      $tmp = $$href{$port};
	  printf (OUTPUT "#define CONFIG_%s_AS_ANALOG() %s = 0\n",$tmp,$v);
	  printf (OUTPUT "#define CONFIG_%s_AS_DIGITAL() %s = 1\n",$tmp,$v);
	} else {
      $v = $$href{$port};	
	  printf (OUTPUT "#define DISABLE_%s_ANALOG() %s = 1\n",$port,$v);
      printf (OUTPUT "#define ENABLE_%s_ANALOG() %s = 0\n",$port,$v);  
	}
   } 
 }
 }
 
 sub printopendrain {

 my ($lref, $href) = @_;
 my ($v);
 
 foreach $port (@$lref) {
  if (exists($$href{$port})) {
    $v = $$href{$port};
	printf (OUTPUT "#define ENABLE_%s_OPENDRAIN() %s = 1\n",$port,$v);
   printf (OUTPUT "#define DISABLE_%s_OPENDRAIN() %s = 0\n",$port,$v);  
   } 
 }
 }
 
 
 



