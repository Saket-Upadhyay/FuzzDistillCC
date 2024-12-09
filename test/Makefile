# Define variables
SOURCE = calculator.cpp
FINALBINARY= calculator.out

# Define targets
.PHONY: clean all run clangbuild instrument bitcodetobinary

all: clean clangbuild fuzzdistillcc bitcodetobinary
clean:
	rm -f *.o *.bc $(FINALBINARY) *.ll *.csv

run: all
	./$(FINALBINARY)

clangbuild: $(SOURCE)
	clang++ -O0 -static -g -c -emit-llvm -o original.bc $(SOURCE)

fuzzdistillcc: clangbuild
	./fuzzdistillcc original.bc

bitcodetobinary: fuzzdistillcc
	llc --relocation-model=pic -filetype=obj original.bc -o original.o
	clang++ original.o -o $(FINALBINARY)