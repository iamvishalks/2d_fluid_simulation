
PKGS=glfw3 glew
CFLAGS=-Wall -Wextra -pedantic `pkg-config --cflags $(PKGS)`
LIBS=`pkg-config --libs $(PKGS)`


mpm: main.cpp
	g++ $(CFLAGS) -o mpm main.cpp $(LIBS) -lm && ./mpm
