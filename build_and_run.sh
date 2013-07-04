#!/bin/bash

g++ -Wall -std=c++0x `pkg-config --cflags libglfw` -g -o walkabout walkabout.cpp drawable.cpp enemy.cpp player.cpp projectile.cpp rect.cpp square.cpp wall.cpp floor.cpp world.cpp `pkg-config --static --libs libglfw` /usr/lib64/libGLEW.a
if [ $? -eq 0 ]; then
	./walkabout
fi
