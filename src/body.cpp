#include "body.hpp"
#include "raymath.h"
#include <cmath>
#include <iostream>

void Body::Draw()
{
    DrawCircleV(position, drawingRadius, color);
}

void Body::VerletUpdate(const Vector2 acceleration)
{
    Vector2 temp = position; //xn
    Vector2 delta = Vector2Subtract(position, prevPosition);
    Vector2 accelTerm = Vector2Scale(acceleration, dt * dt);
    position = Vector2Add(Vector2Add(position, delta), accelTerm);
    prevPosition = temp;
}

Vector2 Body::GetAccelerationFrom(const Body& bodyB)
{
    Vector2 dir = Vector2Subtract(bodyB.position, position);
    float r2 = Vector2LengthSqr(dir) + epsilon * epsilon;
    Vector2 dirNorm = Vector2Normalize(dir);
    float forceMag = (G * bodyB.mass) / r2;
    return Vector2Scale(dirNorm, forceMag);
}

Vector2 Body::GetVelocityFrom(const Body& BodyB)
{
    Vector2 radius = {position.x - BodyB.position.x,position.y - BodyB.position.y};
    float r = sqrtf((radius.x * radius.x) + (radius.y * radius.y));
    Vector2 t = {radius.y, -radius.x};
    Vector2 tNormal = t/sqrtf(t.x * t.x + t.y * t.y);
    float v = sqrtf(G * BodyB.mass/r);
    Vector2 velocity = Vector2Scale(tNormal,v);
    return velocity;
}

void camera::UpdateCamera(const std::vector<Body> bodies, Camera2D &camera, int &cameraTarget)
{
    if(cameraTarget == -1) return;
    if (cameraTarget >= 0 && cameraTarget < (int)bodies.size())
    {
        camera.target = bodies[cameraTarget].position;
    }
}
