test2: test2.o
	g++ -o test2 test2.o -lpthread
test2.o: test2.cpp
	g++ -pedantic-errors -std=c++11 -c test2.cpp -o test2.o
clean:
	rm -f *.o test
PHONY: clean