#include <stdio.h>
#include <error.h>

#include  "Field.h"
#include  "D2Q9.h"
#include  "test.h"
#include  "Util.h"

void test_computeNewCell() {
    Field* pField = NULL;
    pField = createField(3, 3);
    printFieldInHex(pField);

    int newCell = 0;

    newCell = d2q9_computeNewCell(1,1,pField);
    assert_eq( newCell, 0);

    pField->field[1][1] = 1;

    printFieldInHex(pField);
    newCell = d2q9_computeNewCell(1,1,pField);
    assert_eq( newCell, 1);

    pField->field[1][0] = 32;

    printFieldInHex(pField);
    newCell = d2q9_computeNewCell(1,1,pField);
    assert_eq( newCell, 3);

    pField->field[0][0] = 64;

    printFieldInHex(pField);
    newCell = d2q9_computeNewCell(1,1,pField);
    assert_eq( newCell, 7);

    pField->field[0][1] = 128;

    printFieldInHex(pField);
    newCell = d2q9_computeNewCell(1,1,pField);
    assert_eq( newCell, 15);

    pField->field[0][2] = 256;

    printFieldInHex(pField);
    newCell = d2q9_computeNewCell(1,1,pField);
    assert_eq( newCell, 31);

    pField->field[1][2] = 2;

    printFieldInHex(pField);
    newCell = d2q9_computeNewCell(1,1,pField);
    assert_eq( newCell, 63);

    pField->field[2][2] = 4;

    printFieldInHex(pField);
    newCell = d2q9_computeNewCell(1,1,pField);
    assert_eq( newCell, 127);

    pField->field[2][1] = 8;

    printFieldInHex(pField);
    newCell = d2q9_computeNewCell(1,1,pField);
    assert_eq( newCell, 255);

    pField->field[2][0] = 16;

    printFieldInHex(pField);
    newCell = d2q9_computeNewCell(1,1,pField);
    assert_eq( newCell, 511);

    freeField(pField);
}

void test_computeNewCell2() {
    Field* pField = NULL;
    pField = createField(3, 3);
    printFieldInHex(pField);

    int newCell = 0;

    newCell = d2q9_computeNewCell(1,1,pField);
    assert_eq( newCell, 0);

    pField->field[1][1] = 510;

    printFieldInHex(pField);
    newCell = d2q9_computeNewCell(1,1,pField);
    assert_eq( newCell, 0);

    pField->field[1][0] = 479;

    printFieldInHex(pField);
    newCell = d2q9_computeNewCell(1,1,pField);
    assert_eq( newCell, 0);

    pField->field[0][0] = 447;

    printFieldInHex(pField);
    newCell = d2q9_computeNewCell(1,1,pField);
    assert_eq( newCell, 0);

    pField->field[0][1] = 383;

    printFieldInHex(pField);
    newCell = d2q9_computeNewCell(1,1,pField);
    assert_eq( newCell, 0);

    pField->field[0][2] = 255;

    printFieldInHex(pField);
    newCell = d2q9_computeNewCell(1,1,pField);
    assert_eq( newCell, 0);

    pField->field[1][2] = 509;

    printFieldInHex(pField);
    newCell = d2q9_computeNewCell(1,1,pField);
    assert_eq( newCell, 0);

    pField->field[2][2] = 507;

    printFieldInHex(pField);
    newCell = d2q9_computeNewCell(1,1,pField);
    assert_eq( newCell, 0);

    pField->field[2][1] = 503;

    printFieldInHex(pField);
    newCell = d2q9_computeNewCell(1,1,pField);
    assert_eq( newCell, 0);

    pField->field[2][0] = 495;

    printFieldInHex(pField);
    newCell = d2q9_computeNewCell(1,1,pField);
    assert_eq( newCell, 0);

    freeField(pField);
}

void test_computeCollision() {

    d2q9_initialiseCellState();

    double test = 0.0;
    for( int i =0 ; i < 512; ++i ) {
        for( test = 0.0; test< 1.0; test += 0.1) {
            Vector v = d2q9_computeMomentumFromHex( i );
            // v.x = -v.x;
            // v.y = -v.y;
            int bitCount = countBits(i);
            int result = d2q9_computeCollision(i, test);
            Vector resultV = d2q9_computeMomentumFromHex( result );
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

    d2q9_finaliseCellState();
}

void test_computeMomentum() {
    Vector test;

    test = d2q9_computeMomentumFromHex(1);
    assert_eq(test.x, 0);
    assert_eq(test.y, 0);

    test = d2q9_computeMomentumFromHex(2);
    assert_eq(test.x, -50);
    assert_eq(test.y, 0);

    test = d2q9_computeMomentumFromHex(4);
    assert_eq(test.x, -50);
    assert_eq(test.y, 50);

    test = d2q9_computeMomentumFromHex(8);
    assert_eq(test.x, 0);
    assert_eq(test.y, 50);

    test = d2q9_computeMomentumFromHex(16);
    assert_eq(test.x, 50);
    assert_eq(test.y, 50);

    test = d2q9_computeMomentumFromHex(32);
    assert_eq(test.x, 50);
    assert_eq(test.y, 0);

    test = d2q9_computeMomentumFromHex(64);
    assert_eq(test.x, 50);
    assert_eq(test.y, -50);

    test = d2q9_computeMomentumFromHex(128);
    assert_eq(test.x, 0);
    assert_eq(test.y, -50);

    test = d2q9_computeMomentumFromHex(256);
    assert_eq(test.x, -50);
    assert_eq(test.y, -50);
}

void test_computeMomentum2() {
    Vector test;

    test = d2q9_computeMomentumFromHex(1|2);
    assert_eq(test.x, -50);
    assert_eq(test.y, 0);

    test = d2q9_computeMomentumFromHex(4|64);
    assert_eq(test.x, 0);
    assert_eq(test.y, 0);

    test = d2q9_computeMomentumFromHex(16|32);
    assert_eq(test.x, 100);
    assert_eq(test.y, 50);

    test = d2q9_computeMomentumFromHex(4|16|64);
    assert_eq(test.x, 50);
    assert_eq(test.y, 50);

    test = d2q9_computeMomentumFromHex(1|4|16|64);
    assert_eq(test.x, 50);
    assert_eq(test.y, 50);

}

int main(int argc, char** argv) {

    // test_computeNewCell2();
    test_computeCollision();
    // test_computeMomentum2();

}
