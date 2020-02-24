.PHONY: clean

clean:
	rm -f pslgplcd *.o pl

pslgpl:
	clang++ pslgplcd.cpp -o pslgplcd
pl:
	clang++ point_location.cpp -o pl
