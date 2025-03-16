#pragma once

#include "../PrimitiveObject.h"


class Ground : public PrimitiveObject {
public:
	Ground(std::shared_ptr<Texture> texture,
		const glm::vec3& possition = { 0.0, 0.0, 0.0 },
		const glm::vec3& rotation = { 0.0, 0.0, 0.0 },
		const glm::vec3& size = { 1.0f, 1.0f, 1.0f })
		: PrimitiveObject(texture, possition, rotation, size) {
		init();
	}

	std::unique_ptr<VertexBuffer> initVertexBuffer() override {
		GLfloat vertices[] = {
			// coordinates			// normals			// texture
			 10.0f,  -0.5f, -5.0f,	0.0f, 1.0f, 0.0f,	20.0f,  0.0f,
			-10.0f,  -0.5f, -5.0f,	0.0f, 1.0f, 0.0f,	0.0f,   0.0f,
			-10.0f,  -0.5f,  5.0f,	0.0f, 1.0f, 0.0f,	0.0f,   10.0f,
			 10.0f,  -0.5f,  5.0f,	0.0f, 1.0f, 0.0f,	20.0f,  10.0f,
		};

		auto vertexBuffer = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
		return vertexBuffer;
	}

	std::unique_ptr<IndexBuffer> initIndexBuffer() override {
		GLuint indices[] = {
			0, 1, 2,
			0, 2, 3,
		};

		auto indexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices));
		return indexBuffer;
	}
};