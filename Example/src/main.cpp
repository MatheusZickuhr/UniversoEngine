
#include <UniversoEngine.h>

#include "fps_scene/FpsScene.h"
#include "point_light_scene/PointLightScene.h"
#include "directional_light_scene/DirectionalLightScene.h"
#include "procedural_terrain_scene/ProceduralTerrainScene.h"

int main() {	
	using namespace engine;

	Application app;

	app.initiliaze<PointLightScene>(800, 600, "Example");
	app.run();
	
	return 0;
}
