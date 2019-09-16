#pragma once

#include "Vector.h"
#include "Boundary.h"

typedef struct {
    int nRows;
    int nCols;
    int** field;
} Field;

Field* createField(int nRows, int nCols);
void freeField(Field* pField);

void printFieldInHex(const Field* pField);

void drawField(Field* pField, const char* filename,
               Vector (*computeMomentum)(int,int,Field*),
               CellType (*getCellType)(int,int)
              );
