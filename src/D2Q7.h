#pragma once

#include "Field.h"
#include "Vector.h"

//   2:4     3:8
//      \   / 
//1:2 -- 0:1 -- 4:16
//      /   \
//   6:62    5:32

//   0: 000000000
//   1: 000000001
//   2: 000000010
//   4: 000000100
//   8: 000001000
//  16: 000010000
//  32: 000100000
//  64: 001000000
// 128: 010000000
// 256: 100000000

void d2q7_initialiseCellState();
void d2q7_finaliseCellState();

int d2q7_computeCollision(int inputHex, double random);
int d2q7_computeNewCell(int r, int c, Field* pField);
Vector d2q7_computeMomentum(int r, int c, Field* pField);

Vector d2q7_computeMomentumFromHex( int value );

