#pragma once

#include <Inventor/SbVec3f.h>
#include <Inventor/nodes/SoSeparator.h>


class Entity {
public:
    Entity();
    virtual ~Entity();
    virtual void update(float dt) {}    // default is do nothing
    SoSeparator* getNode() { return root; }

    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;

protected:          // children can use these
    SoSeparator* root;
};
