CFLAGS=-Wall -Wpedantic -Werror -Wno-comment

c_files=$(shell find ./src -type f -name \*c -printf '%p ')
o_files=$(addprefix ./obj/,$(addsuffix .o,$(basename $(notdir $(c_files)))))


LatticeGas: $(o_files)
	gcc $^ -o $@

./obj/%.o: ./src/%.c 
	gcc $(CFLAGS) -c $< -I ./src -o $@
	
./src/%.c: ./src/%.h 

.PHONY: run
run: LatticeGas
	./LatticeGas

.PHONY: clean
clean: 
	rm -r ./obj/*
	rm -r ./LatticeGas

