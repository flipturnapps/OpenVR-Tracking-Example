from vpython import *
import time
import threading

#scene.autoscale = False

print("\n")

x = 0
y = 0
z = 0

def process():
	global x
	global y
	global z
	xyz = ['x','y','z']
	while True:
		try:
			nInput = input()
			splits = nInput.split()    		
		except Exception as e:
			break
		else:
			pass
		finally:
			pass
		for split in splits:
			if len(split) < 3 or split[1] != ':' or split[0] not in xyz:
				continue
			num = float(split[2:])
			if split[0] == 'x':
				x = num;
			elif split[0] == 'y':
				y = num;
			elif split[0] == 'z':
				z = num;


		
thread = threading.Thread(target=process)
thread.start()

b = box(pos=vector(x,y,z), color=color.blue)
box(color=color.red)

while True:
	b.pos = vector(x,y,z)
	print( ("X:%f Y:%f Z:%f" % (x,y,z) ) )


