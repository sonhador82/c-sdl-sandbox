CC = gcc 
CLAGS = -D_REENTRANT -I/usr/include/SDL2 -std=c99 -W -Wall -Wextra
LIBS =  -lSDL2 -lGL -lcglm -lm
build:
	gcc $(CLAGS) main.c -o main $(LIBS)
#ld --oformat elf64-x86-64 --verbose 1 -o main  $(LIBS) ./main.o
