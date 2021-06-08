#pragma once
#include <UniversoEngine.h>

class LevelEditorApplication : public engine::Application {

protected:

	void onImGuiRender() override;

	void onInitialize() override;

	void onRender() override;

private:
	
	entt::entity selectedEntity;

	using engine::Application::Application;
};