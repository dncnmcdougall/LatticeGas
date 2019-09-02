#include <stdio.h>
#include "Field.h"
#include "TriCell.h"

int main(int argc, char** argv) {

    validateTriCellState();

    int nRows = 5;
    int nCols = 5;

    Field* pField1 = createField(nRows, nCols);
    Field* pField2 = createField(nRows, nCols);

    pField1->field[1][1] = 7;

    int itt = 0;
    int r = 0, c = 0;

    for( itt = 0; itt < 5; ++itt) {

        printf("===== Iteration: %i =====\n",itt);
        printf("----- Field 1 -----\n");
        printFieldInHex(pField1);

        for ( r = 0; r < nRows; ++r) {
            for ( c = 0; c < nCols; ++c) {
                pField2->field[r][c] = computeCollision( pField1->field[r][c] );
            }
        }
        printf("===== After collisions =====\n");
        printf("----- Field 2 -----\n");
        printFieldInHex(pField2);
        for ( r = 0; r < nRows; ++r) {
            for ( c = 0; c < nCols; ++c) {
                pField1->field[r][c] = computeNewCell( r,c, pField2);
            }
        }
    }

    freeField(pField1);
    freeField(pField2);


}
