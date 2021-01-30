# Documentation for this file. If the \file tag isn't present,
# this file won't be documented.
## \file
#  Given a build environment, this file performs specific builds
#  using the PIC24 library. It should be called from an SCons script
#  that exports:
#  - env: the build environment to use
#  - buildTargets: a list of strings specifying what to build
#    (see if statements toward the end of this file).
#
#  This file delegates to templates/SConscript.py to build all
#  templates.


import os
Import('buildTargets env')


## @{
#  \name Setup for building files based on the PIC24 support library
###############################################################################


## Common sources used for the PIC24 support library
PIC24SupportLibSources = [ 
  'lib/common/dataXfer.c',
  'lib/common/dataXferImpl.c',
  'lib/common/pic24_adc.c',
  'lib/common/pic24_clockfreq.c',
  'lib/common/pic24_configbits.c',
  'lib/common/pic24_ecan.c',
  'lib/common/pic24_flash.c',
  'lib/common/pic24_i2c.c',
  'lib/common/pic24_serial.c',
  'lib/common/pic24_spi.c',
  'lib/common/pic24_timer.c',
  'lib/common/pic24_uart.c',
  'lib/common/pic24_util.c' ]
  
PIC24SupportLibSourcesSmall = [ 
  'lib/common/pic24_clockfreq.c',
  'lib/common/pic24_configbits.c',
  'lib/common/pic24_serial.c',
  'lib/common/pic24_spi.c',
  'lib/common/pic24_timer.c',
  'lib/common/pic24_uart.c',
  'lib/common/pic24_util.c' ] 
  
 
PIC24SupportLibSourcesSmall3 = [ 
  'lib/common/pic24_clockfreq.c',
  'lib/common/pic24_configbits.c',
  'lib/common/pic24_serial.c',
  'lib/common/pic24_spi.c',
  'lib/common/pic24_i2c.c',
  'lib/common/pic24_adc.c',
  'lib/common/pic24_timer.c',
  'lib/common/pic24_uart.c',
  'lib/common/pic24_util.c' ] 


PIC24SupportLibSourcesSmall2 = [ 
  'lib/common/pic24_clockfreq.c',
  'lib/common/dataXfer.c',
  'lib/common/dataXferImpl.c',
  'lib/common/pic24_configbits.c',
  'lib/common/pic24_serial.c',
  'lib/common/pic24_spi.c',
  'lib/common/pic24_i2c.c',
  'lib/common/pic24_adc.c',
  'lib/common/pic24_timer.c',
  'lib/common/pic24_uart.c',
  'lib/common/pic24_util.c' ] 




# Inform SCons about the dependencies in the template-based files
SConscript('./templates/SConscript.py', 'env')

# Inform SCons about the dependencies in the ESOS tree
#SConscript('./esos/SConscript.py', 'env')

## @}

## @{ 
#  \name Example code build
###############################################################################

## This functions converts a cof to a hex file.
#  \param cofName The name of the .c or .cof file to be converted (.c files
#    are assumed to be compiled to .cof elsewhere)
#  \param buildEnvinonment An Environment in which to build these sources.
#  \param aliasString A string to serve as an alias for this build.
def cof2hex(cofName, buildEnvironment, aliasString):
  f = os.path.splitext(cofName)[0]
  myHex = buildEnvironment.Hex(f, f)
  # Add this hex file to a convenient alias
  buildEnvironment.Alias(aliasString, myHex)

## This function builds a program which includes the PIC24 library.
#  \param sourceFileList A list of source files to be built into one program.
#  \param commonSources  A list of source files upon which all sources
#                        in the sourceFileList depend. Wildcards are not
#                        supported.
#  \param buildEnvinonment An Environment in which to build these sources.
#  \param aliasString    A string to serve as an alias for this build.
def buildProgramWithCommonSources(sourceFileList, commonSources, buildEnvironment,
                                  aliasString):
  be = buildEnvironment
  be.Program(sourceFileList + commonSources)
  # Pick the name of the target to be the first c file in the list
  cof2hex(sourceFileList[0], be, aliasString)

## This function takes a list of source files (including wildcards), 
#  adds the PIC24 common
#  sources to each item, then uses Program to build each item.
#  \param sourceFileList A list of source files (which may include
#                        wildcards) to be built.
#  \param commonSources  A list of source files upon which all sources
#                        in the sourceFileList depend. Wildcards are not
#                        supported.
#  \param buildEnvinonment An Environment in which to build these sources.
#  \param exceptionDict A exceptionFileName={key=value pairs used to create the
#                          custom environment correpsonding to the
#                          exceptionFile} dict
#  \param aliasString    A string to serve as an alias for this set of
#                        builds.
def buildWithCommonSources(sourceFileList, commonSources, buildEnvironment,
                           exceptionDict, aliasString):
  for sourceFileGlob in sourceFileList:
    for sourceFile in Glob(sourceFileGlob, True, True, True):
        # See if this is an exception
        if sourceFile in exceptionDict:
          # Yes, so modify environment with dict of changes.
          be = buildEnvironment.Clone()
          flags = exceptionDict[sourceFile]
