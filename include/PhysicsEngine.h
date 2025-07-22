#ifndef PHYSENGINE_H
#define PHYSENGINE_H

#include <SFML/System.h>
#include <SFML/Graphics.h>
#include <math.h>
#include <stdbool.h>
#include "VectorUtils.h"

#define EPS 0.5f //1

typedef struct {
	float mass; //kg
	sfVector2f velocity; //mps


	sfCircleShape* entity;

}CircPhysicsBody;

bool doWallCollison(CircPhysicsBody* body, sfVector2u bounds, float coeff); //TODO optimize this mess
void doCollisionBetweenBodies(CircPhysicsBody* body, CircPhysicsBody* body2, float coeff); //TODO clean up this mess
void updateMotion(CircPhysicsBody* body, float accel, float ppm, sfTime dt, bool stickedToFloor); //use vector operations


#endif // !PHYSENGINE_H
