#include <vector>

class Tile;

class Unit;

#include "raylib.h"

enum class Selectionstate
{
    NORMAL,
    UNIT,
};

enum class Commandcallback
{
    UNITIMPOSSIBLEREACH,
    UNITPOSSIBLEREACH
};

struct Selectedobjects {
    std::vector<Tile *> tiles;
    std::vector<Unit *> units;
};
// player will be an object and will have selectedtiles selectedunitss
class Selection
{
public:
    void selectphysical(Ray ray);
    Selectedobjects getselectedobjects();
private:
    Selectionstate selectionstate;
    std::vector<Tile *> selectedtiles;
    std::vector<Unit *> selectedunits;
    Selectionstate findselectionstate();
};