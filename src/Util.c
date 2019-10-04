#include "Util.h"

void printList( MomentumList* pList) {
    int i;
    while ( pList != NULL ) {
        printf(" list: \n c: %p p: %p, n %p\n v: (%i, %i)\n count: %i\n", (void*)pList, (void*)pList->prev, (void*)pList->next, 
               pList->v.x, pList->v.y, 
               pList->count);
        for( i=0; i < pList->count; ++i ) {
            printf(" %i |%i|,", pList->indices[i], countBits(pList->indices[i]));
        }
        printf(" \n");
        printf(" ----- \n");
        pList = pList->next;
    }
}

int countBits(int hex) {
    int j = 0;
    int bitCount = 0;
    for ( j = 0; j <= 8; ++j ) {
        bitCount += (hex>>j)&1; 
    }
    return bitCount;
}
