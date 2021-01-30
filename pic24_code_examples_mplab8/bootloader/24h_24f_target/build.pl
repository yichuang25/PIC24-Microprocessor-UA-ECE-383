
use ActiveState::Run qw(run);
use File::Copy;

#230400 baudrate
&do_compile("SConstruct","p",["DEVICE=24FJ32GA002","DEFAULT_BAUDRATE=230400"],"_230400baud_bootldr");
&do_compile("SConstruct","p",["DEVICE=24FJ64GA002","DEFAULT_BAUDRATE=230400"],"_230400baud_bootldr");
&do_compile("SConstruct","p", ["DEVICE=24HJ32GP202","DEFAULT_BAUDRATE=230400"],"_230400baud_bootldr");
&do_compile("SConstruct","p",["DEVICE=24HJ64GP502","DEFAULT_BAUDRATE=230400"],"_230400baud_bootldr");

#explorer16_100p
&do_compile("SConstruct","explorer16_100p_p", ["DEVICE=24FJ128GA010", "DEFAULT_BAUDRATE=57600","CFLAG1=EXPLORER16_100P"],"_57600baud_bootldr");
&do_compile("SConstruct","explorer16_100p_p", ["DEVICE=24HJ256GP610", "DEFAULT_BAUDRATE=57600","CFLAG1=EXPLORER16_100P"],"_57600baud_bootldr");



## 28-pin devices
&do_compile("SConstruct","p", ["DEVICE=24HJ32GP202","DEFAULT_BAUDRATE=57600"],"_57600baud_bootldr" );
&do_compile("SConstruct","p",["DEVICE=24FJ32GA002","DEFAULT_BAUDRATE=57600"],"_57600baud_bootldr");
&do_compile("SConstruct","p",["DEVICE=24FJ64GA002","DEFAULT_BAUDRATE=57600"],"_57600baud_bootldr");
&do_compile("SConstruct","p",["DEVICE=24FJ32GA102","DEFAULT_BAUDRATE=57600"],"_57600baud_bootldr");
&do_compile("SConstruct","p",["DEVICE=24FJ64GA102","DEFAULT_BAUDRATE=57600"],"_57600baud_bootldr");
&do_compile("SConstruct","p",["DEVICE=24FJ64GB002","DEFAULT_BAUDRATE=57600"],"_57600baud_bootldr");
&do_compile("SConstruct","p",["DEVICE=24HJ12GP202","DEFAULT_BAUDRATE=57600"],"_57600baud_bootldr");
&do_compile("SConstruct","p",["DEVICE=24HJ64GP502","DEFAULT_BAUDRATE=57600"],"_57600baud_bootldr");
&do_compile("SConstruct","p",["DEVICE=24HJ128GP502","DEFAULT_BAUDRATE=57600"],"_57600baud_bootldr");
&do_compile("SConstruct","p",["DEVICE=24FJ64GB004","DEFAULT_BAUDRATE=57600"],"_57600baud_bootldr");
&do_compile("SConstruct","p",["DEVICE=24EP64GP202","DEFAULT_BAUDRATE=57600"],"_57600baud_bootldr");


## misc
&do_compile("SConstruct","p",["DEVICE=24HJ256GP206","DEFAULT_BAUDRATE=57600"],"_57600baud_bootldr");
&do_compile("SConstruct","p",["DEVICE=24FJ64GA006","DEFAULT_BAUDRATE=57600"],"_57600baud_bootldr");
&do_compile("SConstruct","p",["DEVICE=24FJ128GA006","DEFAULT_BAUDRATE=57600"],"_57600baud_bootldr");
&do_compile("SConstruct","p",["DEVICE=24HJ128GP506","DEFAULT_BAUDRATE=57600"],"_57600baud_bootldr");
&do_compile("SConstruct","p",["DEVICE=24FJ64GB004","DEFAULT_BAUDRATE=57600"],"_57600baud_bootldr");
&do_compile("SConstruct","p",["DEVICE=33FJ32GP202","DEFAULT_BAUDRATE=57600"],"_57600baud_bootldr");
&do_compile("SConstruct","p",["DEVICE=33FJ128GP802","DEFAULT_BAUDRATE=57600"],"_57600baud_bootldr");
&do_compile("SConstruct","p",["DEVICE=24F16KA102","DEFAULT_BAUDRATE=57600"],"_57600baud_bootldr");





exit;



sub do_compile {
 my($sfile,$prefix,$lref,$postfix) = @_;
 my($cmd);
 my($hexfile);
 my($flag);
 my (@words);
 my ($dev);
 
 $cmd = "scons -f ". $sfile;
 foreach $flag (@$lref) {
   $cmd = $cmd . " " . $flag;
   @words = split("=",$flag);
   if ($words[0] eq "DEVICE") {
       $dev = $words[1];
     }
   }
 run($cmd);
 $cmd = "pic30-bin2hex.exe main.cof";
 run($cmd);
 $hexfile = $prefix . $dev . $postfix . ".hex";
 copy("main.hex",$hexfile);
 run("scons -c");
 }
 
 