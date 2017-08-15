from vpython import *
import time
import threading

#scene.autoscale = False

print("\n")

hmd = vector(0,0,0)
left = vector(0,0,0)
right = vector(0,0,0)

devices = [hmd, left, right]

def process():
	xyz = ['x','y','z']
	global devices	
	while True:
		try:
			editIndex = -1;
			nInput = input()
			splits = nInput.split()    		
		except Exception as e:
			break
		else:
			pass
		finally:
			pass
		for split in splits:
			if "HMD" in split:
				editIndex = 0;
			if "LEFT:" in split:
				editIndex = 1;
			if "RIGHT:" in split:
				editIndex = 2;

			if editIndex < 0 or len(split) < 3 or split[1] != ':' or split[0] not in xyz:
				continue
			num = float(split[2:])
			if split[0] == 'x':
				devices[editIndex].z = -num;
			elif split[0] == 'y':
				devices[editIndex].y  = num;
			elif split[0] == 'z':
				devices[editIndex].x  = -num;


		
thread = threading.Thread(target=process)
thread.start()

b_hmd = box(color=color.magenta)
b_left = box(color=color.blue)
b_right = box(color=color.red)

b_white = box(color=color.white)
b_white.pos = vector(0,0,0)

while True:
	b_hmd.pos = devices[0] * 2
	b_left.pos = devices[1] * 2
	b_right.pos = devices[2] * 2
	b_left.rotate(angle=.001, axis=vector(0,1,0))


