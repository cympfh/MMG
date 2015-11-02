all: mmg.exe mmg.debug.exe

test:
	./mmg.exe -D -K 3 < sample/data

mmg.debug.exe: main.cc mmg.cc setcover.o
	g++ --std=c++11 -g -o $@ setcover.o main.cc -lgmp -lgmpxx

mmg.exe: main.cc mmg.h mmg.cc setcover.o
	g++ --std=c++11 -O3 -o $@ setcover.o main.cc -lgmp -lgmpxx

setcover.o: setcover.cc
	g++ -O3 -std=c++11 -c -o $@ $^ -lgmp -lgmpxx

clean:
	rm -f *.exe sample/*.out
