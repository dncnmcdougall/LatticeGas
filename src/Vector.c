#include "Vector.h"

int areSame(const Vector* pV1, const Vector* pV2) {
    if ( ( pV1->x == pV2->x ) && ( pV1->y == pV2->y) ) {
        return 1;
    } else {
        return 0;
    }
}
void addVector(Vector* pV1, const Vector* pV2) {
    pV1->x += pV2->x;
    pV1->y += pV2->y;
}
