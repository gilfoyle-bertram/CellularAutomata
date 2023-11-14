TARGET: main

main: main.o binary-cell.o binary-1d-ca.o rule-vector.o utils.general.o utils.math.o utils.matrix.o utils.polynomial.o
	g++ main.o binary-cell.o binary-1d-ca.o rule-vector.o utils.general.o utils.math.o utils.matrix.o utils.polynomial.o -o main -fopenmp

main.o: src/main.cpp
	g++ -c src/main.cpp -o main.o

binary-cell.o: src/models/binary-cell/binary-cell.cpp
	g++ -c src/models/binary-cell/binary-cell.cpp -o binary-cell.o

binary-1d-ca.o: src/models/binary-1d-ca/binary-1d-ca.cpp
	g++ -c src/models/binary-1d-ca/binary-1d-ca.cpp -o binary-1d-ca.o -fopenmp

rule-vector.o: src/models/rule-vector/rule-vector.cpp
	g++ -c src/models/rule-vector/rule-vector.cpp -o rule-vector.o

utils.general.o: src/utils/lib/general.cpp
	g++ -c src/utils/lib/general.cpp -o utils.general.o

utils.math.o: src/utils/lib/math.cpp
	g++ -c src/utils/lib/math.cpp -o utils.math.o

utils.matrix.o: src/utils/lib/matrix.cpp
	g++ -c src/utils/lib/matrix.cpp -o utils.matrix.o

utils.polynomial.o: src/utils/lib/polynomial.cpp
	g++ -c src/utils/lib/polynomial.cpp -o utils.polynomial.o

clean:
	rm -f main
	rm -f *.o
