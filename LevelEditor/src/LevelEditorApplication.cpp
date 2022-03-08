#include "LevelEditorApplication.h"
#include <imgui.h>
#include <GLFW/glfw3.h>
#include <imgui_internal.h>

#include "scene/Components.h"

using namespace engine;

void LevelEditorApplication::onImGuiRender() {
	renderDock();

	ImGui::Begin("Entities");
	this->currentScene->getRegistry().each([this](entt::entity entityId) {

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

		if (this->selectedEntity == entityId) {
			flags |= ImGuiTreeNodeFlags_Selected;

			rendererSelectedEntityProperties();
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

	if (ImGui::BeginPopupContextWindow(0, 1, false)) {
		
		if (ImGui::MenuItem("Create entity")) {
			std::cout << "entity created!" << std::endl;
		}

		ImGui::EndPopup();
	}


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

void LevelEditorApplication::renderDock() {

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
			auto dockIdRight = ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Right, 0.4f, nullptr, &dockspaceId);

			ImGui::DockBuilderDockWindow("Entities", dockIdLeft);
			ImGui::DockBuilderDockWindow("Properties", dockIdRight);
			ImGui::DockBuilderFinish(dockspaceId);
		}
	}

	ImGui::End();
}

void LevelEditorApplication::rendererSelectedEntityProperties() {

	ImGui::Begin("Properties");

	if (ImGui::Button("Add component")) {

		ImGui::OpenPopup("AddComponent");
	}

	if (ImGui::BeginPopup("AddComponent"))
	{
		if (ImGui::MenuItem("Transform component")) {
			this->currentScene->getRegistry().emplace<TransformComponent>(this->selectedEntity);
		}
			

		ImGui::EndPopup();
	}

	

	if (this->currentScene->getRegistry().has<TransformComponent>(this->selectedEntity)) {

		renderSelectedEntiyComponent<TransformComponent>("Transform Component", [this] () {
			auto& transComp = this->currentScene->getRegistry().get<TransformComponent>(this->selectedEntity);
			ImGui::DragFloat3("Position", &transComp.transform.position[0], 0.1f);
			ImGui::DragFloat3("Rotation", &transComp.transform.rotation[0], 0.1f);
			ImGui::DragFloat3("Scale", &transComp.transform.scale[0], 0.1f);
		});
	}

	if (this->currentScene->getRegistry().has<RigidBodyComponent>(this->selectedEntity)) {

		renderSelectedEntiyComponent<RigidBodyComponent>("RigidBody Component", [this]() {
			auto& rigidBodyComp = this->currentScene->getRegistry().get<RigidBodyComponent>(this->selectedEntity);
			
			char* rigidBodyTypesAsString[] = { "Static", "Dynamic", "Kinematic"};
			char* currentSelectedRigidBodyAsString = rigidBodyTypesAsString[0];

			switch (rigidBodyComp.type) {
			case RigidBodyType::Static:
				currentSelectedRigidBodyAsString = rigidBodyTypesAsString[0];
				break;

			case RigidBodyType::Dynamic:
				currentSelectedRigidBodyAsString = rigidBodyTypesAsString[1];
				break;

			case RigidBodyType::Kinematic:
				currentSelectedRigidBodyAsString = rigidBodyTypesAsString[2];
				break;
			default:
				break;
			}

			if (ImGui::BeginCombo("RigidBody type", currentSelectedRigidBodyAsString)) {
				for (int32_t i = 0; i < 2; i++) {

					bool isSelected = currentSelectedRigidBodyAsString == rigidBodyTypesAsString[i];

					if (ImGui::Selectable(rigidBodyTypesAsString[i], isSelected)) {
						currentSelectedRigidBodyAsString = rigidBodyTypesAsString[i];
						std::cout << currentSelectedRigidBodyAsString << std::endl;
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

		});

	}

	if (this->currentScene->getRegistry().has<MaterialComponent>(this->selectedEntity)) {

		renderSelectedEntiyComponent<MaterialComponent>("Material Component", [this]() {
			auto& materialComp = this->currentScene->getRegistry().get<MaterialComponent>(this->selectedEntity);
			ImGui::DragFloat3("Ambient", &materialComp.material->ambient[0], 0.1f);
			ImGui::DragFloat3("Diffuse", &materialComp.material->diffuse[0], 0.1f);
			ImGui::DragFloat3("Specular", &materialComp.material->specular[0], 0.1f);
			ImGui::DragFloat("Shininess", &materialComp.material->shininess, 0.1f);

			if (materialComp.material->getTexture() != nullptr) {
				ImGui::Text("Texture");
				ImGui::Image((void*)(intptr_t)materialComp.material->getTexture()->getId(), ImVec2(200, 200));
			}

		});
	}

	if (this->currentScene->getRegistry().has<DirectionalLightComponent>(this->selectedEntity)) {

		renderSelectedEntiyComponent<DirectionalLightComponent>("Directional Light Component", [this]() {
			auto& dirLightComp = this->currentScene->getRegistry().get<DirectionalLightComponent>(this->selectedEntity);
			ImGui::DragFloat3("Ambient", &dirLightComp.directionalLight.ambient[0], 0.1f);
			ImGui::DragFloat3("Diffuse", &dirLightComp.directionalLight.diffuse[0], 0.1f);
			ImGui::DragFloat3("Specular", &dirLightComp.directionalLight.specular[0], 0.1f);
			ImGui::Text("Depth map texture");
			ImGui::Image((void*)(intptr_t)dirLightComp.directionalLight.getDepthBufferTexture()->getId(), ImVec2(200, 200));
		});
	}

	if (this->currentScene->getRegistry().has<PointLightComponent>(this->selectedEntity)) {

		renderSelectedEntiyComponent<PointLightComponent>("Point Light Component", [this]() {
			auto& pointLightComp = this->currentScene->getRegistry().get<PointLightComponent>(this->selectedEntity);
			ImGui::DragFloat3("Ambient", &pointLightComp.pointLight.ambient[0], 0.1f);
			ImGui::DragFloat3("Diffuse", &pointLightComp.pointLight.diffuse[0], 0.1f);
			ImGui::DragFloat3("Specular", &pointLightComp.pointLight.specular[0], 0.1f);
			ImGui::DragFloat("Constant", &pointLightComp.pointLight.constant, 0.1f);
			ImGui::DragFloat("Linear", &pointLightComp.pointLight.linear, 0.1f);
			ImGui::DragFloat("Quadratic", &pointLightComp.pointLight.quadratic, 0.1f);
			ImGui::DragFloat("Near plane", &pointLightComp.pointLight.nearPlane, 0.1f);
			ImGui::DragFloat("Far plabe", &pointLightComp.pointLight.farPlane, 0.1f);
		});
	}

	ImGui::End();
}
