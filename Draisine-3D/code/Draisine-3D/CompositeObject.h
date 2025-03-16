#pragma once

#include <vector>
#include <memory>
#include <exception>
#include "Object.h"


class CompositeObject : public Object {
public:
	virtual void render(std::shared_ptr<Shader> shader, bool tex = true) override {
		for (auto& object : _objects)
			object->render(shader, tex);
	}


	void addChild(std::unique_ptr<Object>&& object) {
		_objects.push_back(std::move(object));
	}

	void removeChild(unsigned int number) {
		if(_objects.size() <= number)
			throw std::out_of_range("Index out of range");
		_objects.erase(_objects.begin() + number);
	}


	virtual void move(const glm::vec3& vec) override {
		for (auto& object : _objects)
			object->move(vec);
	}

	virtual void rotate(const glm::vec3& vec) override {
		for (auto& object : _objects)
			object->rotate(vec);
	}

	virtual void scale(const glm::vec3& vec) override {
		for (auto& object : _objects)
			object->scale(vec);
	}

private:
	std::vector <std::unique_ptr<Object>> _objects;
};