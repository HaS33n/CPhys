#ifndef WORLD_H
#define WORLD_H

#include <SFML/Graphics.h>
#include <SFML/System.h>
#include "mtwister.h"
#include <stdbool.h>
#include "PhysicsEngine.h"


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
	bool stopped;
}World;

//---------------------------------------------------------------------------------------------
World* createWorld(Config* config);
void deleteWorld(World* wrld);

void updateWorld(World* wrld, sfTime deltaT); //TODO fix physics
void drawWorld(sfRenderWindow* target, World* wrld);


//----------------------------------------------------------------------------------------------
CircPhysicsBody* createCPBody();
void deleteCPBody(CircPhysicsBody* bdy);

void drawCPBody(sfRenderWindow* target, CircPhysicsBody* obj);

//TODO: ensure no body will be generated on top of another and fix ranges when spread between them is too small
CircPhysicsBody* createRandomCPBody(sfVector2f mass_range, sfVector2f velocity_range, sfVector2f bounds, sfVector2f radius_range, MTRand* rng);
static CircPhysicsBody* createTestDummy(sfVector2f pos, float r, sfVector2f v, float m, MTRand* rng); //TODO change name

void deleteConfig(Config* cfg);
Config* createCopyOfConfig(const Config* cfg);
#endif
