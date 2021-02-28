#include "FpsScene.h"

void FpsScene::onStart() {
    auto mesh = std::make_shared<engine::Mesh>("res/models/crate/crate.obj");
    auto texture = std::make_shared<engine::Texture>("res/textures/crate/crate.jpg");

    auto floor = std::make_shared<Floor>(mesh, texture);
    auto player = std::make_shared<Player>(mesh, texture);
    player->setCamera(this->camera);

    this->appendGameObject(floor);
    this->appendGameObject(player);
}

void FpsScene::onUpdate(float deltaTime) {

}