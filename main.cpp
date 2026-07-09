// mainline systems

#include <iostream>
#include "systems/routine.h"
#include "player/appliants/selection.h"
#include "player/appliants/render.h"

#include <thread>

#include "objects/Tile/tile.h"
#include "objects/Unit/unit.h"

#include <GL/gl.h>

#include "systems/world.h"

void runtime()
{
    while (true)
    {
        WaitTime(0.01);
        Routine::run(delay10);
    }
}

int main()
{
    Selection *selectionsystem = new Selection();
    Render *rendersystem = new Render();
    World *worldsystem = new World(4);
    new Unit(Tile::all[0], RenderState::SELF);
    InitWindow(1920, 1080, "asss");
    SetTargetFPS(60);
    Camera3D camera = {0};
    camera.position = {0, 0, -5};
    camera.target = {0, 0, 0};
    camera.up = {0, 1, 0};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    float S = 1;
    Vector2 D;
    Camera2D interfacecamera = {0};
    interfacecamera.offset = {0,0};
    interfacecamera.zoom = 1.0f;
    interfacecamera.rotation = 0.0f;
    Physical::camera = &camera;
    Overlay::physicalcamera = &camera;
    Overlay::interfacecamera = &interfacecamera;
    Interface::camera = &interfacecamera;
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
        rendersystem->renderphysical();
        EndMode3D();
        BeginMode2D(interfacecamera);
        rendersystem->renderoverlay();
        EndMode2D();
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
                selectionsystem->selectphysical(GetScreenToWorldRay(GetMousePosition(), camera));
            }
        }
    }
    CloseWindow();
    return 1;
}