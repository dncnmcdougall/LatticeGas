CFLAGS=-Wall -Wpedantic -Werror -Wno-comment
LIBS = -lpng -lm

c_files=$(shell find ./src -type f -name \*c -not -name test_\*c -not -name main.c -printf '%p ')
o_files=$(addprefix ./obj/,$(addsuffix .o,$(basename $(notdir $(c_files)))))

test_files=$(shell find ./src -type f -name test_\*c -printf '%p ')
test_o_files=$(addprefix ./test/,$(basename $(notdir $(test_files))))


LatticeGas: ./obj/main.o $(o_files)
	gcc $(CFLAGS) $^ $(LIBS) -o $@

./obj/%.o: ./src/%.c  | ./obj/
	gcc $(CFLAGS) -c $< -I ./src $(LIBS) -o $@
	
./src/%.c: ./src/%.h 

./test/%: ./src/%.c $(o_files) | ./test/
	gcc $(CFLAGS) $^ $(LIBS) -o $@

.PHONY: runAll
runAll: LatticeGas
	./LatticeGas D2Q6
	./LatticeGas D2Q7
	./LatticeGas D2Q8
	./LatticeGas D2Q9

./gifs/%.gif: LatticeGas
	./LatticeGas $*
	convert -delay 20 -loop 0 `find ./pics -name "$*_*" | sort -V` ./gifs/$*.gif


.PHONY: test
test: $(test_o_files)
	for i in $(test_o_files); do $$i; done
	

.PHONY: grind
grind: LatticeGas
	valgrind ./LatticeGas

.PHONY: clean
clean: 
	rm -rf ./obj/*
	rm -rf ./test/*
	rm -rf ./LatticeGas

.PHONY: cleanPics
cleanPics: 
	rm -f ./pics/*
	rm -f ./gifs/*

