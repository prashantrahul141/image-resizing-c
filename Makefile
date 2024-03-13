all: main

main: main_release.o
	gcc main_release.o -o main -lm

main_release.o: main.c
	gcc -c -O3 -o main_release.o -Wall -Wextra -pedantic main.c

build: main_debug.o
	gcc main_debug.o -o main -lm

main_debug.o: main.c
	gcc -c -DDEBUG -o main_debug.o -Wall -Wextra -pedantic main.c

clean:
	rm -f ./main
	rm -f ./main_debug.o
	rm -f ./main_release.o
	rm -f output.jpg
