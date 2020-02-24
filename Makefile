.PHONY: clean pslgpl

clean:
	rm pslgpl_chain_decomp

pslgpl:
	clang++ pslgpl_chain_decomposition.cpp -o pslgpl_chain_decomp
