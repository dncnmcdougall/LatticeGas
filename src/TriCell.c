#include "TriCell.h"

#include <math.h>
#include <error.h>
#include <stdlib.h>
#include <stdio.h>

int getCellAbove(int r, int c, Field* pField);
int getCellBelow(int r, int c, Field* pField);
int getCellBehind(int r, int c, Field* pField);

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

void tri_validateCellState() {
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

int tri_computeCollision(int inputHex) {
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

int tri_computeNewCell(int r, int c, Field* pField) {
    int top = getCellAbove(r,c, pField);
    int bottom = getCellBelow(r,c, pField);
    int side = getCellBehind(r,c, pField);

    return 
          ((top & 4) >> 1) 
        | ((bottom & 2) << 1) 
        | (side & 1);
}

Vector tri_computeMomentum(int r, int c, Field* pField) {
    Vector v;
    v.x= 0;
    v.y= 0;

    int value = pField->field[r][c];

    v.x -= (value&1)*100;
    v.x += ((value&2)>>1)*50;
    v.x += ((value&4)>>2)*50;

    // v.y += (value&1)*0;
    v.y += ((value&2)>>1)*87;
    v.y -= ((value&4)>>2)*87;

    if ( ((r ^ c) & 1) == 0 ) {
        v.x = -v.x;
    }

    // if ( r == c ) printf("Mom: %i: (%i, %i): (x: %i, y: %i)\n", value, r,c,v.x,v.y);

    return v;
}

// ----- PRIVATE METHODS -----

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
    if ( ((r ^ c) & 1) == 0 ) {
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
