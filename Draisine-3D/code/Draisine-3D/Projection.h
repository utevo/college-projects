#pragma once

#include <glm/glm.hpp>

class Projection {
public:
	virtual glm::mat4& getMatrix() = 0;
};