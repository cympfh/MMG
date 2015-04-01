test: kmmg.exe
	./kmmg.exe < sample/thesis | tee sample/thesis.1.out
	./kmmg.exe -r < sample/thesis | tee sample/thesis.2.out
	./kmmg.exe -c -sub 1 -sup 4 < sample/job | tee sample/job.1.out
	./kmmg.exe -r -sub 0.1 -sup 0.4 < sample/job | tee sample/job.2.out

kmmg.exe: kmmg.h kmmg.cc
	g++ --std=c++11 -O3 -o $@ kmmg.cc

clean:
	rm -f *.exe
