#include "../include/PhysicsEngine.h"

bool doWallCollison(CircPhysicsBody* body, sfVector2u bounds, float coeff){
	bool is_sticked_to_floor = false;

	sfVector2f position = sfCircleShape_getPosition(body->entity);
	const float r = sfCircleShape_getRadius(body->entity);
	sfVector2f velocity = body->velocity;

	
	/* Im keeping this for reference, since its easier to read that way
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
	*/

	//Branchless ^^
	//Walls
	bool bx1 = position.x + 2 * r >= bounds.x;
	bool bx2 = position.x <= 0;
	bool bx = bx1 || bx2;

	velocity.x *= (1 * !bx) + (-1 * coeff) * bx;
	velocity.y *= (1 * !bx) + (coeff) * bx;

	position.x = 0 + (bounds.x - (2 * r)) * bx1 + position.x * !bx;

	velocity.x *= !((fabs(velocity.x) < EPS) && bx);
	velocity.y *= !((fabs(velocity.y) < EPS) && bx);

	//Floor and Roof
	bool by1 = position.y + 2 * r >= bounds.y;
	bool by2 = position.y <= 0;
	bool by = by1 || by2;
	
	velocity.y *= (1 * !by) + (-1 * coeff) * by;
	velocity.x *= (1 * !by) + (coeff) * by;

	position.y = 0 + (bounds.y - (2 * r)) * by1 + position.y * !by;

	velocity.x *= !((fabs(velocity.x) < EPS) && by);
	velocity.y *= !((fabs(velocity.y) < EPS) && by);

	is_sticked_to_floor = (fabs(velocity.y) < EPS) && by;


	//set
	sfCircleShape_setPosition(body->entity, position);
	body->velocity = velocity;

	return is_sticked_to_floor;
}

void doCollisionBetweenBodies(CircPhysicsBody* body, CircPhysicsBody* body2, float coeff){
	sfVector2f p1 = sfCircleShape_getPosition(body->entity);
	const float r1 = sfCircleShape_getRadius(body->entity);
	sfVector2f v1 = body->velocity;
	const float m1 = body->mass;

	sfVector2f p2 = sfCircleShape_getPosition(body2->entity);
	const float r2 = sfCircleShape_getRadius(body2->entity);
	sfVector2f v2 = body2->velocity;
	const float m2 = body2->mass;

	/*
	* TODO: clean up this mess
	*/

	//calculate centers
	p1.x += r1;
	p1.y += r1;
	p2.x += r2;
	p2.y += r2;

	float dx = p2.x - p1.x;
	float dy = p2.y - p1.y;

	float d = sqrt(dx * dx + dy * dy);
	if (d <= r1 + r2) {
		

		sfVector2f x_diff = subVectorsF(&p1, &p2);
		sfVector2f v_diff1 = subVectorsF(&v1, &v2);
		float factor1 = (2 * m2 / (m1 + m2)) * (dotProd(&v_diff1, &x_diff) / normSqrd(&x_diff));

		sfVector2f sv = multScalVecF(&x_diff, factor1);

		sfVector2f x_diff2 = subVectorsF(&p2, &p1);
		sfVector2f v_diff2 = subVectorsF(&v2, &v1);
		float factor2 = (2 * m1 / (m1 + m2)) * (dotProd(&v_diff2, &x_diff2) / normSqrd(&x_diff2));

		sfVector2f sv2 = multScalVecF(&x_diff2, factor2);
		v2 = subVectorsF(&v2, &sv2);
		v1 = subVectorsF(&v1, &sv);

		body->velocity = multScalVecF(&v1,coeff);
		body2->velocity = multScalVecF(&v2, coeff);

		//set positions for pixel perfect collisions
		sfVector2f sub = subVectorsF(&p1, &p2);
		sfVector2f u = multScalVecF(&sub, 1.0 / d);

		sfVector2f mul = multScalVecF(&u, r1 + r2);
		sfVector2f p1p = addVectorsF(&p2, &mul);

		sfVector2f shift = subVectorsF(&p1p, &p1);

		p1 = addVectorsF(&p1, &shift);
		p1.x -= r1;
		p1.y -= r1;
		p2.x -= r2;
		p2.y -= r2;

		sfCircleShape_setPosition(body->entity, p1);
		sfCircleShape_setPosition(body2->entity, p2);
	}
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
