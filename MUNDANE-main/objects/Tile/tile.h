#pragma once

#include <vector>
#include <raylib.h>
#include <raymath.h>


enum class RenderState;

enum class Commandcallback;

class Physical;
class Overlay;
class Interface;

class unit;

class Tile
{
public:
    static std::vector<Tile *> all;

    Tile(Vector3 &vertex, std::vector<Vector3> &corners);
    ~Tile();
    void render();
    bool select(Vector3 &point);

    Vector3 getPosition();
    Matrix getOrientation();
    std::vector<Tile *> getNeighbors();

    Physical *physical;
    Overlay *overlay;
    Interface *interface;
private:
    std::vector<Vector3> corners;
    std::vector<Tile *> neigbhors;
    Vector3 position;
    Matrix orientation;
    bool selected = false;

    void setorientation(Vector3 &vertex);
    void correctorder();
    void connectneighbors();
    Color getColor();
    inline bool iscorrecttileorder()
    {
        for (int i = 0; i < corners.size(); i++)
        {
            Vector3 a = corners[i];
            Vector3 b = corners[(i + 1) % (corners.size())];
            Vector3 c1 = Vector3Transform(Vector3Normalize(a), orientation);
            Vector3 c2 = Vector3Transform(Vector3Normalize(b), orientation);
            float d1 = std::atan2(c1.y, c1.x);
            float d2 = std::atan2(c2.y, c2.x);
            float angle = std::atan2(std::sin(d1 - d2), std::cos(d1 - d2));
            if (angle < 0)
            {
                return false;
            }
        }
        return true;
    }
    inline bool PointInTriangle3D(Vector3 p, Vector3 a, Vector3 b, Vector3 c)
    {
        Vector3 bary = Vector3Barycenter(p, a, b, c);

        return bary.x >= 0 && bary.y >= 0 && bary.z >= 0 &&
               bary.x <= 1 && bary.y <= 1 && bary.z <= 1;
    }
};