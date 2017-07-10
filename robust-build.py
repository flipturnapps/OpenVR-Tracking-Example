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
found = False;

print "\nThis is the VR Track build script.\n"
testfile_rel_path = "src/openvr_api_public.cpp"

if (len(sys.argv) > 1):
	ipath = sys.argv[1]
	ifile = os.path.join(ipath, testfile_rel_path)
	found = os.path.isfile(ifile) 

if not found:
	ipath = raw_input("Enter path to the openvr repository clone or press enter\nto have the script search your home directory.\n> ")
	ifile = os.path.join(ipath, testfile_rel_path)
	found = os.path.isfile(ifile) 

if found:
	openvr_path = ifile[:-(len(testfile_rel_path)+1)] ;


if not found:
	print "Attempting to search home directory for openvr sdk...."
	for root, dirs, files in os.walk(expanduser("~")):
		for name in files: 
			absn = os.path.join(root, name)
			match = '*src/openvr_api_public.cpp'
			if fnmatch.fnmatch(absn, match):
				ssdist = len(match) - 0;
				openvr_path = absn[:-ssdist] 
				found = True
				break

if not found:
	print "Attempting to search C:\\Users\\ for openvr sdk...."
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
	print "Failed to find the openvr sdk."
	sys.exit()

#openvr_path = "C:\\Users\\kellark\\Documents\\openvr"

if win:
	openvr_path = makeWinPath(openvr_path)

print "Found the openvr sdk:      '" + openvr_path + "'"



if nux:
	openvr_bin = openvr_path + "/bin/linux64" 
if win:
	openvr_bin = openvr_path + "\\bin\\win64" 
	openvr_bin = makeWinPath(openvr_bin)

print "Found the openvr binaries: '" + openvr_bin + "'\n"

print "Generating compile command..."

comp = 'g++ -L%s -I%s -Wl,-rpath,%s -Wall -Wextra  -std=c++0x -fpermissive -o track *.cpp -lopenvr_api' % (openvr_bin,openvr_path,openvr_bin) 

print " - Command: " + comp + "\n"

libout = os.getcwd()
if win:
	libout = makeWinPath(libout)
	
print "Copying lib binaries to:   '" + libout + "'"

for root, dirs, files in os.walk(openvr_bin):
	for name in files: 
		absn = os.path.join(root, name)
		copy2(absn, libout)

if nux:
	outfile =os.getcwd() + "/compile.sh";
if win:
	outfile =os.getcwd() + "\\compile.bat";
	outfile = makeWinPath(outfile)
	
out = open(outfile,'w+')

if nux:
	out.write("#! /bin/sh \n")


print "Writing output file to:    '" + outfile  + "'\n";

out.write(comp + "\n")

print "Finished."

