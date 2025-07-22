#include "../include/PhysicsEngine.h"

bool doWallCollison(CircPhysicsBody* body, sfVector2u bounds, float coeff){
	bool is_sticked_to_floor = false;

	sfVector2f position = sfCircleShape_getPosition(body->entity);
	const float r = sfCircleShape_getRadius(body->entity);
	const float mass = body->mass;
	sfVector2f velocity = body->velocity;

	if (position.x + 2 * r >= bounds.x || position.x <= 0) {

		velocity.x *= -1 * coeff;
		velocity.y *= coeff;

		if (position.x <= 0) {
			position.x = 0;
		}
		else {
			position.x = bounds.x - (2 * r);
		}

		if (fabs(velocity.x) < EPS) {
			velocity.x = 0;
		}


		if (fabs(velocity.y) < EPS) {
			velocity.y = 0;
		}

	}
	if (position.y + 2 * r >= bounds.y || position.y <= 0) {

		velocity.y *= -1 * coeff;
		velocity.x *= coeff;

		if (position.y <= 0) {
			position.y = 0;
		}
		else {
			position.y = bounds.y - (2 * r);
		}

		if (fabs(velocity.x) < EPS) {
			velocity.x = 0;
		}


		if (fabs(velocity.y) < EPS) {
			velocity.y = 0;
			is_sticked_to_floor = true;
		}

	}
		

	sfCircleShape_setPosition(body->entity, position);
	body->velocity = velocity;

	return is_sticked_to_floor;
}

void doCollisionBetweenBodies(CircPhysicsBody* body, CircPhysicsBody* body2, float coeff){
	//TODO
}

void updateMotion(CircPhysicsBody* body, float accel, float ppm, sfTime dt, bool stickedToFloor){
	sfVector2f position = sfCircleShape_getPosition(body->entity);
	sfVector2f velocity = body->velocity;

	float time = sfTime_asSeconds(dt);
	position.x += velocity.x * time * ppm;
	position.y += (velocity.y * time + ((accel * time * time) / 2)) * ppm * !stickedToFloor;

	velocity.y += time * accel * !stickedToFloor;

	sfCircleShape_setPosition(body->entity, position);
	body->velocity = velocity;
}
