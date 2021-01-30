bin\astyle --brackets=attach --indent-switches --indent=spaces=2 --recursive --exclude=build --lineend=linux --suffix=none --formatted *.c *.h templates/*.*
@echo off
rem Notes on options (see http://astyle.sourceforge.net/astyle.html for more information):
rem --brackets=attach / -a :: Attach brackets to their pre-block statements ( e.g. Java / K&R style ).
rem --indent-switches / -S :: Indent 'switch' blocks so that the 'case X:' statements are indented in the switch block. The entire case block is indented.
rem --indent=spaces / -s#  :: Indent using # spaces per indent
rem --recursive / -r / -R  :: For each directory in the command line, process all subdirectories recursively.
rem --suffix=none / -n     :: Do not retain a backup of the original file. The original file is purged after it is formatted.
rem --lineend=linux / -z2  :: Force use of the specified line end style.
rem --formatted / -Q       :: Formatted files display  mode. Display only the files that have been formatted. Do not display files that are unchanged.
