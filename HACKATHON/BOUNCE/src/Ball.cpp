#include "Ball.h"


Ball::Ball(float r, SbVec3f g, SbVec3f p, const Box* b)
    : Entity(), pos(p), gravity(g), radius(r), box(b)
{
    trans = new SoTranslation;
    mat = new SoMaterial;
    SoSphere* s = new SoSphere;
    s->radius = r;
    root->addChild(trans);   // Root from Entity
    root->addChild(mat);
    root->addChild(s);

    trans->translation.setValue(pos);
}

void Ball::handleBallCollision(Ball& other)
{   
    // Seperation vector
    SbVec3f d = other.pos - pos;
    float radii = other.radius + radius;
    if (d.sqrLength() < radii * radii) // Avoiding sqrt
    {
        // Let n̂ = (p2 − p1) / |p2 − p1|
        float dist = d.length();
        if (dist == 0) return; // same spot: no defined direction, skip it
        SbVec3f normal = d / dist;

        float overlap = radii - dist;
        SbVec3f push = normal * (overlap / 2);
        pos -= push; 
        other.pos += push; 

        float relative = (vel - other.vel).dot(normal);
        if (relative <= 0) return; // Already moving apart
        vel -= relative * normal;
        other.vel += relative * normal;
    }
}

void Ball::handleWallCollision(const Box& box)
{
    float bPos = box.getHalf();
    // Iterate over each dimension
    // 0 x, 1 y, 2 z
    for(int i = 0; i<3; i++)
    {
        // Ball is outside of box 
        if(fabs(pos[i]) + radius > bPos)
        {
            pos[i] = (pos[i] > 0) ? (bPos - radius) : (-bPos + radius);
            vel[i] = -vel[i];
        }
    }   
}

void Ball::update(float dt)
{
    vel += gravity * dt;
    pos += vel * dt;

    handleWallCollision(*box);

    trans->translation.setValue(pos);

    const float vMax = 20.0f;
    float t = vel.length() / vMax;
    if (t > 1) t = 1;      // t in [0, 1]
    mat->diffuseColor.setValue(t, 0.2f, 1 - t);
}


