#ifndef VECTORUTILS_H
#define VECTORUTILS_H

#include <SFML/System/Vector2.h>

sfVector2f addVectorsF(const sfVector2f* v1, const sfVector2f* v2);
sfVector2f subVectorsF(const sfVector2f* v1, const sfVector2f* v2);
sfVector2f multScalVecF(const sfVector2f* v1, const float s);

float dotProd(const sfVector2f* v1, const sfVector2f* v2);
float normSqrd(const sfVector2f* v);

// required form: x - y , where x and y are floats, whitespaces are ignored
sfVector2f strtov2f(const char* str);

#endif