#pragma once

#include "scene/Behavior.h"

using namespace engine;

class CrateBehavior : public Behavior {

public:
	using Behavior::Behavior;

	void onStart() override;
	void onUpdate(float deltaTime) override;
};