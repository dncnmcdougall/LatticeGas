#pragma once
#include "Vector.h"
#include <stdio.h>

typedef struct momList MomentumList;

struct momList {
    Vector v;
    MomentumList* next;
    MomentumList* prev;
    int count;
    int* indices;
};

void printList( MomentumList* pList);

int countBits(int hex);
