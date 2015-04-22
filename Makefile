test: mmg.exe
	./mmg.exe -k 4 < sample/data1 > sample/data1.out && \
	diff sample/data1.out sample/data1.ans
	./mmg.exe -k 4 < sample/data2 > sample/data2.k4.out && \
		diff sample/data2.k4.out sample/data2.k4.ans
	./mmg.exe -k 5 < sample/data2 > sample/data2.k5.out && \
	diff sample/data2.k5.out sample/data2.k4.ans
	./mmg.exe -k 3 < sample/data2 > sample/data2.k3.out && \
	diff sample/data2.k3.out sample/data2.k3.ans
	./mmg.exe -k 5 < sample/data3 > sample/data3.out && \
	diff sample/data3.out sample/data3.ans

mmg.exe: mmg.h mmg.cc
	g++ --std=c++11 -O3 -o $@ mmg.cc

clean:
	rm -f *.exe sample/*.out
