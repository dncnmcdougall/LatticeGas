#pragma once

typedef struct {
    int nRows;
    int nCols;
    int** field;
} Field;

Field* createField(int nRows, int nCols);
void freeField(Field* pField);

void printFieldInHex(const Field* pField);
