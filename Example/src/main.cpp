
#include <UniversoEngine.h>

#include "point_light_scene/PointLightScene.h"
#include "directional_light_scene/DirectionalLightScene.h"
#include "asteroids_clone/AsteroidsCloneScene.h"

int main() {	
	using namespace engine;

	Application app;

	app.initiliaze<DirectionalLightScene>(800, 600, "Example");
	app.run();
	
	return 0;
}
