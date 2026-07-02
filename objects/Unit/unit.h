#include <vector>
#include <raylib.h>
#include <raymath.h>

class Path;

class Tile;

class Unit
{

public:
    static std::vector<Unit *> all;
    Tile *tile;
    Unit(Tile *);
    //~Unit();
    void render();
    bool select(Ray);
    void createPath(std::vector<Tile*>);
    void move(Tile*);

    Tile* getTile();
private:
    Path *path;
    Vector3 position;
};