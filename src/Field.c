#include <stdlib.h>
#include <stdio.h>
#include "Field.h"

Field* createField(int nRows, int nCols){
    Field* pField = malloc(sizeof(Field));
    pField->nRows = nRows;
    pField->nCols = nCols;
    pField->field = calloc(nRows, sizeof(int*));
    int r = 0;
    for ( r = 0; r < nRows; ++r) {
        pField->field[r] = calloc(nCols, sizeof(int));
    }
    return pField;
}

void freeField(Field* pField) {
    int r = 0;
    for ( r = 0; r < pField->nRows; ++r) {
        free(pField->field[r]);
    }
    free(pField->field);
    free(pField);
    pField = NULL;
}

void printFieldInHex(const Field* pField) {
    int r = 0, c = 0;
    for ( r = 0; r < pField->nRows; ++r) {
        if ( (r & 1) == 0 ) {
            printf(" ");
        }
        for ( c = 0; c < pField->nCols; ++c) {
            printf("%i ",pField->field[r][c]);
        }
        printf("\n");
    }
}
