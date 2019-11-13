#include "D2Q8.h"

#include  "Util.h"

#include <math.h>
#include <error.h>
#include <stdlib.h>
#include <stdio.h>

Vector d2q8_computeMomentumFromHex(int value);
int d2q8_getCell1(int r, int c, Field* pField);
int d2q8_getCell2(int r, int c, Field* pField);
int d2q8_getCell3(int r, int c, Field* pField);
int d2q8_getCell4(int r, int c, Field* pField);
int d2q8_getCell5(int r, int c, Field* pField);
int d2q8_getCell6(int r, int c, Field* pField);
int d2q8_getCell7(int r, int c, Field* pField);
int d2q8_getCell8(int r, int c, Field* pField);

MomentumList d2q8_listHeads[9];

void d2q8_initialiseCellState() {
    Vector v;
    int i=0;
    int bitCount=0;

    for( i = 0; i < 9; ++i ) {
        d2q8_listHeads[i].next = NULL;
        d2q8_listHeads[i].prev = NULL;
        d2q8_listHeads[i].count = 0;
        d2q8_listHeads[i].v.x = 0;
        d2q8_listHeads[i].v.y = 0;
    }


    MomentumList* pCurrent;

    for ( i = 0 ; i < 512; ++i ) {
        v = d2q8_computeMomentumFromHex( i );

        bitCount = countBits(i);

        pCurrent = &d2q8_listHeads[bitCount];

        if ( pCurrent->count == 0 ) {
            if ( pCurrent->next != NULL ) {
                error(1, 0000, "This should not happen\n");
            }
            pCurrent->count = 1;
            pCurrent->next  = NULL;
            pCurrent->prev  = pCurrent;
            pCurrent->v = v;
            pCurrent->indices = malloc(sizeof(int));
            pCurrent->indices[0] = i;
        } else {

            while ( pCurrent->next != NULL && !areSame( &pCurrent->v, &v) ) {
                pCurrent = pCurrent->next;
            }
            if ( areSame( &pCurrent->v, &v) ) {
                int count = ++pCurrent->count;
                pCurrent->indices = realloc( pCurrent->indices, sizeof(int)*count);
                pCurrent->indices[count-1] = i;
            } else if (pCurrent->next == NULL ){
                pCurrent->next = malloc(sizeof(MomentumList));
                pCurrent->next->count = 1;
                pCurrent->next->next  = NULL;
                pCurrent->next->prev  = pCurrent;
                pCurrent->next->v = v;
                pCurrent->next->indices = malloc(sizeof(int));
                pCurrent->next->indices[0] = i;
            } else {
                error(1, 0000, "This should not happen\n");
            }
        }
    }

    for ( int j = 0; j < 9; ++j ) {
        printf(" ===== %i =====\n", j);
        pCurrent = &d2q8_listHeads[j];
        printList( pCurrent);
        while ( pCurrent != NULL ) {
            for( i=0; i < pCurrent->count; ++i ) {
                int value = pCurrent->indices[i];
                if( countBits(value) != j ) {
                    error(1,0000,"Expected list item (%i)to have %i bits but found %i\n", value, j, countBits(value));
                }
            }
            pCurrent = pCurrent->next;
        }
    }
}

void d2q8_finaliseCellState() {
    // MomentumList* pCurrent;
    // int j;
    // for ( j = 0; j < 6; ++j ) {
    //     pCurrent = d2q8_listHeads[j].next;
    //     while ( pCurrent != NULL ) {
    //         pCurrent = pCurrent->next;
    //         if ( pCurrent->prev ) {
    //             free( pCurrent->prev );
    //         }
    //     }
    // }
}

int d2q8_computeCollision(int inputHex, double random) {
    Vector v = d2q8_computeMomentumFromHex( inputHex );
    v.x = -v.x;
    v.y = -v.y;
    int bitCount = countBits(inputHex);
    MomentumList* pCurrent = &d2q8_listHeads[bitCount];

    // printf("Compute collision: %i, |h|: %i, v: (%i,%i)\n", inputHex, bitCount, v.x, v.y);
    // printList(pCurrent);
    while ( pCurrent->next != NULL && !areSame( &pCurrent->v, &v) ) {
        pCurrent = pCurrent->next;
    }
    if ( !pCurrent ) {
        error(1, 0000, "This should not happen.\n");
    }

    int i = 0;
    int rowSum= (int)round(random*(pCurrent->count-1));
    // printf("row sum: %i\n",rowSum);
    for ( i = 0; i < rowSum; ++i ) {};
    return pCurrent->indices[i];
}

