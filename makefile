TARGET: main

main: main.o binary-cell.o binary-1d-ca.o binary-1d-ca-manager.o rule-vector.o utils.general.o utils.math.o utils.matrix.o utils.number.o utils.polynomial.o utils.transition-graph.o utils.vector.o
	g++ main.o binary-cell.o binary-1d-ca.o binary-1d-ca-manager.o rule-vector.o utils.general.o utils.math.o utils.matrix.o utils.number.o utils.polynomial.o utils.transition-graph.o utils.vector.o -o main -fopenmp

main.o: src/main.cpp
	g++ -c src/main.cpp -o main.o

binary-cell.o: src/models/binary-cell/binary-cell.cpp
	g++ -c src/models/binary-cell/binary-cell.cpp -o binary-cell.o

binary-1d-ca.o: src/models/binary-1d-ca/binary-1d-ca.cpp
	g++ -c src/models/binary-1d-ca/binary-1d-ca.cpp -o binary-1d-ca.o -fopenmp

binary-1d-ca-manager.o: src/models/binary-1d-ca-manager/binary-1d-ca-manager.cpp
	g++ -c src/models/binary-1d-ca-manager/binary-1d-ca-manager.cpp -o binary-1d-ca-manager.o

rule-vector.o: src/models/rule-vector/rule-vector.cpp
	g++ -c src/models/rule-vector/rule-vector.cpp -o rule-vector.o

utils.general.o: src/utils/lib/general.cpp
	g++ -c src/utils/lib/general.cpp -o utils.general.o

utils.math.o: src/utils/lib/math.cpp
	g++ -c src/utils/lib/math.cpp -o utils.math.o

utils.matrix.o: src/utils/lib/matrix.cpp
	g++ -c src/utils/lib/matrix.cpp -o utils.matrix.o

utils.number.o: src/utils/lib/number.cpp
	g++ -c src/utils/lib/number.cpp -o utils.number.o

utils.polynomial.o: src/utils/lib/polynomial.cpp
	g++ -c src/utils/lib/polynomial.cpp -o utils.polynomial.o

utils.transition-graph.o: src/utils/lib/transition-graph.cpp
	g++ -c src/utils/lib/transition-graph.cpp -o utils.transition-graph.o

utils.vector.o: src/utils/lib/vector.cpp
	g++ -c src/utils/lib/vector.cpp -o utils.vector.o

clean:
	rm -f main
	rm -f *.o
