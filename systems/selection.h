
#include <vector>

class Tile;

class Unit;

#include "raylib.h"

enum Selectionstate
{
    NORMAL,
    UNIT,
};
// player will be an object and will have selectedtiles selectedunits
class Selection
{
public:
    void select(Ray ray);

private:
    Selectionstate selectionstate;
    std::vector<Tile *> selectedtiles;
    std::vector<Unit *> selectedunits;
    Selectionstate findselectionstate();
};