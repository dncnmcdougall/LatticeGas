#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Field.h"
#include "TriCell.h"
#include "Boundary.h"

// #define HEX
#ifdef HEX
    #include "HexCell.h"
    #define MACRO_initialiseCellState hex_initialiseCellState
    #define MACRO_computeCollision hex_computeCollision
    #define MACRO_computeNewCell hex_computeNewCell
    #define MACRO_computeMomentum hex_computeMomentum
    #define MACRO_finaliseCellState hex_finaliseCellState
#else
    #include "D2Q9.h"
    #define MACRO_initialiseCellState d2q9_initialiseCellState
    #define MACRO_computeCollision d2q9_computeCollision
    #define MACRO_computeNewCell d2q9_computeNewCell
    #define MACRO_computeMomentum d2q9_computeMomentum
    #define MACRO_finaliseCellState d2q9_finaliseCellState
#endif

int nRows, nCols;

CellType getCellType(int r, int c) {
    // if (  r == 0 || r == (nRows-1) ) {
    //     return Mirror;
    // } else 
        if ( 
                  (r > 8*nRows/17) 
               && (c > 8*nCols/17) 
               && (r < 9*nRows/17) 
               && (c < 9*nCols/17)
               ) {
        return Mirror;
    } else if ( c == 0 ) {
        return Fixed;
    } else {
        return Flow;
    }
}

int getFixedValue(int r, int c) {
#ifdef HEX
    return 8;
#else
    return 32;
#endif
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
             int (*computeCollision)(int, double), 
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
                    pField2->field[r][c] = computeCollision( pField1->field[r][c] , drand48());
                    break;
            }
        }
    }
    printf("===== After collisions =====\n");
    printf("----- Field 2 -----\n");
    // printFieldInHex(pField2);
    for ( r = 0; r < nRows; ++r) {
        for ( c = 0; c < nCols; ++c) {
            pField1->field[r][c] = computeNewCell( r,c, pField2);
        }
    }
}


int main(int argc, char** argv) {

    Field* pField1 = NULL;
    Field* pField2 = NULL;

    init( MACRO_initialiseCellState, 200, 200, &pField1, &pField2);

    int itt = 0;

    char* filename = calloc(sizeof(unsigned char), 20);

    int r = 0;
    int c = 0;
    for ( r = 0; r < nRows; ++r) {
        for ( c = 0; c < nCols; ++c) {
            CellType ct = getCellType(r,c);
            switch( ct) {
                case Mirror:
                case Fixed:
                    break;
                case Flow:
#ifdef HEX
                    pField1->field[r][c] = 1;
#else
                    pField1->field[r][c] = 2;
#endif
                    break;
            }
        }
    }

    for( itt = 0; itt < 1000; ++itt) {
        iterate(itt, pField1, pField2, MACRO_computeCollision, MACRO_computeNewCell);

        if( itt%10 == 0 ) {
        sprintf(filename, "pics/Itt_%i.png", itt);

        drawField(pField2, filename, MACRO_computeMomentum, getCellType);
        }
    }

    MACRO_finaliseCellState();

    freeField(pField1);
    freeField(pField2);
}
