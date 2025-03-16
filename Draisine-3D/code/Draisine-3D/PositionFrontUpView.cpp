#include "PositionFrontUpView.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "View.h"

PositionFrontUpView::PositionFrontUpView(glm::vec3 position, glm::vec3 front, glm::vec3 up)
	: _position(position), _front(front), _up(up) {
}

PositionFrontUpView::~PositionFrontUpView() {
}

glm::mat4& PositionFrontUpView::getMatrix() {
	_matrix = glm::lookAt(_position, _position + _front, _up);
	return _matrix;
}
