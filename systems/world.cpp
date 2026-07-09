
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <stack>
#include <queue>
#include <array>

#include "../objects/Tile/tile.h"
#include "../objects/Unit/unit.h"

#include "world.h"

World::World(int size)
{
    this->size = size;
    this->vertices = {
        {-0.262865f, 0.425325f, 0.000000f},  // 0
        {0.262865f, 0.425325f, 0.000000f},   // 1
        {-0.262865f, -0.425325f, 0.000000f}, // 2
        {0.262865f, -0.425325f, 0.000000f},  // 3

        {0.000000f, -0.262865f, 0.425325f},  // 4
        {0.000000f, 0.262865f, 0.425325f},   // 5
        {0.000000f, -0.262865f, -0.425325f}, // 6
        {0.000000f, 0.262865f, -0.425325f},  // 7

        {0.425325f, 0.000000f, -0.262865f},  // 8
        {0.425325f, 0.000000f, 0.262865f},   // 9
        {-0.425325f, 0.000000f, -0.262865f}, // 10
        {-0.425325f, 0.000000f, 0.262865f}   // 11
    };
    this->indices = {
        {0, 11, 5},
        {0, 5, 1},
        {0, 1, 7},
        {0, 7, 10},
        {0, 10, 11},

        {1, 5, 9},
        {5, 11, 4},
        {11, 10, 2},
        {10, 7, 6},
        {7, 1, 8},

        {3, 9, 4},
        {3, 4, 2},
        {3, 2, 6},
        {3, 6, 8},
        {3, 8, 9},

        {4, 9, 5},
        {2, 4, 11},
        {6, 2, 10},
        {8, 6, 7},
        {9, 8, 1}};
    int verticereserved = ((20 * std::pow(4, size)) - 8);
    int indicereserved = (20 * std::pow(4, size));
    vertices.reserve(verticereserved);
    indices.reserve(indicereserved);
    for (int i = 0; i < size; i++)
    {
        subdivide();
        compress();
    }
    smooth();
    layouttile();
}
void World::subdivide()
{
    std::vector<std::array<int, 3>> newindices;
    for (std::array<int, 3> face : indices)
    {
        std::array<int, 3> newface1;
        std::array<int, 3> newface2;
        std::array<int, 3> newface3;
        std::array<int, 3> newface4;
        Vector3 a = vertices[face[0]];
        Vector3 b = vertices[face[1]];
        Vector3 c = vertices[face[2]];
        vertices[face[0]] = (a);
        vertices[face[1]] = (b);
        vertices[face[2]] = (c);
        Vector3 c1;
        c1 = Vector3Add(Vector3Scale(a, 0.5f), Vector3Scale(b, 0.5f));
        int ci1 = vertices.size();

        vertices.push_back(c1);

        Vector3 c2;
        c2 = Vector3Add(Vector3Scale(b, 0.5f), Vector3Scale(c, 0.5f));
        int ci2 = vertices.size();

        vertices.push_back(c2);

        Vector3 c3;
        c3 = Vector3Add(Vector3Scale(c, 0.5f), Vector3Scale(a, 0.5f));
        int ci3 = vertices.size();

        vertices.push_back(c3);

        newface1[0] = face[2];
        newface1[1] = ci3;
        newface1[2] = ci2;

        newface2[0] = ci3;
        newface2[1] = face[0];
        newface2[2] = ci1;

        newface3[0] = ci2;
        newface3[1] = ci1;
        newface3[2] = face[1];

        newface4[0] = ci2;
        newface4[1] = ci3;
        newface4[2] = ci1;

        newindices.push_back(newface1);
        newindices.push_back(newface2);
        newindices.push_back(newface3);
        newindices.push_back(newface4);
    }
    indices = newindices;
}
void World::compress()
{
    std::vector<Vector3> newvertices;
    newvertices.reserve(vertices.size());
    for (unsigned long long int i = 0; i < vertices.size(); i++)
    {
        bool with = false;
        for (std::array<int, 3> &indice : indices)
        {
            if (i == indice[0])
            {
                indice[0] = newvertices.size();
                with = true;
                continue;
            }
            if (i == indice[1])
            {
                indice[1] = newvertices.size();
                with = true;
                continue;
            }
            if (i == indice[2])
            {
                indice[2] = newvertices.size();
                with = true;
                continue;
            }
        }
        for (unsigned long long int j = i + 1; j < vertices.size(); j++)
        {
            if (Vector3Distance(vertices[i], vertices[j]) < 0.01f)
            {
                for (std::array<int, 3> &indice : indices)
                {
                    if (j == indice[0])
                    {
                        indice[0] = newvertices.size();
                        with = true;
                        continue;
                    }
                    if (j == indice[1])
                    {
                        indice[1] = newvertices.size();
                        with = true;
                        continue;
                    }
                    if (j == indice[2])
                    {
                        indice[2] = newvertices.size();
                        with = true;
                        continue;
                    }
                }
            }
        }
        if (with)
        {
            newvertices.push_back(vertices[i]);
        }
    }
    vertices.clear();
    vertices = newvertices;
}
void World::smooth()
{
    for (int i = 0; i < vertices.size(); i++)
    {
        vertices[i] = Vector3Normalize(vertices[i]);
    }
}
void World::layouttile()
{
    std::map<int, Vector3> centerface;
    for (int i = 0; i < indices.size(); i++)
    {
        centerface[i] = Vector3Zero();
        for (int vertexindex : indices[i])
        {
            centerface[i] = Vector3Add(centerface[i], vertices[vertexindex]);
        }
        centerface[i] = Vector3Scale(centerface[i], 1.0f / 3.0f);
    }
    for (int mainvertexindex = 0; mainvertexindex < vertices.size(); mainvertexindex++)
    {
        Vector3 vertex = vertices[mainvertexindex];
        std::vector<Vector3> corners;
        corners.reserve(6);
        for (int i = 0; i < indices.size(); i++)
        {
            for (int vertexindex : indices[i])
            {
                if (mainvertexindex == vertexindex)
                {
                    corners.push_back(centerface[i]);
                }
            }
        }
        Tile *newtile = new Tile(vertex, corners);
    }
}