#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Field.h"
#include "TriCell.h"
#include "Boundary.h"

#define D2Q7
#ifdef D2Q6
#include "D2Q6.h"
#define FIXED_VALUE 8
#define MACRO_initialiseCellState d2q6_initialiseCellState
#define MACRO_computeCollision d2q6_computeCollision
#define MACRO_computeNewCell d2q6_computeNewCell
#define MACRO_computeMomentum d2q6_computeMomentum
#define MACRO_finaliseCellState d2q6_finaliseCellState
#endif
#ifdef D2Q7
#include "D2Q7.h"
#define FIXED_VALUE 16
#define MACRO_initialiseCellState d2q7_initialiseCellState
#define MACRO_computeCollision d2q7_computeCollision
#define MACRO_computeNewCell d2q7_computeNewCell
#define MACRO_computeMomentum d2q7_computeMomentum
#define MACRO_finaliseCellState d2q7_finaliseCellState
#endif
#ifdef D2Q9
#include "D2Q9.h"
#define FIXED_VALUE 32
#define MACRO_initialiseCellState d2q9_initialiseCellState
#define MACRO_computeCollision d2q9_computeCollision
#define MACRO_computeNewCell d2q9_computeNewCell
#define MACRO_computeMomentum d2q9_computeMomentum
#define MACRO_finaliseCellState d2q9_finaliseCellState
#endif

int nRows, nCols;

CellType getCellType(int r, int c, int itt) {
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

int getFixedValue(int r, int c, int itt) {
    return FIXED_VALUE;
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
            CellType ct = getCellType(r,c, itt);
            switch( ct) {
                case Mirror:
                    pField2->field[r][c] = pField1->field[r][c];
                    break;
                case Fixed:
                    pField2->field[r][c] = getFixedValue(r,c, itt);
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

    init( MACRO_initialiseCellState, 201, 201, &pField1, &pField2);

    int itt = 0;

    char* filename = calloc(sizeof(unsigned char), 20);

    int r = 0;
    int c = 0;
    for ( r = 0; r < nRows; ++r) {
        for ( c = 0; c < nCols; ++c) {
            CellType ct = getCellType(r,c, 0);
            switch( ct) {
                case Mirror:
                case Fixed:
                    break;
                case Flow:
                    pField1->field[r][c] = 0;
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
