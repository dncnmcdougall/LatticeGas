#include <stdio.h>
#include <error.h>

#include  "Field.h"
#include  "HexCell.h"
#include  "test.h"
#include  "Util.h"

void test_computeNewCell() {
    Field* pField = NULL;
    pField = createField(3, 3);
    printFieldInHex(pField);

    int newCell = 0;

    newCell = hex_computeNewCell(1,1,pField);
    assert_eq( newCell, 0);

    pField->field[1][0] = 8;

    printFieldInHex(pField);
    newCell = hex_computeNewCell(1,1,pField);
    assert_eq( newCell, 1);

    pField->field[0][1] = 16;

    printFieldInHex(pField);
    newCell = hex_computeNewCell(1,1,pField);
    assert_eq( newCell, 3);

    pField->field[0][2] = 32;

    printFieldInHex(pField);
    newCell = hex_computeNewCell(1,1,pField);
    assert_eq( newCell, 7);

    pField->field[1][2] = 1;

    printFieldInHex(pField);
    newCell = hex_computeNewCell(1,1,pField);
    assert_eq( newCell, 15);

    pField->field[2][2] = 2;

    printFieldInHex(pField);
    newCell = hex_computeNewCell(1,1,pField);
    assert_eq( newCell, 31);

    pField->field[2][1] = 4;

    printFieldInHex(pField);
    newCell = hex_computeNewCell(1,1,pField);
    assert_eq( newCell, 63);

    freeField(pField);
}

void test_computeNewCell2() {
    Field* pField = NULL;
    pField = createField(3, 3);
    printFieldInHex(pField);

    int newCell = 0;

    newCell = hex_computeNewCell(1,1,pField);
    assert_eq( newCell, 0);

    pField->field[1][0] = 63;

    printFieldInHex(pField);
    newCell = hex_computeNewCell(1,1,pField);
    assert_eq( newCell, 1);

    pField->field[0][1] = 63;

    printFieldInHex(pField);
    newCell = hex_computeNewCell(1,1,pField);
    assert_eq( newCell, 3);

    pField->field[0][2] = 63;

    printFieldInHex(pField);
    newCell = hex_computeNewCell(1,1,pField);
    assert_eq( newCell, 7);

    pField->field[1][2] = 63;

    printFieldInHex(pField);
    newCell = hex_computeNewCell(1,1,pField);
    assert_eq( newCell, 15);

    pField->field[2][2] = 63;

    printFieldInHex(pField);
    newCell = hex_computeNewCell(1,1,pField);
    assert_eq( newCell, 31);

    pField->field[2][1] = 63;

    printFieldInHex(pField);
    newCell = hex_computeNewCell(1,1,pField);
    assert_eq( newCell, 63);

    freeField(pField);
}

void test_computeCollision() {

    hex_initialiseCellState();

    double test = 0.0;
    for( int i =0 ; i < 64; ++i ) {
        for( test = 0.0; test< 1.0; test += 0.1) {
            Vector v = hex_computeMomentumFromHex( i );
            // v.x = -v.x;
            // v.y = -v.y;
            int bitCount = countBits(i);
            int result = hex_computeCollision(i, test);
            Vector resultV = hex_computeMomentumFromHex( result );
            int resultBitCount = countBits(result);
            if ( (resultBitCount != bitCount)
                && !areSame(&v, &resultV)
               ) {
                error(1, 0000, "Error: %f\n input %i, |h|: %i, v: (%i,%i)\n output %i, |h|: %i, v: (%i,%i)\n", 
                      test, i, bitCount, v.x, v.y,result, resultBitCount, resultV.x, resultV.y);
            }
            else {
                printf("Success: %f\n input %i, |h|: %i, v: (%i,%i)\n output %i, |h|: %i, v: (%i,%i)\n", 
                       test, i, bitCount, v.x, v.y,result, resultBitCount, resultV.x, resultV.y);
            }
        }

    }

    hex_finaliseCellState();
}

int main(int argc, char** argv) {

    // test_computeNewCell();
    // test_computeNewCell2();
    // test_computeCollision();

}
