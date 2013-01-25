CC=g++
CPPFLAGS=--std=c++0x -g3
LDLIBS=-lglut -lGLEW -lGLEW -lGL
all: Main; ./Main; make clean
Main: Shader.o Program.o
clean:
	rm -f *.o Main
	rm *~
.PHONY: all clean