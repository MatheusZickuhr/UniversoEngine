#pragma once

#include <type_traits>
#include <entt/entt.hpp>
#include "Scene.h"
#include "../debug/Assert.h"

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
            return scene->getRegistry().emplace<T>(entity, std::forward<Args>(args)...);
        }

        template<typename T>
        T& getComponent() {
            ASSERT(this->hasComponent<T>(), "Entity does not have the specified component");

            return scene->getRegistry().get<T>(entity);
        }

        template<typename T>
        void removeComponent() {
            ASSERT(this->hasComponent<T>(), "Entity does not have the specified component");

            scene->getRegistry().remove<T>(entity);
        }

        template<typename T>
        bool hasComponent() {
            return scene->getRegistry().has<T>(entity);
        }

        Scene* getScene() {
            return this->scene;
        }

    };

    template<>
    BehaviorComponent&  Entity::addComponent<BehaviorComponent >();

}