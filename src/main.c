#include <stdio.h>
#include <stdlib.h>

#include "Field.h"
#include "TriCell.h"
#include "HexCell.h"
#include "Boundary.h"

int nRows, nCols;

CellType getCellType(int r, int c) {
    if (  r == 0 || r == (nRows-1) ) {
        return Mirror;
    } else if ( 
                  (r > nRows/4 ) 
               && (c > nCols/4) 
               && (r < 3*nRows/4) 
               && (c < 3*nCols/4)
               ) {
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


void init( void (*initFunc)(), int lnRows, int lnCols, 
          Field** pField1, Field** pField2) {

    initFunc();

    nRows = lnRows;
    nCols = lnCols;

    (*pField1) = createField(nRows, nCols);
    (*pField2) = createField(nRows, nCols);
}

void iterate( int itt, Field* pField1, Field* pField2,
             int (*computeCollision)(int), 
             int (*computeNewCell)(int, int, Field*)
            ) {

    int r = 0, c = 0;
    printf("===== Iteration: %i =====\n",itt);
    printf("----- Field 1 -----\n");
    // printFieldInHex(pField1);

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
                    pField2->field[r][c] = tri_computeCollision( pField1->field[r][c] );
                    break;
            }
        }
    }
    printf("===== After collisions =====\n");
    printf("----- Field 2 -----\n");
    // printFieldInHex(pField2);
    for ( r = 0; r < nRows; ++r) {
        for ( c = 0; c < nCols; ++c) {
            pField1->field[r][c] = tri_computeNewCell( r,c, pField2);
        }
    }
}


int main(int argc, char** argv) {

    Field* pField1 = NULL;
    Field* pField2 = NULL;

    hex_initialiseCellState();

    init( tri_initialiseCellState, 20, 20, &pField1, &pField2);


    int itt = 0;

    char* filename = calloc(sizeof(unsigned char), 15);

    for( itt = 0; itt < 1; ++itt) {
        iterate(itt, pField1, pField2, tri_computeCollision, tri_computeNewCell);
        sprintf(filename, "pics/Itt_%i.png", itt);
        drawField(pField2, filename, tri_computeMomentum, getCellType);
    }

    tri_finaliseCellState();

    freeField(pField1);
    freeField(pField2);
}
