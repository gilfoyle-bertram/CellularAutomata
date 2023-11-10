TARGET: main

main: main.o binary-cell.o binary-1d-ca.o utils.o
	g++ main.o binary-cell.o binary-1d-ca.o utils.o -o main -fopenmp

main.o: src/main.cpp
	g++ -c src/main.cpp -o main.o

binary-cell.o: src/models/binary-cell/binary-cell.cpp
	g++ -c src/models/binary-cell/binary-cell.cpp -o binary-cell.o

binary-1d-ca.o: src/models/binary-1d-ca/binary-1d-ca.cpp
	g++ -c src/models/binary-1d-ca/binary-1d-ca.cpp -o binary-1d-ca.o -fopenmp

utils.o: src/utils/utils.cpp
	g++ -c src/utils/utils.cpp -o utils.o

clean:
	rm -f main
	rm -f *.o
