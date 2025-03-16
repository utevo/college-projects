#pragma once

#include "../CompositeObject.h"
#include"../primitives/Cylinder.h"

class Wheels : public CompositeObject {
public:
	Wheels(const glm::vec3& possition = { 0.0, 0.0, 0.0 },
		const glm::vec3& rotation = { 0.0, 0.0, 0.0 },
		const glm::vec3& size = { 1.0f, 1.0f, 1.0f }) {
		auto wheel_tex = std::make_shared<Texture>("textures/Wheel_texture.png");
		auto axle_tex = std::make_shared<Texture>("textures/axle_texture.png");

		addChild(std::move(std::make_unique<Cylinder>(wheel_tex, glm::vec3(0.5, 0.0, 0.5) + possition, glm::vec3(0.0, 90.0, 0.0) + rotation, size * glm::vec3(0.2, 0.2, 0.05))));
		addChild(std::move(std::make_unique<Cylinder>(axle_tex, glm::vec3(0.52, 0.0, 0.5) + possition, glm::vec3(0.0, 90.0, 0.0) + rotation, size * glm::vec3(0.05, 0.05, 0.05))));

		addChild(std::move(std::make_unique<Cylinder>(wheel_tex, glm::vec3(0.5, 0.0, -0.5) + possition, glm::vec3(0.0, 90.0, 0.0) + rotation, size * glm::vec3(0.2, 0.2, 0.05))));
		addChild(std::move(std::make_unique<Cylinder>(axle_tex, glm::vec3(0.52, 0.0, -0.5) + possition, glm::vec3(0.0, 90.0, 0.0) + rotation, size * glm::vec3(0.05, 0.05, 0.05))));
		
		addChild(std::move(std::make_unique<Cylinder>(wheel_tex, glm::vec3(-0.55, 0.0, -0.5) + possition, glm::vec3(0.0, 90.0, 0.0) + rotation, size * glm::vec3(0.2, 0.2, 0.05))));
		addChild(std::move(std::make_unique<Cylinder>(axle_tex, glm::vec3(-0.57, 0.0, -0.5) + possition, glm::vec3(0.0, 90.0, 0.0) + rotation, size * glm::vec3(0.05, 0.05, 0.05))));
		
		addChild(std::move(std::make_unique<Cylinder>(wheel_tex, glm::vec3(-0.55, 0.0, 0.5) + possition, glm::vec3(0.0, 90.0, 0.0) + rotation, size * glm::vec3(0.2, 0.2, 0.05))));
		addChild(std::move(std::make_unique<Cylinder>(axle_tex, glm::vec3(-0.57, 0.0, 0.5) + possition, glm::vec3(0.0, 90.0, 0.0) + rotation, size * glm::vec3(0.05, 0.05, 0.05))));
	}


};
