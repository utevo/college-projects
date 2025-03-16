#pragma once

#include "../CompositeObject.h"
#include "../primitives/Ground.h"
#include "Cactus.h"
#include "railway.h"
#include<iostream>

class Floor : public CompositeObject {
public:
	Floor(int number = 1, const glm::vec3& possition = { 0.0, 0.0, 0.0 },
		const glm::vec3& rotation = { 0.0, 0.0, 0.0 },
		const glm::vec3& size = { 1.0f, 1.0f, 1.0f }) {
		auto ground_tex = std::make_shared<Texture>("textures/cracked_ground.png", true);
		auto bs = std::make_unique<Ground>(ground_tex, possition, rotation, size);
		auto rw = std::make_unique<Railway>(6 + 10 * number);
		center = bs.get();
		railway = rw.get();
		addChild(std::move(rw));
		addChild(std::move(bs));
		for (int i = 1; i <= number; ++i)
		{
			addChild(std::move(std::make_unique<Ground>(ground_tex, possition + i*offset, rotation, size)));
			addChild(std::move(std::make_unique<Ground>(ground_tex, possition - i*offset, rotation, size)));
		}
		addCacti(number, possition);
	}

	void reposition(int dir)
	{
		if (dir > 0)
			move(offset);
		else
			move(-offset);
	}

	glm::vec3 getPos()
	{
		return center->getPos();
	}

	void renderRailway(std::shared_ptr<Shader> shader, bool tex = true)
	{
		railway->render(shader, tex);
	}

	void addCacti(int n, const glm::vec3& possition = { 0.0, 0.0, 0.0 })
	{
		auto ctemp = std::make_unique<Cactus>(5, possition + glm::vec3(4.0, -0.5, -1.0));
		addChild(std::move(ctemp));
		ctemp = std::make_unique<Cactus>(2, possition + glm::vec3(-5.0, -0.5, -2));
		addChild(std::move(ctemp));
		ctemp = std::make_unique<Cactus>(7, possition + glm::vec3(-2.0, -0.5, 2));
		addChild(std::move(ctemp));
		for (int i = 1; i <= 2; ++i)
		{
			ctemp = std::make_unique<Cactus>(5, possition + glm::vec3(4.0, -0.5, -1.0) + i*offset);
			addChild(std::move(ctemp));
			ctemp = std::make_unique<Cactus>(2, possition + glm::vec3(-5.0, -0.5, -2) + i * offset);
			addChild(std::move(ctemp));
			ctemp = std::make_unique<Cactus>(5, possition + glm::vec3(4.0, -0.5, -1.0) - i * offset);
			addChild(std::move(ctemp));
			ctemp = std::make_unique<Cactus>(2, possition + glm::vec3(-5.0, -0.5, -2) - i * offset);
			addChild(std::move(ctemp));
			ctemp = std::make_unique<Cactus>(7, possition + glm::vec3(-2.0, -0.5, 2) - i * offset);
			addChild(std::move(ctemp));
			ctemp = std::make_unique<Cactus>(7, possition + glm::vec3(-2.0, -0.5, 2) + i * offset);
			addChild(std::move(ctemp));
		}
	}

private:
	Ground* center;
	Railway* railway;
	const glm::vec3 offset = {0.0, 0.0, 10.0};

};

