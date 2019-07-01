
PKGS="gtk+-3.0 webkit2gtk-4.0"
INCLUDE=`pkg-config --cflags  $(PKGS)`
LIB=`pkg-config --libs  $(PKGS)`


all: main.c
	gcc -o run main.c $(INCLUDE)  $(LIB) -O3
debug:
	./run
