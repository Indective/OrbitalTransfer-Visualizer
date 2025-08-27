#pragma once

#include "raylib.h"
#include "raymath.h"
#include <vector>

class Body
{
public:
    Vector2 position;
    Vector2 prevPosition;
    Vector2 velocity;
    float mass;
    float drawingRadius;
    Color color;
    std::vector<Vector2> positions;
    float dt = 0.1;
    float G = 10.0f;
    float epsilon = 5.0f;

    void Draw();
    void VerletUpdate(const Vector2 acceleration);
    Vector2 GetAccelerationFrom(const Body& BodyB);
    Vector2 GetVelocityFrom(const Body& BodyB);
};

namespace camera
{
    void UpdateCamera(const std::vector<Body> bodies, Camera2D &camera, int &cameraTarget);
};
