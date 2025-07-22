#include "../include/World.h"
#include "../include//VectorUtils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

World* createWorld(Config* config){
	World* wrld = malloc(sizeof(World));
	if (wrld == NULL) {
		perror("Memory Fail\n");
		exit(EXIT_FAILURE);
	}

	wrld->config = config;
	wrld->bodies = malloc(config->num_bodies * sizeof(CircPhysicsBody*));
	if (wrld->bodies == NULL) {
		perror("Memory Fail\n");
		exit(EXIT_FAILURE);
	}
	srand(time(NULL));
	wrld->rng = seedRand(rand());
	wrld->stopped = false;

	for (int i = 0; i < config->num_defined_bodies; i++) {
		bodyDEF* def = config->definitions + i;

		CircPhysicsBody* obj = createTestDummy(def->position, def->radius, def->velocity, def->mass, &wrld->rng);
		wrld->bodies[i] = obj;
	}
	for (int i = 0; i < (config->num_bodies - config->num_defined_bodies); i++) {
		const sfVector2u u = config->phys_area_size;
		const sfVector2f f_bounds = { u.x, u.y };
		CircPhysicsBody* obj = createRandomCPBody(config->mRange, config->vRange, f_bounds, config->rRange, &wrld->rng);
		wrld->bodies[i] = obj;
	}

	return wrld;
}

void deleteWorld(World* wrld){
	for (int i = 0; i < wrld->config->num_bodies; i++)
		deleteCPBody(wrld->bodies[i]);

	free(wrld->bodies);
	deleteConfig(wrld->config);
	free(wrld);
}

void updateWorld(World* wrld, sfTime deltaT){
	if (wrld->stopped)
		return;

	for (int i = 0; i < wrld->config->num_bodies; i++) {
		CircPhysicsBody* body = wrld->bodies[i];

		bool b = doWallCollison(body, wrld->config->phys_area_size, wrld->config->collision_perfection_coef);
		for (int j = i + 1; j < wrld->config->num_bodies; j++)
			doCollisionBetweenBodies(body, wrld->bodies[j], wrld->config->collision_perfection_coef);

		updateMotion(body, wrld->config->grav_accel, wrld->config->pixels_per_meter, deltaT, b);
	}

}

void drawWorld(sfRenderWindow* target, World* wrld){
	for (int i = 0; i < wrld->config->num_bodies; i++)
		drawCPBody(target, wrld->bodies[i]);
}

//this could use default args
CircPhysicsBody* createCPBody(){
	CircPhysicsBody* body = malloc(sizeof(CircPhysicsBody));
	if (body == NULL) {
		perror("Memory Fail\n");
		exit(EXIT_FAILURE);
	}
	body->entity = NULL;
	body->mass = 0;

	const sfVector2f v = { 0.f, 0.f };
	body->velocity = v;


	return body;
}

void deleteCPBody(CircPhysicsBody* bdy){
	sfCircleShape_destroy(bdy->entity);
	free(bdy);
}

void drawCPBody(sfRenderWindow* target, CircPhysicsBody* obj){
	sfRenderWindow_drawCircleShape(target, obj->entity, NULL);
}

static float RNGFloatInRange(sfVector2f range, MTRand* rng){
	
	if (range.x < range.y){
		float buff = range.x;
		range.x = range.y;
		range.y = buff;
	}

	return range.x + (range.y - range.x) * genRand(rng);
}

static sfColor generateRandomVisibleClr(MTRand* rng) {
	uint32_t clr = genRandLong(rng);

	uint8_t R = clr >> 24;
	uint8_t G = (clr >> 16) & 255u;
	uint8_t B = (clr >> 8) & 255u;

	//ensure color is visible
	float Y = 0.2126 * R + 0.7152 * G + 0.0722 * B;
	if (Y < 0.3f)
		G = (uint8_t)((0.3 - 0.2126 * R - 0.0722 * B) / 0.7152); //if not visible enough, just boost green

	return sfColor_fromRGBA(R,G,B,255u);
}

CircPhysicsBody* createRandomCPBody(sfVector2f mass_range, sfVector2f velocity_range, sfVector2f bounds, sfVector2f radius_range, MTRand* rng){
	CircPhysicsBody* bdy = createCPBody();

	//rand() from libc sucks, thus im using superior way to generate random numbers c:
	//note, this mt19937 implementation is NOT made by me!
	const float m = RNGFloatInRange(mass_range, rng);
	bdy->mass = m;

	const float vx = RNGFloatInRange(velocity_range, rng);
	const float vy = RNGFloatInRange(velocity_range, rng);
	bdy->velocity = (sfVector2f){ vx,vy };

	
	sfCircleShape* shape = sfCircleShape_create();
	float rad = RNGFloatInRange(radius_range, rng);
	sfCircleShape_setRadius(shape, rad);

	const float r = rad + 5.f; //5 serves as a buffor of some sort
	bounds.x -= 2*r;
	bounds.y -= 2*r;
	
	float x = bounds.x * genRand(rng);
	//x += r * (x < 0);

	float y = bounds.y * genRand(rng);
	//y += r * (y < 0);

	const sfVector2f pos = { x,y };
	sfCircleShape_setPosition(shape, pos);

	sfCircleShape_setFillColor(shape, generateRandomVisibleClr(rng));
	bdy->entity = shape;

	return bdy;
}

CircPhysicsBody* createTestDummy(sfVector2f pos, float r, sfVector2f v, float m, MTRand* rng){
	CircPhysicsBody* bdy = createCPBody();
	bdy->mass = m;
	bdy->velocity = v;

	sfCircleShape* shp = sfCircleShape_create();
	sfCircleShape_setRadius(shp, r);
	sfCircleShape_setPosition(shp, pos);
	sfCircleShape_setFillColor(shp, sfColor_fromInteger(genRandLong(rng) | 255u));

	bdy->entity = shp;
	return bdy;
}

void deleteConfig(Config* cfg){
	free(cfg->definitions);
	free(cfg);
}

Config* createCopyOfConfig(const Config* cfg){
	Config* newcfg = malloc(sizeof(Config));

	*newcfg = *cfg;
	newcfg->definitions = malloc(sizeof(bodyDEF) * newcfg->num_defined_bodies);
	memcpy(newcfg->definitions, cfg->definitions, sizeof(bodyDEF) * cfg->num_defined_bodies);

	return newcfg;
}
