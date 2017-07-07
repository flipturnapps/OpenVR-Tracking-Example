LC := /home/kellark/Documents/openvr/bin/linux64/
HD := /home/kellark/Documents/openvr/

track: 
	g++ -L$(LC) -I$(HD) -Wl,-rpath,$(LC) -Wall -Wextra  -std=c++0x -fpermissive -o track *.cpp -lopenvr_api

