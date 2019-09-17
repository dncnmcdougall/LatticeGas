#pragma once

#include "Field.h"
#include "Vector.h"


//        2
//  1 -.<
//        4
//
//  Bits by value
//  0: .
//  1: -.
//  2:  ./
//  3: -./
//  4:  .\
//  5: -.\
//  6:  .<
//  7: -.<

void tri_initialiseCellState();
void tri_finaliseCellState();

int tri_computeCollision(int inputHex);
int tri_computeNewCell(int r, int c, Field* pField);
Vector tri_computeMomentum(int r, int c, Field* pField);

