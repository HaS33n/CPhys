#ifndef WORLD_H
#define WORLD_H

#include <SFML/Graphics.h>
#include <SFML/System.h>
#include "mtwister.h"

typedef struct {
	float mass; //kg
	sfVector2f velocity; //mps


	sfCircleShape* entity;

}CircPhysicsBody;

typedef struct {
	float grav_accel;
	float collision_perfection_coef; //TODO: make use out of it
	float pixels_per_meter;

	sfVector2u phys_area_size;

	CircPhysicsBody** bodies;
	int num_bodies;

	MTRand rng;
}World;

//---------------------------------------------------------------------------------------------
World* createWorld(int n_entities, float grav, float ppm, float col_coeff, sfVector2u bounds);
void deleteWorld(World* wrld);

void updateWorld(World* wrld, sfTime deltaT);
void drawWorld(sfRenderWindow* target, World* wrld);


//----------------------------------------------------------------------------------------------
CircPhysicsBody* createCPBody();
void deleteCPBody(CircPhysicsBody* bdy);

void drawCPBody(sfRenderWindow* target, CircPhysicsBody* obj);
CircPhysicsBody* createRandomCPBody(sfVector2f mass_range, sfVector2f velocity_range, sfVector2f bounds, sfVector2f radius_range, MTRand* rng);

#endif
