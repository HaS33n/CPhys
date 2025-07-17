#ifndef WORLD_H
#define WORLD_H

#define PHYS_EPS 1.f

#include <SFML/Graphics.h>
#include <SFML/System.h>
#include "mtwister.h"

typedef struct {
	float mass; //kg
	sfVector2f velocity; //mps


	sfCircleShape* entity;

}CircPhysicsBody;

typedef struct {
	float mass;
	float radius;
	sfVector2f velocity;
	sfVector2f position;

}bodyDEF;

typedef struct {
	float grav_accel;
	float collision_perfection_coef;
	float pixels_per_meter;
	int num_bodies;

	int num_defined_bodies;
	bodyDEF* definitions;

	sfVector2f mRange;
	sfVector2f vRange;
	sfVector2f rRange;

	sfVector2u phys_area_size;
}Config;

typedef struct {
	Config* config;
	CircPhysicsBody** bodies;

	MTRand rng;
}World;

//---------------------------------------------------------------------------------------------
World* createWorld(Config* config);
void deleteWorld(World* wrld);

void updateWorld(World* wrld, sfTime deltaT);
void drawWorld(sfRenderWindow* target, World* wrld);


//----------------------------------------------------------------------------------------------
CircPhysicsBody* createCPBody();
void deleteCPBody(CircPhysicsBody* bdy);

void drawCPBody(sfRenderWindow* target, CircPhysicsBody* obj);
CircPhysicsBody* createRandomCPBody(sfVector2f mass_range, sfVector2f velocity_range, sfVector2f bounds, sfVector2f radius_range, MTRand* rng);
static CircPhysicsBody* createTestDummy(sfVector2f pos, float r, sfVector2f v, float m, MTRand* rng);

void deleteConfig(Config* cfg);
#endif
