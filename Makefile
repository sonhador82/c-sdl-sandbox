
build:
	gcc -std=c99 -m64 ./main.c -lSDL2 -I/usr/include/SDL2 -W -Wall -Wextra -D_REENTRANT -o main