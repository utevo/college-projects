#pragma once
#include "../PrimitiveObject.h"
#include <iostream>
#define SIDES 38 //does not work for 64 and higher
#define POINT_VALUES 8
#define TEXTURE_RAD_OFFSET 0.005f
class Cylinder : public PrimitiveObject {
public:
	Cylinder(std::shared_ptr<Texture> texture,
		const glm::vec3& possition = { 0.0, 0.0, 0.0 },
		const glm::vec3& rotation = { 0.0, 0.0, 0.0 },
		const glm::vec3& size = { 1.0f, 1.0f, 1.0f })
		: PrimitiveObject(texture, possition, rotation, size) {
		init();
	}
	std::unique_ptr<VertexBuffer> initVertexBuffer() override {
		GLfloat vertices[(4 + SIDES * (4)) * POINT_VALUES];
		int count = 0;
		/*float radius = 1.0f;
		float length = 1.0f;*/
		float sector_angle = glm::two_pi<float>() / float(SIDES); //360degrees/SIDES;
		int i = 0;
		/*// coordinates			// normals			// texture
			 0.25f,  0.5f,  0.0f,	1.0f, 0.0f, 0.0f,	1.0f,  0.0f,
			 */

		vertices[i] = 0.0f;
		vertices[i + 1] = 0.0f;
		vertices[i + 2] = 0.0f;
		vertices[i + 3] = 0.0f;
		vertices[i + 4] = 0.0f;
		vertices[i + 5] = -1.0f;
		vertices[i + 6] = 0.25f;
		vertices[i + 7] = 0.25f;

		i += 8;
		//0 bottom center
		vertices[i] = 0.0f;
		vertices[i + 1] = 0.0f;
		vertices[i + 2] = 1.0f;
		vertices[i + 3] = 0.0f;
		vertices[i + 4] = 0.0f;
		vertices[i + 5] = 1.0f;
		vertices[i + 6] = 0.75f;
		vertices[i + 7] = 0.25f;
		i += 8;
		//1 top center
		for (int counter = 0; counter <= SIDES; counter++) {
			vertices[i] = glm::cos(sector_angle * counter);
			vertices[i + 1] = glm::sin(sector_angle * counter);
			vertices[i + 2] = 0.0f;
			vertices[i + 3] = glm::cos(sector_angle * counter);
			vertices[i + 4] = glm::sin(sector_angle * counter);
			vertices[i + 5] = -1.0f;
			vertices[i + 6] = float(counter) / float(SIDES);
			vertices[i + 7] = 0.5f;
			i += 8;
		}
		//2+ SIDES bottom_end

		for (int counter = 0; counter <= SIDES; counter++) {
			vertices[i] = glm::cos(sector_angle * counter);
			vertices[i + 1] = glm::sin(sector_angle * counter);
			vertices[i + 2] = 1.0f;
			vertices[i + 3] = glm::cos(sector_angle * counter);
			vertices[i + 4] = glm::sin(sector_angle * counter);
			vertices[i + 5] = 1.0f;
			vertices[i + 6] = float(counter) / float(SIDES);
			vertices[i + 7] = 1.0f;
			i += 8;
		}
		//3+ 2* SIDES top_end
		for (int counter = 0; counter < SIDES; counter++) {
			vertices[i] = glm::cos(sector_angle * counter);
			vertices[i + 1] = glm::sin(sector_angle * counter);
			vertices[i + 2] = 0.0f;
			vertices[i + 3] = glm::cos(sector_angle * counter);
			vertices[i + 4] = glm::sin(sector_angle * counter);
			vertices[i + 5] = -1.0f;
			vertices[i + 6] = 0.25f + (0.25f - TEXTURE_RAD_OFFSET) * glm::cos(sector_angle * counter);
			vertices[i + 7] = 0.25f + (0.25f - TEXTURE_RAD_OFFSET) * glm::sin(sector_angle * counter);
			i += 8;
		}
		//3+3*SIDES bottom_shadow
		for (int counter = 0; counter < SIDES; counter++) {
			vertices[i] = glm::cos(sector_angle * counter);
			vertices[i + 1] = glm::sin(sector_angle * counter);
			vertices[i + 2] = 1.0f;
			vertices[i + 3] = glm::cos(sector_angle * counter);
			vertices[i + 4] = glm::sin(sector_angle * counter);
			vertices[i + 5] = 1.0f;
			vertices[i + 6] = 0.75f + (0.25f - TEXTURE_RAD_OFFSET) * glm::cos(sector_angle * counter);
			vertices[i + 7] = 0.25f + (0.25f - TEXTURE_RAD_OFFSET) * glm::sin(sector_angle * counter);
			i += 8;
		}
		//3+4*SIDES top_shadow
		auto vertexBuffer = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
		return vertexBuffer;
	}
	std::unique_ptr<IndexBuffer> initIndexBuffer() override {
		GLuint indices[3 * 2 * SIDES * 2];
		int i = 0;
		for (int counter = 0; counter < SIDES; counter++) {
			indices[i] = 0;
			indices[i + 1] = 4 + (2) * SIDES + counter;
			indices[i + 2] = 4 + (2) * SIDES + (counter + 1) % SIDES;
			i += 3;
		}//bottom shadow

		for (int counter = 0; counter < SIDES; counter++) {
			indices[i] = 1;
			indices[i + 1] = 4 + (3) * SIDES + counter;
			indices[i + 2] = 4 + (3) * SIDES + (counter + 1) % SIDES;
			i += 3;
		}//top shadow

		for (int counter = 0; counter < SIDES; counter++) {
			indices[i] = counter + 2;
			indices[i + 1] = counter + 1 + 2;
			indices[i + 2] = counter + 2 + (SIDES + 1);
			indices[i + 3] = counter + 1 + 2;
			indices[i + 4] = counter + 2 + (SIDES + 1);
			indices[i + 5] = counter + 1 + 2 + (SIDES + 1);
			i += 6;
		}
		//std::cout << "i" << i << "estimate" << 3 * 2 * SIDES * 2 << '\n';
		
auto indexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices));
//std::cout << "succ\n";
		return indexBuffer;
	}
};