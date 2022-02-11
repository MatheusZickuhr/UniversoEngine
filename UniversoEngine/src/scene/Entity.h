#pragma once

#include <type_traits>
#include <entt/entt.hpp>
#include "Scene.h"
#include "../debug/Assert.h"

namespace engine {
    
    struct RigidBodyComponent;
    
    struct BehaviorComponent;

    class Entity {

    friend class Scene;

    private:
        entt::entity enttEntity;
        Scene* scene;

    public:

        Entity(entt::entity enttEntity, Scene* scene)
        : enttEntity(enttEntity), scene(scene){}

        template<typename T, typename... Args>
        T& addComponent(Args &&... args) {
            return scene->getRegistry().emplace<T>(enttEntity, std::forward<Args>(args)...);
        }

        template<typename T>
        T& getComponent() {
            ASSERT(this->hasComponent<T>(), "Entity does not have the specified component");

            return scene->getRegistry().get<T>(enttEntity);
        }

        template<typename T>
        void removeComponent() {
            ASSERT(this->hasComponent<T>(), "Entity does not have the specified component");

            scene->getRegistry().remove<T>(enttEntity);
        }

        template<typename T>
        bool hasComponent() {
            return scene->getRegistry().has<T>(enttEntity);
        }

        Scene* getScene() {
            return this->scene;
        }

    };

    template<>
    BehaviorComponent&  Entity::addComponent<BehaviorComponent >();

}