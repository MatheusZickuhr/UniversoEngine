#pragma once

#include <type_traits>
#include <entt/entt.hpp>
#include "Scene.h"

namespace engine {
    
    struct RigidBodyComponent;
    
    struct BehaviorComponent;

    class Entity {

    private:
        entt::entity entity;
        Scene* scene;

    public:

        Entity(entt::entity entity, Scene* scene)
        : entity(entity), scene(scene){}

        template<typename T, typename... Args>
        T& addComponent(Args &&... args) {
            return scene->registry.emplace<T>(entity, std::forward<Args>(args)...);
        }

        template<typename T>
        T& getComponent() {
            return scene->registry.get<T>(entity);
        }

        template<typename T>
        bool hasComponent() {
            return scene->registry.has<T>(entity);
        }

        Scene* getScene() {
            return this->scene;
        }

    };

    template<>
    RigidBodyComponent& Entity::addComponent<RigidBodyComponent>();

    template<>
    BehaviorComponent&  Entity::addComponent<BehaviorComponent >();

}