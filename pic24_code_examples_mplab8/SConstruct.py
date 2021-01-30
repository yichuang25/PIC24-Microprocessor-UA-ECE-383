# This file contains all that a SConstruct file normally contains.
# In order to use Doxygen, however, all Python files must end with
# a .py extension. So, SConstuct simply executes this file.

# Documentation for this file. If the \file tag isn't present,
# this file won't be documented.
## \file
#  This file provides an automated build process for the 
#  \ref index "libraries" included in this collection.
#  To use:
#  -# Install SCons.
#  -# Install the Microchip compiler. Make sure your path
#     includes the directories in which the compiler binaries
#     exist.
#  -# From the command line, change to the directory in which 
#     this file lies.
#  -# Execute <code>SCons</code>, which builds everything.
#
#  The build process can be modified by passing options to 
#  SCons. See <code>SCons --help</code> for options specific
#  to this build and <code>SCons -H</code> for generic SCons
#  options.
#
#  \todo
#  - Figure out how to get Doxygen to generate valid links in the output
#    produced by parsing this file
#  - ESOS build (chap 14)
#  - Some flags for I2C master/slave not done yet
#  - Create some reset replacement that uses more of the functionality
#    (calls functions from all our .c/.h files


import os

## Make sure SCons is recent enough.
EnsureSConsVersion(2, 0)


## @{
#  \name Create a Microchip MCC24 (PIC24F/H) Construction Environment
###############################################################################

## Define command-line options to set processor, bootloader
opts = Variables()
opts.Add(EnumVariable('BOOTLDR', 'Determines bootloader type', 'msu',
                    allowed_values=('msu','none')))

print "Creating a SCons build environment for Microchip C30 family"
env = Environment(
        # Force SCons to set up for with gnu tools to start 
        # with reasonable defaults. Note: using platform = 'posix'
        # causes SCons to try to call fork() when executing programs
        # (such as compilers), which errors out on Windows.
        tools = ['gcc', 'gnulink', 'ar', 'zip', 'packaging'],
        options = opts, 
        # Copied and cobbled together from SCons\Tools\cc.py with mods
        CCCOM = '$CC -c -o $TARGET $CFLAGS $CCFLAGS $CPPFLAGS $_CPPDEFFLAGS $_CPPINCFLAGS $SOURCES',
        CCCCOMSTR = 'Compiling $SOURCES',
        CCFLAGS = '-mcpu=${MCU} -Wall -O1',
        ARFLAGS = 'rcs',
        ARSTR = 'Create static library: $TARGET',
        OBJSUFFIX = '.o',
        PROGSUFFIX = '.cof')
# Copy the host envrionment's path for our scons environment
# so scons can find C30 tools
env['ENV']['PATH'] = os.environ['PATH']

# Adjust our environment to be specific the host OS
if os.name == 'posix':
  print "Modifiying environment for Linux"
  incDirs = Split( """include
    /usr/pic30-elf/include
    /usr/share/pic30-support/generic/h
    /usr/share/pic30-support/pic24h/h
    """)
  libDirs = Split( """.
    lib
    """)
  ## Change linux-specific environment variables
  env.Replace(
    CPPPATH = incDirs, 
    CC = 'pic30-elf-gcc',
    LIBPATH = libDirs,
    AR = 'pic30-elf-ar',
    LINK = 'pic30-elf-gcc', # Copied from SCons\Tools\link.py with mods
  )
  #
  # add the bin2hex program to the environment as a new builder
  #
  b2h = Builder(action = 'pic30-elf-bin2hex $SOURCE',
            suffix = 'hex',
            src_suffix = 'cof')
  env.Append(BUILDERS = {'Hex' : b2h})

elif os.name == 'nt':
  print "Modifying environment for Windoze"
  incDirs = Split( """lib/include
    """)
  libDirs = Split( """.
    """)
  # Change linux-specific environment variables    
  env.Replace(
    CPPPATH = incDirs,
    CC = 'pic30-gcc',
    LIBPATH = libDirs,
    AR = 'pic30-ar',
    LINK = 'pic30-gcc', # Copied from SCons\Tools\link.py with mods
  )
  #
  # add the bin2hex program to the environment as a new builder
  #
  b2h = Builder(action = 'pic30-bin2hex $SOURCE',
            suffix = 'hex',
            src_suffix = 'cof')
  env.Append(BUILDERS = {'Hex' : b2h})

