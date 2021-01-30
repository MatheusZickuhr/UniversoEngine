#pragma once

#include "Crate.h"

class ExampleLevel : public engine::Level {

public:

	void onStart() override {
		auto crate = this->createGameObject<Crate>();
		auto crateClone = this->cloneGameObject<Crate>(crate);
		crateClone->position = glm::vec3(3.0f, 0.0f, 0.0f);
	}

	void onUpdate(float deltaTime) override {

	}
};

