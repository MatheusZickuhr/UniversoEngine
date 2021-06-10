#pragma once
#include <UniversoEngine.h>
#include  <functional> 

class LevelEditorApplication : public engine::Application {

protected:

	void onImGuiRender() override;

	void onInitialize() override;

	void onRender() override;

private:

	entt::entity selectedEntity;

	using engine::Application::Application;

	void renderDock();

	void rendererSelectedEntityProperties();

	template<typename T>
	void renderSelectedEntiyComponent(const std::string& componentName, const std::function<void()>& renderComponentInternals) {
		ImGui::Text(componentName.c_str());

		ImGui::SameLine();
		bool componentRemoved = false;
		if (ImGui::Button("x"))
			componentRemoved = true;

		renderComponentInternals();

		if (componentRemoved)
			this->currentScene->getRegistry().remove<T>(this->selectedEntity);
	}

};