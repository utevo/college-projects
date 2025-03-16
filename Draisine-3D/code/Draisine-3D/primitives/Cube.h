
#pragma once

#include "../PrimitiveObject.h"
#define BUMP 0.001f

class Cube : public PrimitiveObject {
public:
	Cube(std::shared_ptr<Texture> texture,
		const glm::vec3& possition = { 0.0, 0.0, 0.0 },
		const glm::vec3& rotation = { 0.0, 0.0, 0.0 },
		const glm::vec3& size = { 1.0f, 1.0f, 1.0f })
		: PrimitiveObject(texture, possition, rotation, size) {
		init();
	}

	std::unique_ptr<VertexBuffer> initVertexBuffer() override {
		GLfloat vertices[] = {
			// coordinates			// normals				// texture
			-1.0f,	-1.0f,	 1.0f,	-1.0f,	-1.0f,	 1.0f,	1.0f ,	0.6666f,	 //0
			 1.0f,	-1.0f,	 1.0f,	 1.0f,	-1.0f,	 1.0f,	0.25f,		0.6666f,	//1
			 1.0f,	 1.0f,	 1.0f,	 1.0f,	 1.0f,	 1.0f,	0.5f,		0.6666f,	//2
			-1.0f,	 1.0f,	 1.0f,	-1.0f,	 1.0f,	 1.0f,	0.75f,		0.6666f,	//3
			-1.0f,	-1.0f,	-1.0f,  -1.0f,	-1.0f,	-1.0f,	1.0f ,	0.3334f,	//4
			 1.0f,	-1.0f,	-1.0f,	 1.0f,	-1.0f,	-1.0f,	0.25f,	0.3334f,	//5
			 1.0f,	 1.0f,	-1.0f,	 1.0f,	 1.0f,	-1.0f,	0.5f,	0.3334f,	//6
			-1.0f,	 1.0f,	-1.0f,	-1.0f,	 1.0f,	-1.0f,	0.75f,		0.3334f,	//7
			-1.0f,	-1.0f,	 1.0f,	-1.0f,	-1.0f,	 1.0f,	0.25f,	1.0f,	//8  0
			-1.0f,	 1.0f,	 1.0f,	-1.0f,	 1.0f,	 1.0f,	0.5f,	1.0f,	//9  3
			-1.0f,	-1.0f,	-1.0f,  -1.0f,	-1.0f,	-1.0f,	0.25f,	0.0f,		//10 4
			-1.0f,	 1.0f,	-1.0f,	-1.0f,	 1.0f,	-1.0f,	0.5f,	0.0f,		//11 7
			-1.0f,	-1.0f,	 1.0f,	-1.0f,	-1.0f,	 1.0f,	0.0f,		0.6666f,	//12 0
			-1.0f,	-1.0f,	-1.0f,  -1.0f,	-1.0f,	-1.0f,	0.0f,		0.3334f,	//13 4
		};

		auto vertexBuffer = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
		return vertexBuffer;
	}

	std::unique_ptr<IndexBuffer> initIndexBuffer() override {
		GLuint indices[] = {
			// front
				8, 1, 2,
				2, 9, 8,
				// right
				1, 5, 6,
				6, 2, 1,
				// back
				11, 6, 5,
				5, 10, 11,
				// left
				4, 0, 3,
				3, 7, 4,
				// bottom
				13, 5, 1,
				1, 12, 13,
				// top
				3, 2, 6,
				6, 7, 3
		};

		auto indexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices));
		return indexBuffer;
	}
	
};
