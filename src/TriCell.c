#include "TriCell.h"

#include <math.h>
#include <error.h>
#include <stdlib.h>
#include <stdio.h>

double transformations[][8] = {
/*        0  ,  1   ,  2   ,  3  ,  4   ,  5  ,  6  ,  7 */
/*0*/ { 1.0  ,0.0   ,0.0   ,0.0  ,0.0   ,0.0  ,0.0  ,0.0},
/*1*/ { 0.0  ,0.1   ,0.45  ,0.0  ,0.45  ,0.0  ,0.0  ,0.0},
/*2*/ { 0.0  ,0.45  ,0.1   ,0.0  ,0.45  ,0.0  ,0.0  ,0.0},
/*3*/ { 0.0  ,0.0   ,0.0   ,0.2  ,0.0   ,0.4  ,0.4  ,0.0},
/*4*/ { 0.0  ,0.45  ,0.45  ,0.0  ,0.1   ,0.0  ,0.0  ,0.0},
/*5*/ { 0.0  ,0.0   ,0.0   ,0.4  ,0.0   ,0.2  ,0.4  ,0.0},
/*6*/ { 0.0  ,0.0   ,0.0   ,0.4  ,0.0   ,0.4  ,0.2  ,0.0},
/*7*/ { 0.0  ,0.0   ,0.0   ,0.0  ,0.0   ,0.0  ,0.0  ,1.0}
};

void validateTriCellState() {
    int i = 0, j = 0;
    double rowSum=0.0;
    for( i = 0; i < 8; ++i) {
        rowSum = 0.0;
        for( j = 0; j < 8; ++j) {
            rowSum += transformations[i][j];
        }
        if ( fabs(rowSum - 1.0) > 1e-3 ) {
            error(1, 00000, "Error: row %i sums to %f which is > 1.0\n", i, rowSum);
            break;
        }
    }
}

int computeCollision(int inputHex) {
    int i = 0;
    double rowSum= drand48();
    for( i = 0; i < 8; ++i) {
        rowSum -= transformations[inputHex][i];
        if ( rowSum < 0.0 ) {
            return i;
        }
    }
    error(1, 00000, "Error: Trying to compute collision failed: input: %i\n", inputHex);
    return -1;
}
int getCellAbove(int r, int c, Field* pField) {
    if ( r < 1 ) {
        return 0;
    } else {
        return pField->field[r-1][c];
    }
}
int getCellBelow(int r, int c, Field* pField) {
    if ( r > pField->nRows-2 ) {
        return 0;
    } else {
        return pField->field[r+1][c];
    }
}
int getCellBehind(int r, int c, Field* pField) {
    if ( (r & 1) == 1 ) {
        if ( c > pField->nCols-2 ) {
            return 0;
        } else {
            return pField->field[r][c+1];
        }
    } else {
        if ( c < 1 ) {
            return 0;
        } else {
            return pField->field[r][c-1];
        }
    }
}

int computeNewCell(int r, int c, Field* pField) {
    int top = getCellAbove(r,c, pField);
    int bottom = getCellBelow(r,c, pField);
    int side = getCellBehind(r,c, pField);

    return 
          ((top & 4) >> 1) 
        | ((bottom & 2) << 1) 
        | (side & 1);
}
