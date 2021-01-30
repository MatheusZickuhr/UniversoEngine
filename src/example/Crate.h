#pragma once

class Crate : public engine::GameObject {

public:

	Crate() {
		this->mesh = std::make_shared<engine::Mesh>("res/models/crate/crate.obj");
		this->texture = std::make_shared<engine::Texture>("res/textures/crate/crate.jpg");
	}

	void onStart() override {

	}

	void onUpdate(float deltaTime) override {

	}
};