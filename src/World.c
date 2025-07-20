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

	// /*
	const float dt = sfTime_asSeconds(deltaT);

	for (int i = 0; i < wrld->config->num_bodies; i++) {
		int db = 0;

		CircPhysicsBody* body = wrld->bodies[i];

		sfVector2f position = sfCircleShape_getPosition(body->entity);
		const float r = sfCircleShape_getRadius(body->entity);
		const float mass = body->mass;
		sfVector2f velocity = body->velocity;

		//gravity
		velocity.y += wrld->config->grav_accel * dt;

		//collision with walls TODO: Branchless
		if (position.x + 2 * r >= wrld->config->phys_area_size.x || position.x <= 0) {

			velocity.x *= -1 * wrld->config->collision_perfection_coef;
			velocity.y *= wrld->config->collision_perfection_coef;

			if (fabs(velocity.x) < PHYS_EPS)
				velocity.x = 0;

			if (fabs(velocity.y) < PHYS_EPS)
				velocity.y = 0;
		}
		if (position.y + 2 * r >= wrld->config->phys_area_size.y || position.y <= 0) {

			velocity.y *= -1 * wrld->config->collision_perfection_coef;
			velocity.x *= wrld->config->collision_perfection_coef;

			if (fabs(velocity.x) < PHYS_EPS)
				velocity.x = 0;

			if (fabs(velocity.y) < PHYS_EPS)
				velocity.y = 0;

			db = 1;
		}

		//collision with other bodies
		//TODO


		position.x += dt * velocity.x * wrld->config->pixels_per_meter;
		position.y += dt * velocity.y * wrld->config->pixels_per_meter;



		if (db)
			printf("%f , %f\n", velocity.x, velocity.y);

		body->velocity = velocity;
		sfCircleShape_setPosition(body->entity, position);
	}
	// */
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

CircPhysicsBody* createRandomCPBody(sfVector2f mass_range, sfVector2f velocity_range, sfVector2f bounds, sfVector2f radius_range, MTRand* rng){
	CircPhysicsBody* bdy = createCPBody();

	//rand() from libc sucks, thus im using superior way to generate random numbers c:
	//note, this mt19937 implementation is NOT made by me!
	float m = mass_range.y * genRand(rng);
	m += mass_range.x * (m < mass_range.x); //branchless ensuring that result is greater or equal min_range
	bdy->mass = m;

	float vx = velocity_range.y * genRand(rng);
	vx += velocity_range.x * (vx < velocity_range.x);
	//vx *= 1 - ((genRandLong(rng) % 2) * 2);

	float vy = velocity_range.y * genRand(rng);
	vy += velocity_range.x * (vy < mass_range.x);
	//vy *= 1 - ((genRandLong(rng) % 2) * 2);

	const sfVector2f v = { vx, vy };
	bdy->velocity = v;

	
	sfCircleShape* shape = sfCircleShape_create();
	float rad = radius_range.y * genRand(rng);
	rad += radius_range.x * (rad < radius_range.x); //branchless ensuring that result is greater or equal min_range
	sfCircleShape_setRadius(shape, rad);

	const float r = sfCircleShape_getRadius(shape) + 10.f; //10 serves as a buffor of some sort
	bounds.x -= r;
	bounds.y -= r;
	
	float x = bounds.x * genRand(rng);
	x += r * (x < 0);

	float y = bounds.y * genRand(rng);
	y += r * (y < 0);

	const sfVector2f pos = { x,y };
	sfCircleShape_setPosition(shape, pos);


	sfCircleShape_setFillColor(shape, sfColor_fromInteger(genRandLong(rng) | 255u)); //ensure alpha always stays 255
	bdy->entity = shape;

	printf("%f , %f\n", vx, vy);
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
