#pragma once

#include "../CompositeObject.h"
#include"../primitives/Cube.h"

class Railway : public CompositeObject {
public:
	Railway(int number = 1,
		const glm::vec3& possition = { 0.0, 0.0, 0.0 },
		const glm::vec3& rotation = { 0.0, 0.0, 0.0 },
		const glm::vec3& size = { 1.0f, 1.0f, 1.0f }) {
		auto rail_tex = std::make_shared<Texture>("textures/rail.png");
		auto cart_tex = std::make_shared<Texture>("textures/rail_plank.png");
		glm::vec3 offset = glm::vec3(0.0, 0.0, 1.0);
		for (int i = 0; i < number; ++i)
		{
			addChild(std::move(std::make_unique<Cube>(cart_tex, glm::vec3(0.0, -0.48, 0.0) + i * offset + possition, rotation, size * glm::vec3(0.6, 0.02, 0.2))));
			addChild(std::move(std::make_unique<Cube>(rail_tex, glm::vec3(-0.525, -0.44, 0.0) + i * offset + possition, rotation, size * glm::vec3(0.03, 0.02, 0.5))));
			addChild(std::move(std::make_unique<Cube>(rail_tex, glm::vec3(0.525, -0.44, 0.0) + i * offset + possition, rotation, size * glm::vec3(0.03, 0.02, 0.5))));
			addChild(std::move(std::make_unique<Cube>(cart_tex, glm::vec3(0.0, -0.48, 0.0) - i * offset + possition, rotation, size * glm::vec3(0.6, 0.02, 0.2))));
			addChild(std::move(std::make_unique<Cube>(rail_tex, glm::vec3(-0.525, -0.44, 0.0) - i * offset + possition, rotation, size * glm::vec3(0.03, 0.02, 0.5))));
			addChild(std::move(std::make_unique<Cube>(rail_tex, glm::vec3(0.525, -0.44, 0.0) - i * offset + possition, rotation, size * glm::vec3(0.03, 0.02, 0.5))));
		}
	}

	
};