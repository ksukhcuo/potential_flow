ifeq ($(OS),Windows_NT)
	A = ./build/a.exe
else
	A = ./build/a.out
endif

make: $(A)
	$(A)

$(A): ./build/main.o ./build/Gnuplot.o
	g++ ./build/main.o ./build/Gnuplot.o -o $(A)

./build/main.o: ./src/main.cpp
	g++ -c ./src/main.cpp -o ./build/main.o

./build/Gnuplot.o: ./src/Gnuplot.cpp
	g++ -c ./src/Gnuplot.cpp -o ./build/Gnuplot.o

