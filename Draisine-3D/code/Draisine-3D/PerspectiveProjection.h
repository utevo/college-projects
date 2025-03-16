#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Projection.h"


class PerspectiveProjection : public Projection {
public:
	PerspectiveProjection(float fovy, float aspect, float near, float far);
	~PerspectiveProjection();

	glm::mat4& getMatrix();

private:
	glm::mat4 _matrix;
};

