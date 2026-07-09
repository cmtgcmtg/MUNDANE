#pragma once


#include "raylib.h"
#include <vector>
enum class RenderState
{
    SELF
};
enum class Selectcallback
{
    NONE,
    SELECT,
};
class Player;

struct Selectedobjects;

class Physical
{
public:
    static std::vector<Physical *> all;
    static Camera3D *camera;
    Physical();
    virtual ~Physical();
    virtual void render();
    virtual Selectcallback select(Ray ray);
private:
    std::vector<Physical*>::iterator index;
};
// designates ui where there location is based off of world
class Overlay
{
public:
    static std::vector<Overlay *> all;
    static Camera3D *physicalcamera;
    static Camera2D *interfacecamera;
    Overlay();
    virtual ~Overlay();
    virtual void render();
    virtual Selectcallback select(Vector2 point);
private:
    std::vector<Overlay*>::iterator index;
};
// interface 2D where location isnt determined
class Interface
{
public:
    static std::vector<Interface *> all;
    static Camera2D *camera;
    Interface();
    virtual ~Interface();
    virtual void render();
    virtual Selectcallback select(Vector2 point);
private:
    std::vector<Interface*>::iterator index;
};

// object are composed of little objects that do composition
