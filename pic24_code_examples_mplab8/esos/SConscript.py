# This file contains all that a SConstruct file normally contains.
# In order to use Doxygen, however, all Python files must end with
# a .py extension. So, SConstuct simply executes this file.

# Documentation for this file. If the \file tag isn't present,
# this file won't be documented.
## \file
#  This file supports ESOS builds.

## Common sources used for the PIC24 support library
ESOSSupportLibSources = [ 
  'src/esos.c',
  '../common/pic24_clockfreq.c',
  '../common/pic24_configbits.c',
  'src/pic24/esos_pic24_tick.c',
  'src/pic24/esos_pic24_rs232.c',
  'src/esos_comm.c',
  'src/pic24/esos_pic24_irq.c']

env = env.Clone(CPPDEFINES=['BUILT_ON_ESOS' '_NOASSERT'])
