#!/usr/bin/env python3

import os, fnmatch
from os.path import expanduser
from platform import system
from shutil import copy2
from subprocess import call
import shlex
import sys

def makeWinPath(path):
	path = path.replace("/cygdrive/c","C:\\")
	path = path.replace("/","\\")
	path = path.replace("\\\\","\\")
	path = path.replace("\\","\\\\")
	return path


nux = "nux" in system().lower()
win = "win" in system().lower()
found = False

print("\nThis script will output a script to build the VR Track example.\n")
testfile_rel_path = "src/openvr_api_public.cpp"

if (len(sys.argv) > 1):
	ipath = sys.argv[1]
	ifile = os.path.join(ipath, testfile_rel_path)
	found = os.path.isfile(ifile) 

if not found:
	ipath = input("Enter path to the openvr repository clone or press enter\nto have the script search your home directory.\n> ")
	ifile = os.path.join(ipath, testfile_rel_path)
	found = os.path.isfile(ifile) 

if found:
	openvr_path = ifile[:-(len(testfile_rel_path)+1)] 


if not found:
	print("Attempting to search home directory for openvr sdk....")
	for root, dirs, files in os.walk(expanduser("~")):
		for name in files: 
			absn = os.path.join(root, name)
			match = '*src/openvr_api_public.cpp'
			if fnmatch.fnmatch(absn, match):
				ssdist = len(match) - 0
				openvr_path = absn[:-ssdist] 
				found = True
				break

if (not found) and win:
	print("Attempting to search C:\\Users\\ for openvr sdk....")
	winroot = "C:\\Users\\"
	for root, dirs, files in os.walk(winroot):
		for name in files: 
			absn = os.path.join(root, name)
			match = '*src/openvr_api_public.cpp'
			if fnmatch.fnmatch(absn, match):
				ssdist = len(match) - 0
				openvr_path = absn[:-ssdist] 
				found = True
				break
			
if not found:
	print("Failed to find the openvr sdk. Clone Valve's openvr sdk and try again.")
	sys.exit()

if win:
	openvr_path = makeWinPath(openvr_path)

print("Found the openvr sdk:      '" + openvr_path + "'")



if nux:
	openvr_bin = openvr_path + "/bin/linux64" 
if win:
	openvr_bin = openvr_path + "\\bin\\win64" 
	openvr_bin = makeWinPath(openvr_bin)

print("Found the openvr binaries: '" + openvr_bin + "'\n")

print("Generating compile command...")

comp = 'g++ -L%s -I%s -Wl,-rpath,%s -Wall -Wextra  -std=c++0x -o build/track *.cpp *.c -lopenvr_api' % (openvr_bin,openvr_path,openvr_bin) 

print(" - Command: " + comp + "\n")

os.mkdir("build")

if nux:
	outfile = "build/compile.sh"
if win:
	outfile = "build\\compile.bat"
	outfile = makeWinPath(outfile)
	
out = open(outfile,'w+')

if nux:
	out.write("#! /bin/sh \n")


print("Writing output file to:    '" + outfile  + "'\n")

out.write(comp + "\n")

print("Finished.")

