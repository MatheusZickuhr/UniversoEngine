#include "LevelEditorApplication.h"
#include <imgui.h>
#include <GLFW/glfw3.h>
#include <imgui_internal.h>

#include "scene/Components.h"

using namespace engine;

void LevelEditorApplication::onImGuiRender() {
	static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	windowFlags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
		windowFlags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace", nullptr, windowFlags);
	ImGui::PopStyleVar();
	ImGui::PopStyleVar(2);

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
		ImGuiID dockspaceId = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);

		static auto first_time = true;
		if (first_time) {
			first_time = false;

			ImGui::DockBuilderRemoveNode(dockspaceId);
			ImGui::DockBuilderAddNode(dockspaceId, dockspaceFlags | ImGuiDockNodeFlags_DockSpace);
			ImGui::DockBuilderSetNodeSize(dockspaceId, viewport->Size);

			auto dockIdLeft = ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Left, 0.2f, nullptr, &dockspaceId);
			auto dockIdRight = ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Right, 0.5f, nullptr, &dockspaceId);

			ImGui::DockBuilderDockWindow("Entities", dockIdLeft);
			ImGui::DockBuilderDockWindow("Properties", dockIdRight);
			ImGui::DockBuilderFinish(dockspaceId);
		}
	}

	ImGui::End();

	ImGui::Begin("Entities");
	this->currentScene->getRegistry().each([this](entt::entity entityId) {

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

		if (this->selectedEntity == entityId) {
			flags |= ImGuiTreeNodeFlags_Selected;
			ImGui::Begin("Properties");
			
			if (this->currentScene->getRegistry().has<TransformComponent>(entityId)) {
				auto& transComp = this->currentScene->getRegistry().get<TransformComponent>(entityId);

				ImGui::Text("Transform Component");

				ImGui::DragFloat3("Position", &transComp.transform.position[0], 0.1f);
				ImGui::DragFloat3("Rotation", &transComp.transform.rotation[0], 0.1f);
				ImGui::DragFloat3("Scale", &transComp.transform.scale[0], 0.1f);
			}

			ImGui::End();
		}
		
		if (this->currentScene->getRegistry().has<NameComponent>(entityId)) {
			auto& nameComp = this->currentScene->getRegistry().get<NameComponent>(entityId);
			ImGui::TreeNodeEx((void*) entityId, flags, nameComp.name.c_str());
		}
		else {
			ImGui::TreeNodeEx((void*) entityId, flags, "Entity");
		}

		if (ImGui::IsItemClicked())
			selectedEntity = entityId;
	});
	ImGui::End();

	ImGui::Begin("Performance");
	ImGui::Text("Frametime: %.1f", 1000.0f / ImGui::GetIO().Framerate);
	ImGui::Text("Fps: %.1f", ImGui::GetIO().Framerate);
	ImGui::End();
}

void LevelEditorApplication::onInitialize() {
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void LevelEditorApplication::onRender() {

}
