#pragma once

#include "Field.h"
#include "Vector.h"

// 2:2    3:4   4:8
//      \  |  /
// 1:1  -  .  - 5:16
//      /  |  \
// 8:128  7:64  6:32

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

void d2q8_initialiseCellState();
void d2q8_finaliseCellState();

int d2q8_computeCollision(int inputHex, double random);
int d2q8_computeNewCell(int r, int c, Field* pField);
Vector d2q8_computeMomentum(int r, int c, Field* pField);

Vector d2q8_computeMomentumFromHex( int value );

