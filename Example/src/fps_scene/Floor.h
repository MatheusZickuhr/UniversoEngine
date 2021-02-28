#include "scene/GameObject.h"

class Floor: public engine::GameObject {

   public:

	using engine::GameObject::GameObject;

	void onStart() override;

	void onUpdate(float deltaTime) override;
    
};