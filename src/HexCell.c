#include "HexCell.h"

#include <math.h>
#include <error.h>
#include <stdlib.h>
#include <stdio.h>

Vector computeMomentum(int value);
int getCell1(int r, int c, Field* pField);
int getCell2(int r, int c, Field* pField);
int getCell3(int r, int c, Field* pField);
int getCell4(int r, int c, Field* pField);
int getCell5(int r, int c, Field* pField);
int getCell6(int r, int c, Field* pField);

typedef struct momList MomentumList;

struct momList {
    Vector v;
    MomentumList* next;
    MomentumList* prev;
    int count;
    int* indices;
};

MomentumList listHead;

void hex_initialiseCellState() {
    Vector v;

    listHead.next = NULL;
    listHead.prev = NULL;
    listHead.count = 0;
    listHead.v.x = 0;
    listHead.v.y = 0;

    MomentumList* pCurrent;

    int i = 0;
    for ( i = 0 ; i < 64; ++i ) {
        v = computeMomentum( i );
        pCurrent = &listHead;

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
            printf("created new block");
        } else {
            error(1, 0000, "This should not happen\n");
        }
    }

    pCurrent = &listHead;
    while ( pCurrent->next != NULL ) {
        printf(" list:\n c: %p p: %p, n %p\n v: (%i, %i)\n inx: %i\n", (void*)pCurrent, (void*)pCurrent->prev, (void*)pCurrent->next, 
               pCurrent->v.x, pCurrent->v.y, 
               pCurrent->count);
        for( i=0; i < pCurrent->count; ++i ) {
            printf(" %i,", pCurrent->indices[i]);
        }
        printf(" \n");
        pCurrent = pCurrent->next;
    }
}

void hex_finaliseCellState() {
    MomentumList* pCurrent = listHead.next;
    while ( pCurrent->next != NULL ) {
        pCurrent = pCurrent->next;
        if ( pCurrent->prev ) {
            free( pCurrent->prev );
        }
    }
}

int hex_computeCollision(int inputHex) {
    Vector v = computeMomentum( inputHex );
    MomentumList* pCurrent = &listHead;
    while ( pCurrent->next != NULL && !areSame( &pCurrent->v, &v) ) {
        pCurrent = pCurrent->next;
    }
    if ( !pCurrent ) {
        error(1, 0000, "This should not happen.\n");
    }

    int i = 0;
    double rowSum= drand48()*pCurrent->count;
    for ( i = 0; i < rowSum; ++i ) {};
    return pCurrent->indices[i-1];
}

int hex_computeNewCell(int r, int c, Field* pField) {
    int cell1 = getCell1(r, c, pField);
    int cell2 = getCell2(r, c, pField);
    int cell3 = getCell3(r, c, pField);
    int cell4 = getCell4(r, c, pField);
    int cell5 = getCell5(r, c, pField);
    int cell6 = getCell6(r, c, pField);

    return 
        (  (( cell1 &  8) << 3 )
         | (( cell2 & 16) << 3 )
         | (( cell3 & 32) << 3 )
         | (( cell4 &  1) >> 3 )
         | (( cell5 &  2) >> 3 )
         | (( cell6 &  4) >> 3 )
        );
}


Vector hex_computeMomentum(int r, int c, Field* pField) {
    int value = pField->field[r][c];

    return computeMomentum( value );
}

// ----- PRIVATE METHODS -----

Vector computeMomentum( int value ) {
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
int getCell1(int r, int c, Field* pField) {
    if ( c < 1 ) {
        return 0;
    } else {
        return pField->field[r][c-1];
    }
}
int getCell4(int r, int c, Field* pField) {
    if ( c > pField->nCols-2 ) {
        return 0;
    } else {
        return pField->field[r][c+1];
    }
}
int getCell2(int r, int c, Field* pField) {
    if ( r < 1 ) {
        return 0;
    } else {
        if ( ((r ^ c) & 1) == 0 ) {
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
int getCell6(int r, int c, Field* pField) {
    if ( r > pField->nRows-2 ) {
        return 0;
    } else {
        if ( ((r ^ c) & 1) == 0 ) {
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
int getCell3(int r, int c, Field* pField) {
    if ( r < 1 ) {
        return 0;
    } else {
        if ( ((r ^ c) & 1) == 0 ) {
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
int getCell5(int r, int c, Field* pField) {
    if ( r > pField->nRows-2 ) {
        return 0;
    } else {
        if ( ((r ^ c) & 1) == 0 ) {
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

