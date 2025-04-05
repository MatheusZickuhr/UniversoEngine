
#include <UniversoEngine.h>

#include "point_light_scene/PointLightScene.h"
#include "directional_light_scene/DirectionalLightScene.h"
#include "asteroids_clone/AsteroidsCloneScene.h"

int main() {	
	using namespace engine;

	std::unique_ptr<DirectionalLightScene> scene = std::make_unique<DirectionalLightScene>();

	WindowSettings windowSettings;
	windowSettings.name = "Example";
	windowSettings.width = 800;
	windowSettings.height = 600;

	Application app{ std::move(scene), windowSettings };
	app.run();


	return 0;
}
