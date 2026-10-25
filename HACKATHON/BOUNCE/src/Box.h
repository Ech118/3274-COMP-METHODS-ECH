#pragma once

#include "Entity.h"


class Box : public Entity {
public:
    Box(float half);     // wireframe cube, 2*half on each side, centered at origin
    float getHalf() const { return half; }
private:
    float half;
};
