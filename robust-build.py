#!/usr/bin/env python3

import os, fnmatch
from os.path import expanduser
from platform import system
from shutil import copy2
from subprocess import call
import shlex

nux = "nux" in system().lower()
win = "win" in system().lower()

print "Attempting to search for openvr sdk"

for root, dirs, files in os.walk(expanduser("~")):
	for name in files: 
		absn = os.path.join(root, name)
		match = '*src/openvr_api_public.cpp'
		if fnmatch.fnmatch(absn, match):
			ssdist = len(match) - 0;
			openvr_path = absn[:-ssdist] 

print "Found the openvr sdk:      '" + openvr_path + "'"

if nux:
	openvr_bin = openvr_path + "/bin/linux64" 
if win:
	openvr_bin = openvr_path + "\\bin\\win32" 

print "Found the openvr binaries: '" + openvr_bin + "'"

print "Generating compile command..."

comp = 'g++ -L%s -I%s -Wl,-rpath,%s -Wall -Wextra  -std=c++0x -fpermissive -o track *.cpp -lopenvr_api' % (openvr_bin,openvr_path,openvr_bin) 

print "Copying lib binaries to:   '" + os.getcwd() + "'"

for root, dirs, files in os.walk(openvr_bin):
	for name in files: 
		absn = os.path.join(root, name)
		copy2(absn, os.getcwd())

if nux:
	outfile =os.getcwd() + "/compile.sh";
if win:
	outfile =os.getcwd() + "\\compile.bat";

out = open(outfile,'w+')

if nux:
	out.write("#! /bin/sh \n")

print "Writing output file to:    '" + outfile + "'";

out.write(comp + "\n")

print "Finished."

