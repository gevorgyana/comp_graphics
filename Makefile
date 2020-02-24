.PHONY: clean pslgpl

clean:
	rm pslgpl_chain_decomp

pslgpl:
	clang++ pslgplcd.cpp -o pslgpl_chain_decomp
