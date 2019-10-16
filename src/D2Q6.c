#include "D2Q6.h"

#include  "Util.h"

#include <math.h>
#include <error.h>
#include <stdlib.h>
#include <stdio.h>

Vector d2q6_computeMomentumFromHex(int value);
int d2q6_getCell1(int r, int c, Field* pField);
int d2q6_getCell2(int r, int c, Field* pField);
int d2q6_getCell3(int r, int c, Field* pField);
int d2q6_getCell4(int r, int c, Field* pField);
int d2q6_getCell5(int r, int c, Field* pField);
int d2q6_getCell6(int r, int c, Field* pField);

MomentumList d2q6_listHeads[7];

void d2q6_initialiseCellState() {
    Vector v;
    int i=0;
    int bitCount=0;

    for( i = 0; i < 7; ++i ) {
        d2q6_listHeads[i].next = NULL;
        d2q6_listHeads[i].prev = NULL;
        d2q6_listHeads[i].count = 0;
        d2q6_listHeads[i].v.x = 0;
        d2q6_listHeads[i].v.y = 0;
    }


    MomentumList* pCurrent;

    for ( i = 0 ; i < 64; ++i ) {
        v = d2q6_computeMomentumFromHex( i );

        bitCount = countBits(i);

        pCurrent = &d2q6_listHeads[bitCount];

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

    for ( int j = 0; j < 7; ++j ) {
        printf(" ===== %i =====\n", j);
        pCurrent = &d2q6_listHeads[j];
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

void d2q6_finaliseCellState() {
    // MomentumList* pCurrent;
    // int j;
    // for ( j = 0; j < 6; ++j ) {
    //     pCurrent = d2q6_listHeads[j].next;
    //     while ( pCurrent != NULL ) {
    //         pCurrent = pCurrent->next;
    //         if ( pCurrent->prev ) {
    //             free( pCurrent->prev );
    //         }
    //     }
    // }
}

int d2q6_computeCollision(int inputHex, double random) {
    Vector v = d2q6_computeMomentumFromHex( inputHex );
    v.x = -v.x;
    v.y = -v.y;
    int bitCount = countBits(inputHex);
    MomentumList* pCurrent = &d2q6_listHeads[bitCount];

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

int d2q6_computeNewCell(int r, int c, Field* pField) {
    int cell1 = d2q6_getCell1(r, c, pField);
    int cell2 = d2q6_getCell2(r, c, pField);
    int cell3 = d2q6_getCell3(r, c, pField);
    int cell4 = d2q6_getCell4(r, c, pField);
    int cell5 = d2q6_getCell5(r, c, pField);
    int cell6 = d2q6_getCell6(r, c, pField);

    // printf( "1:%d, 2:%d, 3:%d, 4:%d, 5:%d, 6:%d\n", cell1, cell2, cell3, cell4, cell5, cell6);

    return
        (  (( cell1 &  8) >> 3 )
         | (( cell2 & 16) >> 3 )
         | (( cell3 & 32) >> 3 )
         | (( cell4 &  1) << 3 )
         | (( cell5 &  2) << 3 )
         | (( cell6 &  4) << 3 )
        );

}


Vector d2q6_computeMomentum(int r, int c, Field* pField) {
    int value = pField->field[r][c];

    return d2q6_computeMomentumFromHex( value );
}

// ----- PRIVATE METHODS -----

Vector d2q6_computeMomentumFromHex( int value ) {
    Vector v;
    v.x= 0;
    v.y= 0;

    v.x -= 100*(( value &  1) );
    v.x -=  50*(( value &  2) >> 1 );
    v.x +=  50*(( value &  4) >> 2 );
    v.x += 100*(( value &  8) >> 3 );
    v.x +=  50*(( value & 16) >> 4 );
    v.x -=  50*(( value & 32) >> 5 );

    // v.y -=   0*(( value &  1) );
    v.y +=  87*(( value &  2) >> 1 );
    v.y +=  87*(( value &  4) >> 2 );
    // v.y +=   0*(( value &  8) >> 3 );
    v.y -=  87*(( value & 16) >> 4 );
    v.y -=  87*(( value & 32) >> 5 );

    // if ( r == c ) printf("Mom: %i: (%i, %i): (x: %i, y: %i)\n", value, r,c,v.x,v.y);

    return v;
}
int d2q6_getCell1(int r, int c, Field* pField) {
    if ( c < 1 ) {
        return 0;
    } else {
        return pField->field[r][c-1];
    }
}
int d2q6_getCell4(int r, int c, Field* pField) {
    if ( c > pField->nCols-2 ) {
        return 0;
    } else {
        return pField->field[r][c+1];
    }
}
int d2q6_getCell2(int r, int c, Field* pField) {
    if ( r < 1 ) {
        return 0;
    } else {
        if ( (r & 1) == 0 ) {
            return pField->field[r-1][c];
        } else {
            if ( c < 1 ) {
                return 0;
            } else {
                return pField->field[r-1][c-1];
            }
        }
    }
}
int d2q6_getCell6(int r, int c, Field* pField) {
    if ( r > pField->nRows-2 ) {
        return 0;
    } else {
        if ( (r & 1) == 0 ) {
            return pField->field[r+1][c];
        } else {
            if ( c < 1 ) {
                return 0;
            } else {
                return pField->field[r+1][c-1];
            }
        }
    }
}
int d2q6_getCell3(int r, int c, Field* pField) {
    if ( r < 1 ) {
        return 0;
    } else {
        if ( (r & 1) == 0 ) {
            if ( c > pField->nCols-2 ) {
                return 0;
            } else {
                return pField->field[r-1][c+1];
            }
        } else {
            return pField->field[r-1][c];
        }
    }
}
int d2q6_getCell5(int r, int c, Field* pField) {
    if ( r > pField->nRows-2 ) {
        return 0;
    } else {
        if ( (r & 1) == 0 ) {
            if ( c > pField->nCols-2 ) {
                return 0;
            } else {
                return pField->field[r+1][c+1];
            }
        } else {
            return pField->field[r+1][c];
        }
    }
}


