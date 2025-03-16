#include "PerspectiveProjection.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Projection.h"


PerspectiveProjection::PerspectiveProjection(float fovy, float aspect, float near, float far) {
	_matrix = glm::perspective(fovy, aspect, near, far);
}

PerspectiveProjection::~PerspectiveProjection() {
}

glm::mat4& PerspectiveProjection::getMatrix() {
	return _matrix;
}
