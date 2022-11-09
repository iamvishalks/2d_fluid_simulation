
PKGS=glfw3 glew
CFLAGS=-Wall -Wextra -pedantic `pkg-config --cflags $(PKGS)`
LIBS=`pkg-config --libs $(PKGS)`


mpm: main.c
	gcc $(CFLAGS) -o mpm main.c $(LIBS) -lm && ./mpm