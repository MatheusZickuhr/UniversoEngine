
#include <UniversoEngine.h>
#include "DefaultLevelEditorScene.h"
#include "LevelEditorApplication.h"

int main() {	
	using namespace engine;

	LevelEditorApplication app;

	app.initiliaze<DefaultLevelEditorScene>(800, 600, "Level Editor");
	app.run();

	return 0;
}
