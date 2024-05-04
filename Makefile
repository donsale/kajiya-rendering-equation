CC = g++
CFLAGS = -std=c++20 -fopenmp -O2 -Wl,--stack,4194304
LDFLAGS =

# default target
all: compile run

compile: build src/main.cpp
	$(CC) $(CFLAGS) src/main.cpp -o build/main.o
	
ifeq ($(OS),Windows_NT)

build:
	mkdir build
run: build/main.o
	build/main.o
clean:
	rmdir /s /q build


test: src/test.cpp
	@g++ src/test.cpp -o build/test
	@./build/test
	
else

build:
	@mkdir -p build

run: build/main.o
	@./build/main.o

test: src/test.cpp
	@g++ src/test.cpp -o build/test
	@./build/test

clean:
	rm -rf build

endif
