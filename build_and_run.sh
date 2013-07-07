#!/bin/bash

g++ -Wall -std=c++0x -g -o walkabout -lGL -lGLU -lglut /usr/lib64/libGLEW.a walkabout.cpp drawable.cpp enemy.cpp player.cpp projectile.cpp rect.cpp square.cpp wall.cpp floor.cpp world.cpp
if [ $? -eq 0 ]; then
	./walkabout
fi
