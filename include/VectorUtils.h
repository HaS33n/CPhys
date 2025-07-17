#ifndef VECTORUTILS_H
#define VECTORUTILS_H

#include <SFML/System/Vector2.h>

void normalizeVecF(sfVector2f* vec);
void normalizeVecU(sfVector2u* vec);
void normalizeVecI(sfVector2i* vec);

#define normalizeVec(vec) _Generic(vec, sfVector2i*: normalizeVecI, sfVector2u*: normalizeVecU, sfVector2f*: normalizeVecF)(vec)


#endif