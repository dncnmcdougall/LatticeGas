#include <stdio.h>
#include "Field.h"
#include "TriCell.h"
#include "Boundary.h"

int nRows, nCols;

CellType getCellType(int r, int c) {
    if (  r == 0 || r == (nRows-1) ) {
        return Mirror;
    } else if ( (r & 1) == 1 && c == 0 ) {
        return Fixed;
    } else {
        return Flow;
    }
}

int getFixedValue(int r, int c) {
    return 6;
}

int main(int argc, char** argv) {

    validateTriCellState();

    nRows = 7;
    nCols = 7;

    Field* pField1 = createField(nRows, nCols);
    Field* pField2 = createField(nRows, nCols);

    int itt = 0;
    int r = 0, c = 0;

    for( itt = 0; itt < 5; ++itt) {

        printf("===== Iteration: %i =====\n",itt);
        printf("----- Field 1 -----\n");
        printFieldInHex(pField1);

        for ( r = 0; r < nRows; ++r) {
            for ( c = 0; c < nCols; ++c) {
                CellType ct = getCellType(r,c);
                switch( ct) {
                    case Mirror:
                        pField2->field[r][c] = pField1->field[r][c];
                        break;
                    case Fixed:
                        pField2->field[r][c] = getFixedValue(r,c);
                        break;
                    case Flow:
                        pField2->field[r][c] = computeCollision( pField1->field[r][c] );
                        break;
                }
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
