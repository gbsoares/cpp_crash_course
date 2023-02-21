all: 8_1 8_3

8_1: main_8_1.cc FibonacciRange.cc FibonacciIterator.cc
	g++ -o 8_1.out main_8_1.cc FibonacciRange.cc FibonacciIterator.cc -I.

8_3: main_8_3.cc PrimeNumberRange.cc FibonacciRange.cc FibonacciIterator.cc
	g++ -o 8_3.out main_8_3.cc PrimeNumberRange.cc FibonacciRange.cc FibonacciIterator.cc -I.

clean:
	rm ./8_*.out