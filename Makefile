CC = gcc
CFLAGS = -g -Wall 

all: main main-asan

main: main.cpp
	$(CC) $(CFLAGS) -o main main.cpp

main-asan: main.cpp
	clang -fsanitize=address -O1 -fno-omit-frame-pointer -g main.cpp -o main-asan

.PHONY: clean
clean:
	rm -f main main-asan