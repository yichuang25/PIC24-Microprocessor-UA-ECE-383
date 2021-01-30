# Note the .py extension on this file, necessary so that
# Doxygen parses as a Python script (but not necessary
# for SCons).

# Documentation for this file. If the \file tag isn't present,
# this file won't be documented.
## \file
#  This file builds various files from templates in the
#  templates/ directory in which this file resides. It should
#  be called from the main SConscript.py script.
#

## @{
#  \name Setup for template building
###############################################################################

import os
import string
from string import Template

# Import environment from calling SConstruct context
Import('env')

## Define the directory in which the templates reside
templateDir = './'

## Define the include directory destination for .h templates
includeDir = '../lib/include/'

## Define the common directory destination for .c templates
commonDir = '../lib/common/'

## @}
 
## @{
#  \name Functions supporting template file builds
###############################################################################

## A function to do search and replaces on a file, optionally
#  appending the results to the output file.
#  \param sourceFileName Name of the source file
#  \param destFileName Name of the destination file
#  \param mapping A dictionary of key=value pairs used by
#         Template to perform the search and replace operation.
#  \param openMode Mode with which to open the destination file.
#         Use the default of 'wb' to overwrite the 
#         destination file with the replaced source file. Use 'ab' to
#         append the replaced source file to the destination file.
#         The 'b' at end end of 'wb'/'ab' keeps line endings Unix-style,
#         even when run under Windows.
def searchAndReplace(sourceFile, destFileName, mapping, openMode='wb'):
  outFile = open(destFileName, openMode)
  template = Template(open(sourceFile).read())
  outFile.write(template.substitute(mapping))

## A function to create a .c/.h file from multiple replaces through
#  a template.
#  \param templateFileName Name of the template file to use
#  \param destFileName Name of the destination file to create
#         by accumulating multiple passes through the template
#         file
#  \param iters Number of iterations (passes) through the file
#         to make. During each iteration, $x is replaced by the
#         iteration number.
def genFromTemplate(templateFileName, destFileName, iters):
  openMode = 'wb'
  for i in range(1, iters + 1):
    searchAndReplace(templateFileName, destFileName, 
      {'x' : str(i)}, openMode)
    openMode = 'ab'

## Builds a .c from a template -- SCons Builder function formation.
# Function will build the .c as requested.  Has the proper
# inputs, outputs and returns value to be registered SCons environment
# as a builder function
#  \param target Target file for this function to build
#  \param source File on which target is built from
#  \param env Environment (if needed)
def c_template_builder(target, source, env):
  s=str(source[0])
  t=str(target[0])
  f=os.path.split(str(target[0]))[-1]
  g=os.path.splitext(f)[0]
  if (g == "pic24_uart"):
    genFromTemplate(s, t, 4)
  if (g == "pic24_i2c"):
    genFromTemplate(s, t, 2)
  if (g == "pic24_ecan"):
    genFromTemplate(s, t, 2)
  if (g == "pic24_spi"):
    genFromTemplate(s, t, 2)
  return None

## Builds a .h from a template -- SCons Builder function formation.
# Function will build the .c as requested.  Has the proper
# inputs, outputs and returns value to be registered SCons environment
# as a builder function
#  \param target Target file for this function to build
#  \param source File on which target is built from
#  \param env Environment (if needed)
def h_template_builder(target, source, env):
  s=str(source[0])
  t=str(target[0])
  f=os.path.split(str(target[0]))[-1]
  g=os.path.splitext(f)[0]
  if ( g == "pic24_uart"):
    genFromTemplate(s, t, 4)
  if ( g == "pic24_i2c"):
    genFromTemplate(s, t, 2)
  if ( g == "pic24_ecan"):
    genFromTemplate(s, t, 2)
  return None

## Define and register a template-driven builder for .c files
cbldr = Builder(action = c_template_builder, suffix='.c', src_suffix='.c-template')
env.Append(BUILDERS = {'CTemplate' : cbldr})

## Define and register a template-driven builder for .h files
hbldr = Builder(action = h_template_builder, suffix='.h', src_suffix='.h-template')
env.Append(BUILDERS = {'HTemplate' : hbldr})

## @}


## @{
#  \name Calls to build templates
###############################################################################

# Specify which files are produced by templates
env.CTemplate('../lib/common/pic24_uart','pic24_uart')
env.HTemplate('../lib/include/pic24_uart','pic24_uart')
env.CTemplate('../lib/common/pic24_i2c','pic24_i2c')
env.HTemplate('../lib/include/pic24_i2c','pic24_i2c')
env.CTemplate('../lib/common/pic24_spi','pic24_spi')
env.CTemplate('../lib/common/pic24_ecan','pic24_ecan')
env.HTemplate('../lib/include/pic24_ecan','pic24_ecan')


## @}
