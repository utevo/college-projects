#pragma once

#include "../CompositeObject.h"
#include "../primitives/Cube.h"
#include "Wheels.h"
#include "../primitives/Podstawka.h"
#include<iostream>

#define M_PI 3.14159265358979323846

class Cart : public CompositeObject {
public:
Cart(const glm::vec3& possition = { 0.0, 0.0, 0.0 },
		const glm::vec3& rotation = { 0.0, 0.0, 0.0 },
		const glm::vec3& size = { 1.0f, 1.0f, 1.0f }): speed(0) {
		auto cart_tex = std::make_shared<Texture>("textures/cart.png");
		auto wood_tex = std::make_shared<Texture>("textures/lever_tex.png");
		auto blue = std::make_shared<Texture>("textures/podstawka.png");
		auto metal_tex = std::make_shared<Texture>("textures/lever_metal.png");
		auto wh = std::make_unique<Wheels>(possition + glm::vec3(0.0, -0.22, 0.0), rotation, size);
		auto bs = std::make_unique<Cube>(cart_tex, possition + glm::vec3(0.0, -0.22, 0.0), rotation, size * glm::vec3(0.5, 0.02, 1.0));
		auto lc = std::make_unique<Cube>(wood_tex, possition + glm::vec3( 0.0, 0.6, 0.0 ), rotation, size * glm::vec3( 0.02, 0.02, 0.6 ));
		auto l1 = std::make_unique<Cylinder>(metal_tex, possition + glm::vec3(-0.25, 0.6, 0.57), rotation + glm::vec3(0.0, 90.0, 0.0), size * glm::vec3(0.02, 0.02, 0.5));
		auto l2 = std::make_unique<Cylinder>(metal_tex, possition + glm::vec3(-0.25, 0.6, -0.57), rotation + glm::vec3(0.0, 90.0, 0.0), size * glm::vec3(0.02, 0.02, 0.5));
		wheels = wh.get();
		base = bs.get();
		lever_base = lc.get();
		lever_1 = l1.get();
		lever_2 = l2.get();
		addChild(std::move(bs));
		addChild(std::move(wh));
		addChild(std::move(lc));
		addChild(std::move(l1));
		addChild(std::move(l2));
		addChild(std::move(std::make_unique<Podstawka>(blue, possition + glm::vec3(0.0, 0.2, 0.0), rotation, size * glm::vec3(0.1, 0.38, 0.1))));
	}

	void addSpeed(float step)
	{
		speed += step;
		//std::cout << speed << std::endl;
		if (speed >  50) speed =  50;
		if (speed < -50) speed = -50;
	}
	
	bool getSpeed_sign() { return (speed >= 0.0f); }
	
	void moveAuto()
	{
		rotateLever();
		move(speed * glm::vec3(0.0, 0.0, -0.004));
		rotateWheels(speed * glm::vec3(0.0, 0.0, -1.0));
	}

	void stop()
	{
		speed = 0;
	}

	glm::vec3 getPos()
	{
		return base->getPos();
	}

private:
	Wheels* wheels;
	Cube *base, *lever_base;
	Cylinder *lever_1, *lever_2;
	float speed;
	int rotation_dir = 1;
	float cos, sin;

	void rotateLever()
	{
		if (lever_base->getAngle().x >= 45 || lever_base->getAngle().x <= -45) rotation_dir *= -1;
		lever_base->rotate(rotation_dir * speed * glm::vec3(0.2, 0.0, 0.0));
		lever_1->rotate(rotation_dir * speed * glm::vec3(0.2, 0.0, 0.0));
		lever_2->rotate(rotation_dir * speed * glm::vec3(0.2, 0.0, 0.0));
		cos = cosf((-rotation_dir * speed * 0.2) * M_PI / 180);
		sin = sinf((-rotation_dir * speed * 0.2) * M_PI / 180);
		glm::vec3 root = lever_base->getPos();
		lever_1->reposition(glm::vec3(lever_1->getPos().x, (root.y + cos * (lever_1->getPos().y - root.y) + sin * (lever_1->getPos().z - root.z)), \
			(root.z + cos * (lever_1->getPos().z - root.z) - sin * (lever_1->getPos().y - root.y))));
		lever_2->reposition(glm::vec3(lever_2->getPos().x, (root.y + cos * (lever_2->getPos().y - root.y) + sin * (lever_2->getPos().z - root.z)), \
			(root.z + cos * (lever_2->getPos().z - root.z) - sin * (lever_2->getPos().y - root.y))));
	}

	void rotateWheels(const glm::vec3& rotation) {
		wheels->rotate(rotation);
	}
};

