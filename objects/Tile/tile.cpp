
#include "tile.h"
#include <algorithm>
std::vector<Tile *> Tile::all;
Tile::Tile(Vector3 &vertex, std::vector<Vector3> &corners)
{
    all.push_back(this);
    neigbhors.reserve(corners.size());
    corners.reserve(corners.size());
    position = Vector3Normalize(vertex);
    setorientation(vertex);
    this->corners = corners;
    correctorder();
    connectneighbors();
}
Tile::~Tile()
{
}
void Tile::render()
{
    for (int i = 0; i < corners.size(); i++)
    {
        Vector3 a = corners[i];
        Vector3 b = corners[(i + 1) % (corners.size())];
        DrawTriangle3D(position, a, b, getColor());
        DrawLine3D(a, b, RED);
    }
}
bool Tile::select(Vector3 &point)
{
    if (Vector3Distance(point, position) < Vector3Distance(position, corners[0]))
    {
        for (int i = 0; i < corners.size(); i++)
        {
            Vector3 a = corners[i];
            Vector3 b = corners[(i + 1) % (corners.size())];
            if (PointInTriangle3D(point, position, a, b))
            {
                selected = !selected;
                return true;
            }
        }
    }
    return false;
}

// getters

Vector3 Tile::getPosition()
{
    return position;
}
Matrix Tile::getOrientation()
{
    return orientation;
}
std::vector<Tile *> Tile::getNeighbors()
{
    return neigbhors;
}
// private
void Tile::setorientation(Vector3 &vertex)
{
    Vector3 N = Vector3Normalize(vertex);
    Vector3 U = Vector3Normalize(Vector3CrossProduct(fabs(N.y) < 0.99f ? Vector3{0, 1, 0} : Vector3{1, 0, 0}, N));
    Vector3 V = Vector3CrossProduct(N, U);
    Matrix M = {
        U.x, U.y, U.z, 0,
        V.x, V.y, V.z, 0,
        N.x, N.y, N.z, 0,
        0, 0, 0, 1};

    orientation = M;
}

void Tile::correctorder()
{
    while (!iscorrecttileorder())
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
                corners[i] = b;
                corners[(i + 1) % (corners.size())] = a;
                break;
            }
        }
    }
}

void Tile::connectneighbors()
{
    for (Tile *othertile : Tile::all)
    {
        if (othertile==this){
            continue;
        }
        for (Vector3 cornerA : corners)
        {
            for (Vector3 cornerB : othertile->corners)
            {
                if (Vector3Distance(cornerA, cornerB) < 0.005f)
                {
                    if (othertile->neigbhors.end()==std::find(othertile->neigbhors.begin(),othertile->neigbhors.end(),this)){
                        othertile->neigbhors.push_back(this);
                    }
                    if (neigbhors.end()==std::find(neigbhors.begin(),neigbhors.end(),othertile)){
                        neigbhors.push_back(othertile);
                    }
                    break;
                }
            }
        }
    }
}
Color Tile::getColor()
{
     if (corners.size() == 5)
    {
        return GREEN;
    }
    else if (selected)
    {
        return WHITE;
    }
    else
    {
        return BLUE;
    }
}