# adjust our default environment based on user command-line requests
dict = env.Dictionary()
if dict['BOOTLDR'] == 'msu':
    env.Replace(
        LINKFLAGS = '-mcpu=${MCU} -Wl,--heap=100,$LINKERSCRIPT',
        LINKERSCRIPT = '--script=lib/lkr' + os.sep + 'p${MCU}_bootldr.gld',
    )
else:
    env.Replace(
        LINKFLAGS = '-mcpu=${MCU} -Wl,--heap=100,$LINKERSCRIPT',
        LINKERSCRIPT = '--script=p${MCU}.gld',
    )

## By default, run two jobs at once (assume a dual-core PC)
#  For some reason I haven't yet determined, this causes build errors. Turn it off for now.
#env.SetOption('num_jobs', 2)
print "Running with -j", env.GetOption('num_jobs')

# generate some command line help for our custom options
Help(opts.GenerateHelpText(env))

#
# A DEBUG STATEMENT to see what the scons build envrionment (env) has defined
#
#print   env.Dump()

## @}



# Definition of targets


## Define what parts of the source tree should be inclued in a
#  .zip distribution.
archiveFiles = [
  'Doxyfile',
  'readme.txt',
  'runAStyle.bat',
  'SConstruct',
  'SConstruct.py',
  'SCons_build.py',
  'standard_header.txt',
  'bin',
  'bootloader',
  'docs',
  'hex',
  'lib/lkr',
  'chap3',
  'chap4',
  'chap6',
  'chap7',
  'chap8',
  'chap9',
  'chap10',
  'chap11',
  'chap12',
  'chap13',
  'chap14',
  'chap15',
  'esos',
  'lib/common',
  'lib/include',
  'explorer16_100p',
  'templates',
  'util' ]

## Select the file name for the .zip archive
archiveFileName = 'build/pic24_code_examples.zip'

## Call SConscript with a specific buildTargets value
def buildTargetsSConscript(buildTargets, env, variantDirName):
  SConscript('SCons_build.py', exports='buildTargets env', 
    variant_dir='build/' + env['MCU'] + "_" + variantDirName)


# Inform SCons about the dependencies in the template-based files
SConscript('./templates/SConscript.py', 'env')

env.Alias('template-build', ['lib/include/pic24_uart.h', 'lib/include/pic24_i2c.h',
                             'lib/include/pic24_ecan.h',
                             'lib/common/pic24_uart.c',  'lib/common/pic24_i2c.c',
                             'lib/common/pic24_spi.c',   'lib/common/pic24_ecan.c']);

## Create a target which zips up these files;
## otherwise, create compilation targets.
if 'zipit' in COMMAND_LINE_TARGETS:
    # Update docs
    env.Command(Glob('docs/*'), Glob('lib/common/*.c'), "doxygen")
    # Zip it!
    zipNode = env.Zip(archiveFileName, archiveFiles)
    env.Alias('zipit', zipNode)
else:
    # Build the PIC24H32/FJ64Gxx02-compatible directories
    #######################################################
    # Build small, non-DMA on the PIC24HJ32GP202
    buildTargetsSConscript(['chap8', 'chap9', 'chap10', 'chap11nodma', 'chap12',      
                            'bootloader'],
    env.Clone(MCU='24HJ32GP202'), 'default')

    # Build the large files on the PIC24HJ64GP202
    buildTargetsSConscript(                            ['chap10large', ],
      env.Clone(MCU='24HJ64GP202'), 'default')

    # Build everything on the PIC24FJ64GA002
    buildTargetsSConscript(['chap8', 'chap9', 'chap10', 'chap10large', 'chap11nodma', 'chap12',
                            'chap15', 'bootloader'],
      env.Clone(MCU='24FJ64GA002'), 'default')

    # Build small, non-DMA on the dsPIC33FJ32GP202
    buildTargetsSConscript(['chap8', 'chap9', 'chap10',                'chap11nodma', 'chap12',
                  'bootloader'],
      env.Clone(MCU='33FJ32GP202'), 'default')

    # Build the large files on the dsPIC33FJ64GP202
    buildTargetsSConscript(                            ['chap10large'],
      env.Clone(MCU='33FJ64GP202'), 'default')

    # Minimally test the 24F16KA102
