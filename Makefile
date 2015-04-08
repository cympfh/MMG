mmg.exe: mmg.h mmg.cc
	g++ --std=c++11 -O3 -o $@ mmg.cc

test: mmg.exe
	./mmg.exe -k 4 < sample/data | tee sample/data.out
	./mmg.exe -c -sub 1 -sup 4 < sample/job | tee sample/job.1.out
	./mmg.exe -r -sub 0.1 -sup 0.4 < sample/job | tee sample/job.2.out

clean:
	rm -f *.exe
