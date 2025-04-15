#pragma once

//#include "Scene.h"

namespace engine {

    class Scene;

    class NewSceneListener {
    
    public:

		virtual ~NewSceneListener() = default;

        virtual void onNewScene(std::unique_ptr<Scene> scene) = 0;
    };
}