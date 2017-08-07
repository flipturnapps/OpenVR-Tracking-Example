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
	count = -1
	while True:
		numGood = False
		try:
			nInput = input()
			count++
			if not nInput.lower().startswith('invalid'):
				num = float(nInput)
				numGood = True
		except Exception as e:
			break
		else:
			pass
		finally:
			pass
		if numGood is True:
			if count % 3 == 0:
				devices[count//3].z = -num;
			elif count % 3 == 0:
				devices[count//3].y  = num;
			elif count % 3 == 0:
				devices[count//3].x  = -num;


		
thread = threading.Thread(target=process)
thread.start()

hmd = box(color=color.green)
left = box(color=color.blue)
right = box(color=color.red)

box(color=color.white)

while True:
	hmd.pos = devices[0] * 2
	left.pos = devices[1] * 2
	right.pos = devices[2] * 2


