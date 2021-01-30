#! /usr/bin/env python

import  os

def openfile(fileobj, mode = "", list=None):
    """Generic file opener, adds newlines to list and returns lines or string"""
    try:
        if mode == "":
            file = open(fileobj, "r")
        else:
            file = open(fileobj, mode)

        # Add newline if we write a list to a file
        if mode == "w" or mode == "a":
            for line in list:
                line += '\n'
                file.write(line)

        # Read and return a list if we read a file
        if mode == "r":
            lines = file.read().splitlines()
            return lines

        # If we want a single, big-ass string instead
        else:
            lines = file.read()
            return lines

        file.close()

    except IOError:
        print "No such file: %s" % fileobj


def stringify(list):
    """Turn list into string object and return"""
    return "".join(map(str, list))

def test_openfile():
    """
    Test function for trying out our code shenanigans before using them in
    the actual application.
    """

    lines = openfile("functions.py")
    string = stringify(lines)
    print string

if __name__ == '__main__':
        test_openfile()