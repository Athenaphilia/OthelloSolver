compile:
	gcc main.c -o main position.c solver.c utils.c -lm -Wall -O3
	gcc tester.c -o main position.c -Wall -O3