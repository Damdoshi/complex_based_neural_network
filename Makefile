
CONF= -g -g3 -ggdb -O0
#CONF = -O3 -ffast-math -march=native

all:
	g++ src/*.cpp -I./include/ -o test  -std=gnu++20 -llapin -lavcall -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system $(CONF)


