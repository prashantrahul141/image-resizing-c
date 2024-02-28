all: main

test_1: main
	./main 2 ./images/test.jpg

test_2: main
	./main 2 ./images/arch.jpg

main: main.c
	gcc -o main -Wall -Wextra -pedantic main.c -lm

clean:
	rm ./main
