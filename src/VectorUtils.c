#include "../include/VectorUtils.h"
#include <stdio.h>
#include <stdlib.h>

void normalizeVecF(sfVector2f* vec){
	float vx = vec->x;
	float vy = vec->y;
	float magnitude = vx * vx + vy * vy;

	vec->x = vx / magnitude;
	vec->y = vy / magnitude;
}

void normalizeVecU(sfVector2u* vec){
	unsigned vx = vec->x;
	unsigned vy = vec->y;
	unsigned magnitude = vx * vx + vy * vy;

	vec->x = vx / magnitude;
	vec->y = vy / magnitude;
}

void normalizeVecI(sfVector2i* vec){
	int vx = vec->x;
	int vy = vec->y;
	int magnitude = vx * vx + vy * vy;

	vec->x = vx / magnitude;
	vec->y = vy / magnitude;
}

float dotProd(sfVector2f* v1, sfVector2f* v2){
	return v1->x * v2->x + v1->y * v2->y;
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

