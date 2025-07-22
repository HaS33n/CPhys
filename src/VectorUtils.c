#include "../include/VectorUtils.h"
#include <stdio.h>
#include <stdlib.h>

sfVector2f addVectorsF(const sfVector2f* v1, const sfVector2f* v2){
	sfVector2f res;
	res.x = v1->x + v2->x;
	res.y = v1->y + v2->y;
	return res;
}

sfVector2f subVectorsF(const sfVector2f* v1, const sfVector2f* v2){
	sfVector2f v;
	v.x = -(v2->x);
	v.y = -(v2->y);
	return addVectorsF(v1, &v);
}

sfVector2f multScalVecF(const sfVector2f* v1, const float s){
	sfVector2f res;

	res.x = v1->x * s;
	res.y = v1->y * s;
	return res;
}

float dotProd(const sfVector2f* v1, const sfVector2f* v2){
	return v1->x * v2->x + v1->y * v2->y;
}

float normSqrd(const sfVector2f* v){
	return dotProd(v, v);
}

sfVector2f strtov2f(const char* str){
	sfVector2f vec;

	if (sscanf(str, "%f - %f", &vec.x, &vec.y) != 2) {
		perror("badly defined vector");
		//printf(str);
		exit(EXIT_FAILURE);
	}

	return vec;
}

