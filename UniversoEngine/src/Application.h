#pragma once

#include "window.h"
#include "scene/Scene.h"
#include "scene/NewSceneListener.h"
#include "debug/Assert.h"
#include "renderer/renderer_api/DrawApi.h"

struct GLFWwindow;

namespace engine {

	struct WindowSettings {
		int32_t width;
		int32_t height;
		const char* name;
	};

	class Application : NewSceneListener {

	public:

		Application(
			std::unique_ptr<Scene> scene,
			WindowSettings windowSettings
		);
		
		Application(Application const&) = delete;

		void run();

		void onNewScene(std::unique_ptr<Scene> scene) override;
	
	protected:

		

		virtual void onImGuiRender();

		virtual void onRender();

		virtual void onInitialize();

	private:
		
		std::shared_ptr<Window> window;

		std::unique_ptr<Scene> currentScene;
		std::unique_ptr<Scene> nextScene;

		std::shared_ptr<PhysicsWorld> physicsWorld;
		std::shared_ptr<Renderer3D> renderer3d;
		std::shared_ptr<Renderer2D> renderer2d;

		void renderImGui();

		void setCurrentScene(std::unique_ptr<Scene> scene);

	};
}