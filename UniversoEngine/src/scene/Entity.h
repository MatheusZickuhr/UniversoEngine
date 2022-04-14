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


        Entity()
            : enttEntity((entt::entity) 0), scene(nullptr) {
        }

        Entity(entt::entity enttEntity, Scene* scene)
        : enttEntity(enttEntity), scene(scene){}

        template<typename T, typename... Args>
        T& addComponent(Args &&... args) {
            ASSERT(isValid(), "Entity must be valid (It was probably destroyed or not initialized)");
            return scene->getRegistry().emplace<T>(enttEntity, std::forward<Args>(args)...);
        }

        template<typename T>
        T& getComponent() {
            ASSERT(isValid(), "Entity must be valid (It was probably destroyed or not initialized)");
            ASSERT(this->hasComponent<T>(), "Entity does not have the specified component");
            return scene->getRegistry().get<T>(enttEntity);
        }

        template<typename T>
        void removeComponent() {
            ASSERT(isValid(), "Entity must be valid (It was probably destroyed or not initialized)");
            ASSERT(this->hasComponent<T>(), "Entity does not have the specified component");

            scene->getRegistry().remove<T>(enttEntity);
        }

        template<typename T>
        bool hasComponent() {
            ASSERT(isValid(), "Entity must be valid (It was probably destroyed or not initialized)");
            return scene->getRegistry().has<T>(enttEntity);
        }

        bool isValid() {
            return scene->getRegistry().valid(enttEntity);
        }

        Scene* getScene() {
            return this->scene;
        }

    };

    template<>
    BehaviorComponent&  Entity::addComponent<BehaviorComponent >();

}