#    buildTargetsSConscript(['reset', 'echo',  'bootloader'],
#      env.Clone(MCU='24F16KA102'), 'default')

    # Build the PIC24HJGP502-compatible directories
    buildTargetsSConscript(['chap11dma', 'chap13', 'chap15', 'bootloader'],
      env.Clone(MCU='24HJ64GP502'), 'default')

    # Same as above, but for the dsPIC
    buildTargetsSConscript(['chap8', 'chap9', 'chap10', 'chap10stdio', 'chap11dma',  'chap12big','chap12',
                            'chap13', 'chap15', 'bootloader'],
      env.Clone(MCU='33FJ128GP802'), 'default')
    
    # Build some for the PIC24E device
    buildTargetsSConscript(['chap8', 'chap9', 'chap10', 'chap11_24E',  'chap12big','chap12_24E',
                            'bootloader'],
      env.Clone(MCU='24EP64GP202'), 'default')
       
   
    # Build for the explorer board
    buildTargetsSConscript(['explorer', 'bootloader'],
      env.Clone(MCU='24FJ128GA010', CPPDEFINES='HARDWARE_PLATFORM=EXPLORER16_100P'), 'default')    
    buildTargetsSConscript(['explorerh', 'bootloader'],
      env.Clone(MCU='24HJ256GP610', CPPDEFINES='HARDWARE_PLATFORM=EXPLORER16_100P'), 'default')    

    # Do a no-float build of reset
    buildTargetsSConscript(['reset'], 
      env.Clone(MCU='24HJ32GP202',  CPPDEFINES='_NOFLOAT'), 'nofloat')
      
    # Build reset on other supported platforms
    buildTargetsSConscript(['reset'],
      env.Clone(MCU='24FJ64GA002',  CPPDEFINES='HARDWARE_PLATFORM=STARTER_BOARD_28P'), 'starter_board_28p')
    buildTargetsSConscript(['reset'],
      env.Clone(MCU='33FJ128GP204', CPPDEFINES='HARDWARE_PLATFORM=DANGEROUS_WEB'), 'dangerous_web')

    # Build reset with various clock options on all processors
    for clock in ['SIM_CLOCK', 'FRCPLL_FCY16MHz', 'FRC_FCY4MHz', 
    'PRI_NO_PLL_7372KHzCrystal', 'PRIPLL_8MHzCrystal_16MHzFCY']:
        buildTargetsSConscript(['reset'], 
          env.Clone(MCU='24FJ64GA002', CPPDEFINES='CLOCK_CONFIG=' + clock), clock)
        buildTargetsSConscript(['reset'], 
          env.Clone(MCU='24FJ64GA102', CPPDEFINES='CLOCK_CONFIG=' + clock), clock)
#        buildTargetsSConscript(['reset'],
#          env.Clone(MCU='24F16KA102', CPPDEFINES='CLOCK_CONFIG=' + clock), clock)
    for clock in ['SIM_CLOCK', 'PRI_NO_PLL_7372KHzCrystal', 'FRC_FCY3685KHz',
    'FRCPLL_FCY40MHz', 'PRIPLL_7372KHzCrystal_40MHzFCY', 'PRIPLL_8MHzCrystal_40MHzFCY']:
        buildTargetsSConscript(['reset'], 
          env.Clone(MCU='24HJ32GP202',  CPPDEFINES='CLOCK_CONFIG=' + clock), clock)
        buildTargetsSConscript(['reset'], 
          env.Clone(MCU='33FJ128GP802', CPPDEFINES='CLOCK_CONFIG=' + clock), clock)
