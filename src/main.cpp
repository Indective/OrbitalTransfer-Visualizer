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
    float pixelsPerAU = 500.0f; // 1 AU = 500 pixels
    std::vector<Body> bodies;
    int cameraTarget = 0;
    Texture2D background = LoadTexture("assests/BackGround.png");

    InitWindow(screenWidth,screenHeight, "Orbital Transfer Simulation");
    
    Body sun;
    sun.position = { screenWidth/2.0f, screenHeight/2.0f };
    sun.velocity = { 0, 0 };
    sun.drawingRadius = 20;    
    sun.color = MAROON;
    sun.mass = 200;            
    sun.prevPosition = sun.position;
    bodies.push_back(sun);

    Body earth;
    earth.position = { sun.position.x + 1.0f * pixelsPerAU, sun.position.y }; 
    earth.drawingRadius = 8;   
    earth.color = BLUE;
    earth.mass = 100; 
    earth.velocity = earth.GetVelocityFrom(sun);
    earth.prevPosition = Vector2Subtract(earth.position, Vector2Scale(earth.velocity, earth.dt));
    bodies.push_back(earth);

    Body spacecraft;
    spacecraft.position = { earth.position.x + 20.0f, earth.position.y }; 
    spacecraft.drawingRadius = 3;  
    spacecraft.color = GRAY;
    spacecraft.mass = 5.03e-26;
    spacecraft.velocity = Vector2Add(spacecraft.GetVelocityFrom(earth), earth.velocity);
    spacecraft.prevPosition = Vector2Subtract(spacecraft.position, Vector2Scale(spacecraft.velocity, spacecraft.dt));
    bodies.push_back(spacecraft);
    
    Body moon;
    moon.position = {earth.position.x + 80.0f, earth.position.y }; 
    moon.drawingRadius = 5;  
    moon.color = GRAY;
    moon.mass = 8;
    moon.velocity = Vector2Add(moon.GetVelocityFrom(earth), earth.velocity);
    moon.prevPosition = Vector2Subtract(moon.position, Vector2Scale(moon.velocity, moon.dt));
    bodies.push_back(moon);    

    
    Camera2D camera = {0};
    camera.target = (Vector2){ screenWidth/2, screenHeight/2 };
    camera.offset = (Vector2){ screenWidth/2, screenHeight/2 }; 
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    
    while(!WindowShouldClose()) 
    {

        camera.zoom += GetMouseWheelMove() * 0.1f;

        if (camera.zoom < 0.01f) camera.zoom = 0.01f;
        if (camera.zoom > 10.0f) camera.zoom = 10.0f;

        if (IsKeyDown(KEY_RIGHT)) camera.target.x += 10 / camera.zoom;
        if (IsKeyDown(KEY_LEFT))  camera.target.x -= 10 / camera.zoom;
        if (IsKeyDown(KEY_DOWN))  camera.target.y += 10 / camera.zoom;
        if (IsKeyDown(KEY_UP))    camera.target.y -= 10 / camera.zoom;
        if (IsKeyPressed(KEY_M))
        {
            cameraTarget = (cameraTarget + 1) % bodies.size();
        }

        for (int i = bodies.size() - 1; i >= 0; --i) 
        {
            totalAcc = {0};
            for (int j = bodies.size() - 1; j >= 0; --j)        
            {
                if (i == j) continue; // skip self
                if(bodies[j].mass == sun.mass && bodies[i].mass == spacecraft.mass) continue; // skip to keep spacecraft orbiting earth
                if(bodies[i].mass == sun.mass) continue; // skip sun to keep it static
                Vector2 acc = bodies[i].GetAccelerationFrom(bodies[j]);
                totalAcc = Vector2Add(totalAcc, acc);
            }
            bodies[i].VerletUpdate(totalAcc);
        }

        BeginDrawing();
        ClearBackground(BLACK); // in case PNG has transparency

        DrawTexture(background, 0, 0, BLACK); // static background

        BeginMode2D(camera);

        for (auto& b : bodies)
        {
            b.Draw();
        }

        camera::UpdateCamera(bodies,camera,cameraTarget);
        EndMode2D();
        DrawText("Use mouse wheel to zoom, arrow keys to move", 10, 10, 20, DARKGRAY);

        EndDrawing();
    }
    UnloadTexture(background);
    return 0;
}