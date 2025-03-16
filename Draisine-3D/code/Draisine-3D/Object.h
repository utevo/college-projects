#pragma once

#include <glm/vec3.hpp>

#include "Shader.h"

class Object {
public:
	Object() {};
	// virtual ~Object() = 0;
	virtual void render(std::shared_ptr<Shader> shader, bool tex=true) = 0;

	virtual void move(const glm::vec3& vec) = 0;
	virtual void rotate(const glm::vec3& vec) = 0;
	virtual void scale(const glm::vec3& vec) = 0;
};

