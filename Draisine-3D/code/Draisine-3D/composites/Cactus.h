#pragma once

#include "../CompositeObject.h"
#include "../primitives/Cylinder.h"
#include "../primitives/Sphere.h"
#include <iostream>

#define MAX_BRANCHES 4
#define MAX_STEM_HEIGHT 10
#define STEM_OFFSET 6
#define BRANCH_OFFSET 3
#define SEED 123456
#define SEED_BUMP 17
class Cactus : public CompositeObject {
public:
	Cactus(int type, const glm::vec3& possition = { 2.0, -0.51, 0.0 }) {

		const glm::vec3& rotation = { -90.0f, 0.0f, -90.0f };
		const glm::vec3& size = { 0.1f, 0.1f, 0.1f };
		type += SEED_BUMP;
		glm::vec3 stem_size(size);
		glm::vec3 connection_size(size);
		connection_size[0] /= 2;
		connection_size[1] /= 2;
		connection_size[2] *= 2.5f;
		glm::vec3 height_unit = { 0.0f,0.1f,0.0f };
		glm::vec3 branch_offset_horizontal = { 0.25f,0.0f,0.0f };
		auto cacti_tex = std::make_shared<Texture>("textures/cacti_tex.png");
		int branch_nr = 2;
		int stem_height = (type * SEED) % (MAX_STEM_HEIGHT + 1 - STEM_OFFSET) + STEM_OFFSET; //

		stem_size[2] *= stem_height;
		auto stem = std::make_unique<Cylinder>(cacti_tex, possition, rotation, stem_size);
		glm::vec3 top_position(possition);
		top_position[1] += stem_size[2];
		auto stem_top = std::make_unique<Sphere>(cacti_tex, top_position, rotation, size);
		addChild(std::move(stem));
		addChild(std::move(stem_top));
		float branch_revolution = glm::two_pi<float>() / float(branch_nr);
		//if (branch_nr != 0)branch_revolution /= branch_nr;
		glm::mat4 rotationMat(1);
		rotationMat = glm::rotate(rotationMat, branch_revolution, glm::vec3(0.0, 1.0, 0.0));

		int lower_bound = 0;
		int upper_bound = 0;
		for (int i = 0; i < branch_nr; i++) {

			lower_bound = (i*type * SEED) % (stem_height - BRANCH_OFFSET) + BRANCH_OFFSET / 2;
			upper_bound = 0;
			int tmp_seed = SEED;
			while (upper_bound <= lower_bound) { upper_bound = (i * upper_bound + type + tmp_seed) % (stem_height)+BRANCH_OFFSET / 2; tmp_seed += SEED_BUMP; } //std::cout << "U:" << upper_bound << "L:" << lower_bound << '\n'; }
			glm::vec3 branch_size(size);
			branch_size[2] *= (upper_bound - lower_bound);

			auto branch_top = std::make_unique<Sphere>(cacti_tex, possition + branch_offset_horizontal + (height_unit * upper_bound), rotation, size);

			auto branch_bottom = std::make_unique<Sphere>(cacti_tex, possition + branch_offset_horizontal + (height_unit * lower_bound), rotation, size);

			auto branch = std::make_unique<Cylinder>(cacti_tex, possition + branch_offset_horizontal + (height_unit * lower_bound), rotation, branch_size);
			glm::vec3 half_offset(branch_offset_horizontal);
			half_offset[0] /= 3;
			half_offset[1] /= 3;
			half_offset[2] /= 3;
			glm::vec3 con_rot = {
			0.0f, (branch_revolution * float(i + 1)) * 360.0f / glm::two_pi<float>() , 0.0f
			};


			auto connection = std::make_unique<Cylinder>(cacti_tex, possition + half_offset + (height_unit * lower_bound) + glm::vec3(0.0, 0.0, 0.0), con_rot + glm::vec3(0.0, -90.0, 0.0), connection_size);
			addChild(std::move(connection));
			addChild(std::move(branch));
			addChild(std::move(branch_top));
			addChild(std::move(branch_bottom));
			branch_offset_horizontal = glm::vec3(rotationMat * glm::vec4(branch_offset_horizontal, 1.0));

		}
	}

	void reposition(int dir)
	{
		if (dir > 0)
			move(offset);
		else
			move(-offset);
	}


	/*glm::vec3 getPos()
	{
		return base->getPos();
	}
	*/
private:
	const glm::vec3 offset = { 0.0, 0.0, 10.0 };

};

