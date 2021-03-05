#pragma once

#include <entt/entt.hpp>
#include "Scene.h"

namespace engine {

    class Entity {

    private:
        entt::entity entity;
        Scene* scene;

    public:

        Entity(entt::entity entity, Scene* scene)
        : entity(entity), scene(scene){}

        template<typename T, typename... Args>
        T& addComponent(Args &&... args) {
            T& comp = scene->registry.emplace<T>(entity, std::forward<Args>(args)...);
            return comp;
        }

        template<typename T>
        T& getComponent() {
            return scene->registry.get<T>(entity);
        }

        template<typename T>
        bool hasComponent() {
            return scene->registry.has<T>(entity);
        }

    };

}