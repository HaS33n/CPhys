#include "../include/World.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

World* createWorld(int n_entities, float grav, float ppm, float col_coeff, sfVector2u bounds){
	World* wrld = malloc(sizeof(World));
	if (wrld == NULL) {
		printf("Memory Fail\n");
		exit(EXIT_FAILURE);
	}

	wrld->grav_accel = grav;
	wrld->num_bodies = n_entities;
	wrld->pixels_per_meter = ppm;
	wrld->phys_area_size = bounds;
	wrld->collision_perfection_coef = col_coeff;
	wrld->bodies = malloc(n_entities * sizeof(CircPhysicsBody*));
	if (wrld->bodies == NULL) {
		printf("Memory Fail\n");
		exit(EXIT_FAILURE);
	}
	srand(time(NULL));
	wrld->rng = seedRand(rand());
	const sfVector2f mass_range = { 0.5f, 10.f };
	const sfVector2f velocity_range = { 0.5f, 100.f };
	const sfVector2f radius_range = { 20.f, 70.f };
	const sfVector2f f_bounds = { bounds.x, bounds.y };

	for (int i = 0; i < n_entities; i++) {
		CircPhysicsBody* obj = createRandomCPBody(mass_range, velocity_range, f_bounds, radius_range,&wrld->rng);
		wrld->bodies[i] = obj;
	}

	return wrld;
}

void deleteWorld(World* wrld){
	for (int i = 0; i < wrld->num_bodies; i++)
		deleteCPBody(wrld->bodies[i]);

	free(wrld->bodies);
	free(wrld);
}

void updateWorld(World* wrld, sfTime deltaT){
	//TODO: physics
}

void drawWorld(sfRenderWindow* target, World* wrld){
	for (int i = 0; i < wrld->num_bodies; i++)
		drawCPBody(target, wrld->bodies[i]);
}

//this could use default args
CircPhysicsBody* createCPBody(){
	CircPhysicsBody* body = malloc(sizeof(CircPhysicsBody));
	if (body == NULL) {
		printf("Memory Fail\n");
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

//TODO: ensure no body will be generated on top of another and add some padding to rng output to prevent multiplying by 0
CircPhysicsBody* createRandomCPBody(sfVector2f mass_range, sfVector2f velocity_range, sfVector2f bounds, sfVector2f radius_range, MTRand* rng){
	CircPhysicsBody* bdy = createCPBody();

	//rand() from libc sucks, thus im using superior way to generate random numbers c:
	//note, this mt19937 implementation is NOT made by me!
	bdy->mass = ((float)(mass_range.y - mass_range.x)) * genRand(rng);

	const float vx = (((float)(velocity_range.y - velocity_range.x)) * genRand(rng)) * ((genRandLong(rng) % 2) * -1);
	const float vy = (((float)(velocity_range.y - velocity_range.x)) * genRand(rng)) * ((genRandLong(rng) % 2) * -1);
	const sfVector2f v = { vx, vy };
	bdy->velocity = v;
	
	sfCircleShape* shape = sfCircleShape_create();
	sfCircleShape_setRadius(shape, ((float)(radius_range.y - radius_range.x)) * genRand(rng));

	const float x = ((float)(bounds.x - sfCircleShape_getRadius(shape) - 10)) * genRand(rng); //10 serves as a buffor of some sort
	const float y = ((float)(bounds.y - sfCircleShape_getRadius(shape) - 10)) * genRand(rng); // - || -
	const sfVector2f pos = { x,y };
	sfCircleShape_setPosition(shape, pos);

	sfCircleShape_setFillColor(shape, sfColor_fromInteger(genRandLong(rng) | 255u)); //ensure alpha always stays 255
	bdy->entity = shape;

	return bdy;
}
