.PHONY: clean run

run: pslgplcd
	./pslgplcd

clean:
	rm -f pslgplcd *.o pl *.out

pslgplcd: pslgplcd.cpp
	clang++ pslgplcd.cpp -o pslgplcd
pl:
	clang++ point_location.cpp -o pl
