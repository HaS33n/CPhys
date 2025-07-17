#ifndef VECTORUTILS_H
#define VECTORUTILS_H

#include <SFML/System/Vector2.h>

void normalizeVecF(sfVector2f* vec);
void normalizeVecU(sfVector2u* vec);
void normalizeVecI(sfVector2i* vec);

#define normalizeVec(vec) _Generic(vec, sfVector2i*: normalizeVecI, sfVector2u*: normalizeVecU, sfVector2f*: normalizeVecF)(vec)

float dotProd(sfVector2f* v1, sfVector2f* v2);

// required form: x - y , where x and y are floats, whitespaces are ignored
sfVector2f strtov2f(const char* str); 

#endif