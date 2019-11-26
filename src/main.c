#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "Field.h"
#include "TriCell.h"
#include "Boundary.h"

#include "D2Q6.h"
#include "D2Q7.h"
#include "D2Q8.h"
#include "D2Q9.h"

typedef enum {
    D2Q6,
    D2Q7,
    D2Q8,
    D2Q9
} ElementType;

int nRows, nCols;

int fixedValue;
char* elementName;
void (*elm_initialiseCellState)();
int (*elm_computeCollision)(int, double);
int (*elm_computeNewCell)(int, int, Field*);
Vector (*elm_computeMomentum)(int,int,Field*);
void (*elm_finaliseCellState)();

void printUsage() {
    printf("Usage: LaticeGas [D2Q6|D2Q7|D2D8|D2Q9]");
    exit(1);
}

int isElement( ElementType elmType, char* elmStr) {
    switch ( elmType ) {
        case D2Q6:
            return strncmp(elmStr, "D2Q6",4) ==0 || strncmp(elmStr, "d2q6", 5) == 0;
        case D2Q7:
            return strncmp(elmStr, "D2Q7",4) ==0 || strncmp(elmStr, "d2q7", 5) == 0;
        case D2Q8:
            return strncmp(elmStr, "D2Q8",4) ==0 || strncmp(elmStr, "d2q8", 5) == 0;
        case D2Q9:
            return strncmp(elmStr, "D2Q9",4) ==0 || strncmp(elmStr, "d2q9", 5) == 0;
        default:
            return 0;
    }
}

void parseArgsAndChooseElement(int argc, char** argv) {
    if ( argc != 2 ) {
        printUsage();
    } else if ( isElement(D2Q6, argv[1]) ) {
        elementName = "D2Q6";
        fixedValue =  8;
        elm_initialiseCellState = &d2q6_initialiseCellState;
        elm_computeCollision = &d2q6_computeCollision;
        elm_computeNewCell = &d2q6_computeNewCell;
        elm_computeMomentum = &d2q6_computeMomentum;
        elm_finaliseCellState = &d2q6_finaliseCellState;
    } else if ( isElement(D2Q7, argv[1]) ) {
        elementName = "D2Q7";
        fixedValue =  16;
        elm_initialiseCellState = &d2q7_initialiseCellState;
        elm_computeCollision = &d2q7_computeCollision;
        elm_computeNewCell = &d2q7_computeNewCell;
        elm_computeMomentum = &d2q7_computeMomentum;
        elm_finaliseCellState = &d2q7_finaliseCellState;
    } else if ( isElement(D2Q8, argv[1]) ) {
        elementName = "D2Q8";
        fixedValue =  16;
        elm_initialiseCellState = &d2q8_initialiseCellState;
        elm_computeCollision = &d2q8_computeCollision;
        elm_computeNewCell = &d2q8_computeNewCell;
        elm_computeMomentum = &d2q8_computeMomentum;
        elm_finaliseCellState = &d2q8_finaliseCellState;
    } else if ( isElement(D2Q9, argv[1]) ) {
        elementName = "D2Q9";
        fixedValue =  32;
        elm_initialiseCellState = &d2q9_initialiseCellState;
        elm_computeCollision = &d2q9_computeCollision;
        elm_computeNewCell = &d2q9_computeNewCell;
        elm_computeMomentum = &d2q9_computeMomentum;
        elm_finaliseCellState = &d2q9_finaliseCellState;
    } else {
        printUsage();
    }
}




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
    return fixedValue;
    // return FIXED_VALUE;
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
    printf("===== Iteration: %s: %i =====\n",elementName, itt);

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
    for ( r = 0; r < nRows; ++r) {
        for ( c = 0; c < nCols; ++c) {
            pField1->field[r][c] = computeNewCell( r,c, pField2);
        }
    }
}

int main(int argc, char** argv) {

    parseArgsAndChooseElement(argc, argv);

    Field* pField1 = NULL;
    Field* pField2 = NULL;

    init( elm_initialiseCellState, 201, 201, &pField1, &pField2);

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

    for( itt = 0; itt < 250; ++itt) {
        iterate(itt, pField1, pField2, elm_computeCollision, elm_computeNewCell);

        if( itt%10 == 0 ) {
            sprintf(filename, "pics/%s_itt_%i.png", elementName, itt);

            drawField(pField2, filename, elm_computeMomentum, getCellType);
        }
    }

    elm_finaliseCellState();

    freeField(pField1);
    freeField(pField2);
}
