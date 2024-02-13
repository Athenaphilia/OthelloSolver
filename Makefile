BIN_NAME=othello
build:
	$(CC) -o othello *.c -lm -Wall -O3

clean:
	rm main