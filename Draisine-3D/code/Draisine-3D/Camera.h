#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/scalar_multiplication.hpp>
#include <memory>

#include "View.h"
#include "PositionFrontUpView.h"

#define GLOBAL_REVERSE_SENSITIVITY 5000
#define UPPER_BOUND 89.0f*glm::half_pi<float>()/90.0f
#define LOWER_BOUND -89.0f*glm::half_pi<float>()/90.0f

class Camera {

public:
	Camera(glm::vec3  cP, glm::vec3  cF, glm::vec3  cU) : first_mouse_call(true),
		cameraPos(cP),
		cameraFront(cF),
		startCameraFront(cF),
		cameraUp(cU),
		_yaw(0.0), _pitch(0.0)
	{
		// _view = std::make_shared<PositionFrontUpView>(cameraPos, cameraFront, cameraUp);
	}


	//std::shared_ptr<PositionFrontUpView> Get_view() { return _view; }

	void Rotate_Mouse(double X_offset, double Y_offset) {

		int X_sensitivity = 20; //range 1-100
		int Y_sensitivity = 20; //range 1-100






		X_offset *= X_sensitivity;
		Y_offset *= Y_sensitivity;

		X_offset /= GLOBAL_REVERSE_SENSITIVITY;
		Y_offset /= GLOBAL_REVERSE_SENSITIVITY;

		_yaw += (X_offset / glm::half_pi<float>());
		_pitch += (Y_offset / glm::half_pi<float>());

		if (_pitch > UPPER_BOUND)_pitch = UPPER_BOUND;
		if (_pitch < LOWER_BOUND)_pitch = LOWER_BOUND;
		
		Rotate_Vectors();
	}

	void Step_Longtitudal(double step) {
		cameraPos += step * cameraFront;
	}
	void Step_Lateral(const double& step) {
		glm::mat4 quarterRotation(1);
		quarterRotation = glm::rotate(quarterRotation, -glm::half_pi<float>(), cameraUp);
		glm::vec3 lateral_vector(quarterRotation * glm::vec4(cameraFront, 1.0));
		lateral_vector[1] = 0.0f;
		cameraPos += step * lateral_vector;
	}
	
    void Step_Vertical(const double& step)
	{
		cameraPos += step * cameraUp; //glm::vec3(0.0f, 1.0f, 0.0f);
	}

	glm::vec3 getFront() { return cameraFront; }
	glm::vec3 getUp() { return cameraUp; }
	glm::vec3 getPos() { return cameraPos; }

	void setPos(glm::vec3 a) { cameraPos = a; }
	void setUp(glm::vec3 a) { cameraUp = a; }
	void setFront(glm::vec3 a) { cameraFront = a; _yaw = 0.0; _pitch = 0.0; }

private:

	void Rotate_Vectors() {
		glm::mat4 rotationMat(1);
		rotationMat = glm::rotate(rotationMat, float(_yaw), glm::vec3(0.0, 1.0, 0.0));
		rotationMat = glm::rotate(rotationMat, float(_pitch), glm::vec3(1.0, 0.0, 0.0));
		cameraFront = glm::vec3 (0.0f, 0.0f, -1.0f);
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		cameraFront = glm::vec3(rotationMat * glm::vec4(cameraFront, 1.0));
		cameraUp = glm::vec3(rotationMat * glm::vec4(cameraUp, 1.0));
	}


	bool first_mouse_call;
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	const glm::vec3 startCameraFront;
	glm::vec3 cameraUp;
	double _yaw;
	double _pitch;
	//std::shared_ptr<PositionFrontUpView> _view;
};