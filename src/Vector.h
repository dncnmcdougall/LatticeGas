#pragma once

typedef struct {
    int x;
    int y;
} Vector;

int areSame(const Vector* pV1, const Vector* pV2);

void addVector(Vector* pV1, const Vector* pV2);
