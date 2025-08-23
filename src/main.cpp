#include "body.hpp"
#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <vector>

int main()
{
    const int screenWidth = 2000;
    const int screenHeight = 1000;
    Vector2 totalAcc;
    std::vector<Body> bodies;


    InitWindow(screenWidth,screenHeight, "Orbital Transfer Simulation");

    Body sun;
    sun.position = {screenWidth/2,screenHeight/2};
    sun.velocity = {0,0};
    sun.radius = 100;
    sun.color = MAROON;
    sun.mass = 50;
    sun.prevPosition = sun.position;
    bodies.push_back(sun);

    Body earth;
    earth.position = {300.0f,(screenHeight/2)};
    earth.radius = 50;
    earth.color = BLUE;
    earth.mass = 0.001; 
    earth.velocity = earth.GetVelocityFrom(sun);
    earth.prevPosition = Vector2Subtract(earth.position, Vector2Scale(earth.velocity, earth.dt));
    std::cout << earth.velocity.x << " " << earth.velocity.y << std::endl;
    bodies.push_back(earth);

    Body spacecraft;
    spacecraft.position = {earth.position.x + 70,earth.position.y};
    spacecraft.radius = 3;
    spacecraft.color = GRAY;
    spacecraft.mass = 0.000001; 
    spacecraft.velocity = spacecraft.GetVelocityFrom(earth);
    spacecraft.prevPosition = Vector2Subtract(spacecraft.position, Vector2Scale(spacecraft.velocity, spacecraft.dt));
    bodies.push_back(spacecraft);

    while(!WindowShouldClose()) 
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        for (int i = bodies.size() - 1; i >= 0; --i) 
        {
            totalAcc = {0};
            for (int j = bodies.size() - 1; j >= 0; --j) 
            {
                if (i == j) continue; // skip self
                if(bodies[j].mass == sun.mass && bodies[i].mass == spacecraft.mass) continue;
                if(bodies[i].mass == sun.mass) continue; // skip sun to keep it static
                Vector2 acc = bodies[i].GetAccelerationFrom(bodies[j]);
                totalAcc = Vector2Add(totalAcc, acc);
            }

            bodies[i].VerletUpdate(totalAcc);
        }
        for (auto& b : bodies) 
        {
            b.Draw();
        }

        EndDrawing();
    }

    return 0;
}