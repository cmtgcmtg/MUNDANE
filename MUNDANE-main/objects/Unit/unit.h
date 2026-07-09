#pragma once

#include <vector>
#include <raylib.h>
#include <raymath.h>

enum class RenderState;

enum class Commandcallback;

class Physical;
class Overlay;
class Interface;

class Path;

class Tile;

class Unit
{
public:
    static std::vector<Unit *> all;

    Tile *tile;
    Unit(Tile *, RenderState);
    //~Unit();
    Commandcallback createPath(std::vector<Tile *>);
    void move(Tile *);

    void setRender(RenderState);

    Vector3 getPosition();
    Tile *getTile();

    Physical *physical;
    Overlay *overlay;
    Interface *interface;

private:
    Path *path;
    Vector3 position;
};