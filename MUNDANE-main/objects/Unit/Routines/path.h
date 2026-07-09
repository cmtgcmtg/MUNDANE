#include "../../../systems/routine.h"
class Tile;
class Unit;
class Path : public Routine {
    public:
    Unit* unit;
    Path(Unit* unit,std::vector<Tile*> tiles);
    ~Path() override;
    private:
    std::vector<Tile*> tiles;
    float distance;
    float traveled;
    bool increment() override;

    void completion() override;
};