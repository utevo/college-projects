#include "OrtogonalProjection.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


OrtogonalProjection::OrtogonalProjection(float left, float right, float bottom, float top) {
	_matrix = glm::ortho(left, right, bottom, top);
}

OrtogonalProjection::OrtogonalProjection(float left, float right, float bottom, float top, float zNear, float zFar) {
	_matrix = glm::ortho(left, right, bottom, top, zNear, zFar);
}

OrtogonalProjection::~OrtogonalProjection() {
}

glm::mat4& OrtogonalProjection::getMatrix() {
	return _matrix;
}
