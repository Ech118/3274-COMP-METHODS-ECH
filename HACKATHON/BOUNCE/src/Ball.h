#pragma once

#include "Box.h"
#include "Entity.h"


#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoMaterial.h>


class Ball : public Entity {            // Ball is an Entity
public:
    // radius, gravity, initial position, box it bounces in
    Ball(float r, SbVec3f g, SbVec3f p, const Box* box);

    // Helper
    void addVelocity(SbVec3f dv) { vel += dv; }
    void scaleVelocity(float k) { vel *= k; }

    // Each tick
    void update(float dt) override;     // replaces Entity's version

    // Collision Handlers
    void handleBallCollision(Ball& other);
    void handleWallCollision(const Box& box);


private:
    SbVec3f pos, vel{0,0,0}, gravity;
    float radius;
    float boxHalf;
    const Box* box;
    SoTranslation* trans;
    SoMaterial* mat;
};
