#ifndef PHYSENGINE_H
#define PHYSENGINE_H

#include <SFML/System.h>
#include <SFML/Graphics.h>
#include <math.h>
#include <stdbool.h>

#define EPS 1

typedef struct {
	float mass; //kg
	sfVector2f velocity; //mps


	sfCircleShape* entity;

}CircPhysicsBody;

bool doWallCollison(CircPhysicsBody* body, sfVector2u bounds, float coeff); //TODO optimize this mess
void doCollisionBetweenBodies(CircPhysicsBody* body, CircPhysicsBody* body2, float coeff); //TODO implement
void updateMotion(CircPhysicsBody* body, float accel, float ppm, sfTime dt, bool stickedToFloor);


#endif // !PHYSENGINE_H
