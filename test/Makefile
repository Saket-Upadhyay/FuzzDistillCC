# Define variables
SOURCE = calculator.cpp
FINALBINARY= modifiedbinary

# Define targets
.PHONY: clean all run clangbuild instrument bitcodetobinary runlli dis

all: clean clangbuild fuzzdistillcc bitcodetobinary
clean:
	rm -f *.o *.bc modifiedbinary *.ll *.csv

run: all
	./$(FINALBINARY)

runlli: fuzzdistillcc
	lli -load out.bc

clangbuild: $(SOURCE)
	clang++ -O0 -static -g -c -emit-llvm -o original.bc $(SOURCE)

fuzzdistillcc: clangbuild
	./fuzzdistillcc original.bc

bitcodetobinary: fuzzdistillcc
	llc --relocation-model=pic -filetype=obj original.bc -o original.o
	clang++ original.o -o $(FINALBINARY)

 dis: all
	llvm-dis *.bc
