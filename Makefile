test: kmmg.exe
	./kmmg.exe < sample/input.txt | tee sample/output.txt
	./kmmg.exe -m 0.0 -p 0.2 < sample/test.txt > sample/out0.txt
	./kmmg.exe -m 0.2 -p 0.4 < sample/test.txt > sample/out1.txt
	./kmmg.exe -m 0.4 -p 0.6 < sample/test.txt > sample/out2.txt
	./kmmg.exe -m 0.6 -p 0.8 < sample/test.txt > sample/out3.txt
	./kmmg.exe -m 0.8 -p 1.0 < sample/test.txt > sample/out4.txt

kmmg.exe: kmmg.h kmmg.cc
	g++ --std=c++11 -O3 -o $@ kmmg.cc

clean:
	rm -f *.exe
