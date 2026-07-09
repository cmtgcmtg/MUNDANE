#include <cmath>



#include <raylib.h>
#include <raymath.h>

class World
{
public:
    int size;
    World(int size);
private:
    std::vector<Vector3> vertices;
    std::vector<std::array<int, 3>> indices;
    void subdivide();
    void compress();
    void smooth();
    void layouttile();
};
