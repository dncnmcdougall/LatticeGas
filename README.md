# LatticeGas

This is a simple implementation of a [lattice gas automation](https://en.wikipedia.org/wiki/Lattice_gas_automaton) simulation.
The output is written to png.

This was written for two reasons. 1. I have wanted to implement a lattice gas
for some time. 2. This serves as an exhibit of my c coding style.


## Elements
D2Q6, D2Q7, D2Q8 and D2Q9 (2D with 9 states) elements are implemented. Which element is in
use is changed by parsing in a different command line argument.
## D2Q6
![D2Q6](./gifs/D2Q6_small.gif)
## D2Q7
![D2Q7](./gifs/D2Q7_small.gif)
## D2Q8
![D2Q8](./gifs/D2Q8_small.gif)
## D2Q9
![D2Q9](./gifs/D2Q9_small.gif)

## Structure:
* _src_ folder hold all the source. 
* _src/main.c_ is the entry point.
* _obj/_ houses all the object files. It needs to exist.
* _test/_ houses the test executables. It needs to exist for testing.
* _pics/_ houses the output pictures
* _gifs/_ houses the output gifs. Made with the ImageMagick's convert utility.
* _Makefile_ is the file that defines the build commands.
* _LatticeGas_ is the executable that will be built.

## Requirements:
* _gcc_ is used to build.
* _libpng_ is used and expects it in the normal build paths.
* _make_ is used as the build system.
* _ImageMagick_ is used to make the gifs.

## Testing
Testing is very rudimentary and does not use any external libraries.
Each _src/test\_*.c_ file is build as an executable in _test/_ and then
executed. If it returnes a non-zero exit code, the test is assumed to have
failed.
A shortcomming is that the next test will still be run.

Done with _make test_

## Usage
The default make command is the _LatticeGas_ executable. This is the main
program. When executed it will reun a lattice gas simulation up to the given
number of iterations and print the output as png files in the _pics_ folder.

The structure that is simulated is defined in the _sec/main.c_. In particular
each cell is defined as either _Mirror_, _Fixed_ or _Flow. 
The type of a cell is queried through the _getCellType_ function. 

### Mirror
Mirror cells simply reflect any input back the way it came. These should be used
for no flow boundaries. (They violate the concervation of momentum).
### Fixed
Fixed cells always have the same value in all iterations. These should be used
for inlets and outlets. (They violate the concervation of momentum and mass.)

The value of fixed cells is determined by the value returned _getFixedValue_
function.
### Flow
Flow cells are normal cells that propogate and collide bits in them conserving
both mass (number of bits) and momentum.

## Extensions
There is no damping. There is no damping of the momentum, so the wake of
an object becomes infinitly wide, given enough time. Obviously this is
non-physical. Damping should be introduced. However this si a bit tricky as all
bits are binary values and so introducing damping while maintaining the binary
bits is a difficult problem. Any ideas would be welcome.