int d2q8_computeNewCell(int r, int c, Field* pField) {
    int cell1 = d2q8_getCell1(r, c, pField);
    int cell2 = d2q8_getCell2(r, c, pField);
    int cell3 = d2q8_getCell3(r, c, pField);
    int cell4 = d2q8_getCell4(r, c, pField);
    int cell5 = d2q8_getCell5(r, c, pField);
    int cell6 = d2q8_getCell6(r, c, pField);
    int cell7 = d2q8_getCell7(r, c, pField);
    int cell8 = d2q8_getCell8(r, c, pField);

    // printf( "0:%d 1:%d, 2:%d, 3:%d, 4:%d, 5:%d, 6:%d\n", cell0, cell1, cell2, cell3, cell4, cell5, cell6);

    return
        (  (( cell1 &   16) >> 4 )
         | (( cell2 &   32) >> 4 )
         | (( cell3 &   64) >> 4 )
         | (( cell4 &  128) >> 4 )
         | (( cell5 &    1) << 4 )
         | (( cell6 &    2) << 4 )
         | (( cell7 &    4) << 4 )
         | (( cell8 &    8) << 4 )
        );
}


Vector d2q8_computeMomentum(int r, int c, Field* pField) {
    int value = pField->field[r][c];

    return d2q8_computeMomentumFromHex( value );
}

// ----- PRIVATE METHODS -----

Vector d2q8_computeMomentumFromHex( int value ) {
    Vector v;
    v.x = 0;
    v.y = 0;

    v.x -= 50*(( value &   1) );
    v.x -= 50*(( value &   2) >> 1 );
    // v.x +=   0*(( value &   4) >> 2 );
    v.x += 50*(( value &   8) >> 3 );
    v.x += 50*(( value &  16) >> 4 );
    v.x += 50*(( value &  32) >> 5 );
    // v.x +=   0*(( value & 64) >> 6 );
    v.x -= 50*(( value & 128) >> 7 );

    // v.y +=   0*(( value &   1) );
    v.y += 50*(( value &   2) >> 1 );
    v.y += 50*(( value &   4) >> 2 );
    v.y += 50*(( value &  8) >> 3 );
    // v.y +=   0*(( value &  16) >> 4 );
    v.y -= 50*(( value &  32) >> 5 );
    v.y -= 50*(( value &  64) >> 6 );
    v.y -= 50*(( value & 128) >> 7 );

    // if ( r == c ) printf("Mom: %i: (%i, %i): (x: %i, y: %i)\n", value, r,c,v.x,v.y);

    return v;
}
int d2q8_getCell1(int r, int c, Field* pField) {
    if ( c < 1 ) {
        return 0;
    } else {
        return pField->field[r][c-1];
    }
}
int d2q8_getCell2(int r, int c, Field* pField) {
    if ( r < 1 ) {
        return 0;
    } else if ( c < 1 ) {
        return 0;
    } else {
        return pField->field[r-1][c-1];
    }
}
int d2q8_getCell3(int r, int c, Field* pField) {
    if ( r < 1 ) {
        return 0;
    } else {
        return pField->field[r-1][c];
    }
}
int d2q8_getCell4(int r, int c, Field* pField) {
    if ( r < 1 ) {
        return 0;
    } else if ( c > pField->nCols-2 ) {
        return 0;
    } else {
        return pField->field[r-1][c+1];
    }
}
int d2q8_getCell5(int r, int c, Field* pField) {
    if ( c > pField->nCols-2 ) {
        return 0;
    } else {
        return pField->field[r][c+1];
    }
}
int d2q8_getCell6(int r, int c, Field* pField) {
    if ( r > pField->nRows-2 ) {
        return 0;
    } else if ( c > pField->nCols-2 ) {
        return 0;
    } else {
        return pField->field[r+1][c+1];
    }
}
int d2q8_getCell7(int r, int c, Field* pField) {
    if ( r > pField->nRows-2 ) {
        return 0;
    } else {
        return pField->field[r+1][c];
    }
}
int d2q8_getCell8(int r, int c, Field* pField) {
    if ( r > pField->nRows-2 ) {
        return 0;
    } else if ( c < 1 ) {
        return 0;
    } else {
        return pField->field[r+1][c-1];
    }
}
