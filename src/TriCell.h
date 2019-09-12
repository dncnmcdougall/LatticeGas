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
double transformations[8][8];

// double transformations[][8] = {
// /*        0  ,  1   ,  2   ,  3  ,  4   ,  5  ,  6  ,  7 */
// /*0*/ { 1.0  ,0.0   ,0.0   ,0.0  ,0.0   ,0.0  ,0.0  ,0.0},
// /*1*/ { 0.0  ,0.1   ,0.45  ,0.0  ,0.45  ,0.0  ,0.0  ,0.0},
// /*2*/ { 0.0  ,0.45  ,0.1   ,0.0  ,0.45  ,0.0  ,0.0  ,0.0},
// /*3*/ { 0.0  ,0.0   ,0.0   ,0.2  ,0.0   ,0.4  ,0.4  ,0.0},
// /*4*/ { 0.0  ,0.45  ,0.45  ,0.0  ,0.1   ,0.0  ,0.0  ,0.0},
// /*5*/ { 0.0  ,0.0   ,0.0   ,0.4  ,0.0   ,0.2  ,0.4  ,0.0},
// /*6*/ { 0.0  ,0.0   ,0.0   ,0.4  ,0.0   ,0.4  ,0.2  ,0.0},
// /*7*/ { 0.0  ,0.0   ,0.0   ,0.0  ,0.0   ,0.0  ,0.0  ,1.0}
// };

void validateTriCellState();

int computeCollision(int inputHex);
int computeNewCell(int r, int c, Field* pField);
Vector computeMomentum(int r, int c, Field* pField);

int getCellAbove(int r, int c, Field* pField);
int getCellBelow(int r, int c, Field* pField);
int getCellBehind(int r, int c, Field* pField);
