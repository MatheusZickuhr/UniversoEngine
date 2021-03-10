#include "Behavior.h"
namespace engine {

    void Behavior::initialize() {
        this->initialized = true;
    }

    bool Behavior::isInitialized() {
        return this->initialized;
    }

}
