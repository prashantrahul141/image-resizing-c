all: main

test_1: main
	./main 2 ./images/arch.jpg

test_2: main
	./main 2 ./images/2x2.png

main: main.c
	gcc -DDEBUG -o main -Wall -Wextra -pedantic main.c -lm

build: main.c
	gcc -O3 -o main -Wall -Wextra -pedantic main.c -lm


clean:
	rm ./main
