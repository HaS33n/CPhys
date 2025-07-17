#include "../include/VectorUtils.h"

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