#          print flags
#          be.MergeFlags(flags) # produces weird
          # errors, so hand-code a simple alternative
          # TODO: This overwrites flags, which is a BAD thing
          for a in flags:
            be[a] = flags[a]
        else:
          # No, just use the existing environment
          be = buildEnvironment
        buildProgramWithCommonSources([sourceFile], commonSources, be,
          aliasString)
      

## This function takes a list of source files (including wildcards), 
#  adds the PIC24 common
#  libraries to each item, then uses Program to build each item.
#  \param sourceFileList A list of source files (which may include
#                        wildcards) to be built.
#  \param commonLibs     A list of libraries upon which all sources
#                        in the sourceFileList depend. Wildcards are not
#                        supported.
#  \param buildEnvinonment An Environment in which to build these sources.
def buildWithCommonLibs(sourceFileList, commonLibs, buildEnvironment):
   for sourceFileGlob in sourceFileList:
      for sourceFile in Glob(sourceFileGlob, True, True, True):     
          f=os.path.splitext(sourceFile)[0]
          print sourceFile, f, commonLibs
          buildEnvironment.Program([sourceFile], LIBS=commonLibs)
          myHex = buildEnvironment.Hex(f,f)
          Depends( myHex, commonLibs)

## Instruct SCons to build the common PIC24 library upon which 
#  all the textbook example programs are based.
#  PIC24 support library files specified in \ref  PIC24SupportLibSources.
#myLib = env.StaticLibrary(target='lib/msu_pic24', source=PIC24SupportLibSources )

# Instruct SCons to build all the textbook example programs based
#  on the PIC24 support library files specified in \ref
#  PIC24SupportLibSources.
#if 'lib11' in COMMAND_LINE_TARGETS:
#  #env.Program(['chap11/dac_r2r.c'],LIBS=myLib)
#  env.Program(['chap11/dac_r2r.c'] + PIC24SupportLibSources)
#  env.Hex('chap11/dac_r2r','chap11/dac_r2r')
#  #buildWithCommonLibs(['chap11/*.c'], myLib, env)

## Compile the support library into objects for the default
#  environment.
PIC24SupportLibObjects = env.Object(PIC24SupportLibSources)
PIC24SupportLibObjectsSmall = env.Object(PIC24SupportLibSourcesSmall)
PIC24SupportLibObjectsSmall2 = env.Object(PIC24SupportLibSourcesSmall2)


if 'reset' in buildTargets:
  buildWithCommonSources(['chap8/reset.c'], PIC24SupportLibObjectsSmall, env, {}, 'reset')
if 'echo' in buildTargets:
  buildWithCommonSources(['chap8/echo.c'], PIC24SupportLibObjectsSmall, env, {}, 'reset')
if 'ledsw1' in buildTargets:
  buildWithCommonSources(['chap8/ledsw1.c'], PIC24SupportLibObjectsSmall, env, {}, 'ledsw1')
if 'chap8' in buildTargets:
  buildWithCommonSources(['chap8/*.c'], PIC24SupportLibObjects, env, {}, 'chap8')
if 'chap9' in buildTargets:
  buildWithCommonSources(['chap9/*.c'], PIC24SupportLibObjects, env, {}, 'chap9')
if 'chap10' in buildTargets:
  buildWithCommonSources([
                          'chap10/mcp24lc515_i2c_eeprom.c',
                          'chap10/reverse_string.c',
                          'chap10/reverse_string1.c',
                          'chap10/soft_uart.c',
                          'chap10/spi_master_revstring.c',
                          'chap10/spi_slave_revstring.c'],
    PIC24SupportLibObjects, env, 
    {'chap10\\reverse_string.c'  : { 'CPPDEFINES': 'UART1_RX_INTERRUPT'}, 
     'chap10\\reverse_string1.c' : { 'CPPDEFINES': ['UART1_RX_INTERRUPT', 'UART1_TX_INTERRUPT']}}, 
    'chap10')
if 'chap10large' in buildTargets:   # These files usually need a larger device (64K) to compile
  buildWithCommonSources(['chap10/stdio_test.c', 
                          'chap10/reverse_string_stdio.c', 'chap10/ds1621_i2c.c', 'chap10/ds1631_i2c.c','chap10/ds1722_spi_tempsense.c',
                          'chap10/mcp41xxx_spi_pot.c',], 
    PIC24SupportLibObjects, env, {}, 'chap10stdio')
if 'chap11_24E' in buildTargets:
  buildWithCommonSources([ 'chap11/adc_spidac_test.c', 
     'chap11/adc_test.c', 
    'chap11/adc_test_12bit.c'], PIC24SupportLibSourcesSmall3, env, {}, 'chap11')
if 'chap11nodma' in buildTargets:
  buildWithCommonSources([ 'chap11/adc2pots1.c','chap11/adc4simul.c','chap11/adc7scan1.c', 'chap11/adc7scan2.c', 'chap11/adc_spidac_test.c', 
     'chap11/adc_test.c', 
    'chap11/adc_test_12bit.c', 'chap11/dac_r2r.c'], PIC24SupportLibSourcesSmall3, env, {}, 'chap11')
