CC=g++
OPTS=-g -Werror

all: main.o cache.o
	$(CC) $(OPTS) -O3 -o cache main.o cache.o -lm

main.o: main.cpp cache.hpp
	$(CC) $(OPTS) -O3 -c main.cpp

cache.o: cache.hpp cache.cpp
	$(CC) $(OPTS) -O3 -c cache.cpp

clean:
	rm -f *.o cache;
