//mainline systems

#include <iostream>
#include "systems/routine.h"
#include "systems/selection.h"

#include <thread>

#include "objects/Tile/tile.h"
#include "objects/Unit/unit.h"

#include <GL/gl.h>

#include "systems/world.h"

void runtime() {
    while(true){
        WaitTime(0.01);
        Routine::run(delay10);
        
    }
}


int main()
{
    Selection *selectionsystem = new Selection();
    World *worldsystem = new World(4);
    new Unit(Tile::all[0]);
    InitWindow(1920, 1080, "asss");
    SetTargetFPS(60);
    Camera3D camera = {0};
    camera.position = {0, 0, -5};
    camera.target = {0, 0, 0};
    camera.up = {0, 1, 0};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    float S=1;
    Vector2 D;
    std::thread t(runtime);
    t.detach();
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(GRAY);
        DrawSphere({0, 0, 0}, 1, WHITE);
        BeginMode3D(camera);
        glDisable(GL_CULL_FACE);
        for (Tile *tile : Tile::all)
        {
            tile->render();
        }
        for (Unit *unit : Unit::all)
        {
            unit->render();
        }
        EndMode3D();
        EndDrawing();
        S += GetMouseWheelMove();
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            D.x = D.x + GetMouseDelta().x / 100.0f;
            D.y = Clamp(D.y + GetMouseDelta().y / 100.0f, -1.5707, 1.5707);
            camera.position = Vector3Transform({0, 0, S}, MatrixMultiply(MatrixRotateX(D.y), MatrixRotateY(D.x)));
        }
        else
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                selectionsystem->select(GetScreenToWorldRay(GetMousePosition(), camera));
                
            }
        }
    }
    CloseWindow();
    return 1;
}