if 'chap11dma' in buildTargets:
  buildWithCommonSources(['chap11/adc4simul_dma.c', 'chap11/adc7scan1_dma_conv_order.c', 
    'chap11/adc7scan1_dma_scatter_gather_1.c'],
    PIC24SupportLibObjects, env, {}, 'chap11dma')
if 'chap12_24E' in buildTargets:
  buildWithCommonSources(['chap12/incap_freqmeasure.c','chap12/incap_switch_pulse_measure.c',
  'chap12/ir_biphase_decode.c', 'chap12/manual_switch_pulse_measure.c',
  'chap12/outcompare_contpulse.c', 'chap12/outcompare_squarewave.c',
  'chap12/outputcompare_multiservo.c','chap12/outputcompare_oneservo.c',
  'chap12/pwm_dac.c', 'chap12/pwm_dac_test.c', 'chap12/timer32bit_switch_pulse_measure.c'], PIC24SupportLibSourcesSmall3, env, {}, 'chap12')
if 'chap12' in buildTargets:
  buildWithCommonSources(['chap12/incap_freqmeasure.c','chap12/incap_switch_pulse_measure.c',
  'chap12/ir_biphase_decode.c', 'chap12/ledpwm_pic24f.c', 'chap12/manual_switch_pulse_measure.c',
  'chap12/outcompare_contpulse.c', 'chap12/outcompare_squarewave.c',
  'chap12/outputcompare_multiservo.c','chap12/outputcompare_oneservo.c',
  'chap12/pwm_dac.c', 'chap12/pwm_dac_test.c', 'chap12/timer32bit_switch_pulse_measure.c'], PIC24SupportLibSourcesSmall3, env, {}, 'chap12')
if 'chap12big' in buildTargets:
  buildWithCommonSources(['chap12/ledpwm.c'], PIC24SupportLibSourcesSmall2, env, {}, 'chap12')
if 'chap13' in buildTargets:
  buildWithCommonSources(['chap13/*.c'], PIC24SupportLibObjects, env, 
  {'chap13\\i2c_multmaster_rstring.c' :  { 'CPPDEFINES': 'CPU_ID=1' },
   'chap13\\i2c_multmaster_rstring1.c' : { 'CPPDEFINES': 'CPU_ID=2' }},
  'chap13')
if 'chap14' in buildTargets:
  buildWithCommonSources(['chap14/*.c'], PIC24SupportLibObjects, env, {}, 'chap14')
if 'chap15' in buildTargets:
  # Case-by-case, since this chapter's program have multiple source files
  buildProgramWithCommonSources(['chap15/audio.c', 'chap15/audio_adpcm.c', ], 
    PIC24SupportLibObjects, env, 'chap15')
  buildProgramWithCommonSources(['chap15/reflow_oven.c', 'chap15/reflow_debug.c', 
    'chap15/reflow_flash.c', 'chap15/reflow_operate.c'], PIC24SupportLibObjects,
    env, 'chap15')
  buildProgramWithCommonSources(['chap15/robot.c', 'chap15/robot_ir_decode.c', ], 
    PIC24SupportLibObjects, env, 'chap15')
if 'explorerh' in buildTargets:  # Don't build rtcc.c, since the PIC24H doesn't have it
  buildWithCommonSources(['explorer16_100p/adc_test.c', 'explorer16_100p/lcd4bit.c', 
    'explorer16_100p/mcp25lc256_spi_eeprom.c', 'explorer16_100p/reset.c', 
    'explorer16_100p/timer1_sosc.c'], PIC24SupportLibObjects, env, {}, 'explorerh')
if 'explorer' in buildTargets:
  buildWithCommonSources(['explorer16_100p/*.c'], PIC24SupportLibObjects, env, {}, 'explorer')
if 'bootloader' in buildTargets:
  targetName = 'p${MCU}_230400baud_bootldr'
  env.Program(target=targetName, source = ['bootloader/24h_24f_target/main.c', 
    'bootloader/24h_24f_target/mem.c',
    'bootloader/24h_24f_target/pic24_clockfreq.c', 
    'bootloader/24h_24f_target/pic24_configbits.c'])
  cof2hex(targetName, env, 'bootloader')

## @}


#################################################################################
## TESTING
##
## Interestingly, the adc7scan* projects cause the Linux c30-gcc compiler
##    to seg-fault.  
##
#env.Program('dac_r2r.c', LIBS='msu_pic24', LIBPATH='../lib')
#MyHex = env.Hex('dac_r2r', 'dac_r2r')
#Depends( MyHex, ['dac_r2r.c'])
#Depends( MyHex, myLib)
################################
#buildWithCommonLibs(['./*.c'], myLib, env)
#buildWithCommonLibs(['chap11/adc2pots1.c','chap11/adc4simul.c','chap11/dac_r2r.c'], myLib, env)
#buildWithCommonLibs(['chap11/dac*.c','chap11/adc*.c'], myLib, env)
#buildWithCommonLibs(['chap10/ds1621_i2c.c'], myLib, env)
