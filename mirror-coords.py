from vpython import *
import time
import threading


hmd = vector(1,0,0)

w = 0;
oldw = 0;

def process():
	print("??")
	while True:
		n = 0
		try:
			editIndex = -1;
			nInput = input()
			if not nInput.startswith("PROCESSED"):
				continue
			#print("good " + nInput)
			splits = nInput.split()
		except Exception as e:
			#print("bad " + nInput)
			print(e)
			continue
		else:
			pass
		finally:
			pass

		for split in splits:
			if n == 0:
				n =1
				continue
			num = float(split[2:])
			if split[0] == 'w':
				pass
			elif split[0] == 'x':
				hmd.x = num;
			elif split[0] == 'y':
				hmd.y  = num;
			elif split[0] == 'z':
				hmd.z  = num;

		#print("x:%.3f y:%.3f :%.3f" % (hmd.x,hmd.y,hmd.z))


		
thread = threading.Thread(target=process)
thread.start()

b_hmd = box(color=color.magenta)
b_hmd.pos = vector(-1,0,0)

b_box = box(color=color.white)
b_box.pos = vector(-3,0,0)
while True:
	b_hmd.axis = norm(hmd)