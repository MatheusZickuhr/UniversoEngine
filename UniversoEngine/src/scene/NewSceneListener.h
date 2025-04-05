#pragma once

//#include "Scene.h"

namespace engine {

    class Scene;

    class NewSceneListener {
    public:

        virtual void onNewScene(std::unique_ptr<Scene> scene) = 0;
    };
}