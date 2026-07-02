

#include "unit.h"
#include "Routines/path.h"
#include "../Tile/tile.h"

#include <iostream>

#include "raylib.h"
#include "raymath.h"
std::vector<Unit *> Unit::all;

Unit::Unit(Tile *tile)
{
    all.push_back(this);
    this->tile = tile;
    this->position = Vector3Scale(tile->getPosition(), 1.005f);
}
void Unit::render(){
    DrawSphere(position,0.01f,RED);
}
bool Unit::select(Ray ray){
    return GetRayCollisionSphere(ray,position,.1f).hit;
}
void Unit::move(Tile *tile)
{
    this->tile = tile;
    this->position = Vector3Scale(tile->getPosition(), 1.005f);
}
Tile *recurvenexttile(Tile *tile, const Vector3 &direction)
{
    float previousangle = 900;
    Tile *nexttile=nullptr;
    for (Tile *neighbortile : tile->getNeighbors())
    {
        std::cout<<tile->getNeighbors().size()<<std::endl;
        float nextangle = Vector3Angle(Vector3Normalize(Vector3Subtract(neighbortile->getPosition(),tile->getPosition())), direction);
        std::cout<<nextangle<<std::endl;
        if (previousangle > nextangle)
        {
            std::cout<<"neighbortile"<<neighbortile<<std::endl;
            previousangle = nextangle;
            nexttile = neighbortile;
        }
    }
    return nexttile;
}
void Unit::createPath(std::vector<Tile *> tiles)
{
    std::vector<Tile *> pathtiles;
    pathtiles.reserve(300);
    for (int i = 0; i < tiles.size(); i++)
    {
        Tile *endtile = tiles[i];

        Tile *starttile;
        if (i != 0)
        {
            starttile = tiles[(i - 1) % (tiles.size())];
        }
        else
        {
            starttile = tile;
        }
        std::cout<<i<<" "<<starttile<<'\n';
        std::cout<<i<<" "<<endtile<<'\n';
        Vector3 direction = Vector3Normalize(Vector3Subtract(endtile->getPosition(), starttile->getPosition()));
        Tile* currenttile=recurvenexttile(starttile, direction);
        pathtiles.push_back(currenttile);
        while (currenttile != endtile)
        {   
            std::cout<<currenttile<<'\n';
            direction = Vector3Normalize(Vector3Subtract(endtile->getPosition(), currenttile->getPosition()));
            currenttile=recurvenexttile(currenttile, direction);
            pathtiles.push_back(currenttile);
            if (pathtiles.size()>300){
                break;
            }
        }
    }
    if (path!=nullptr){
        delete path;
    }
    path = new Path(this, pathtiles);
}
//getters
Tile* Unit::getTile(){
    return tile;
}
Path::Path(Unit *unit, std::vector<Tile *> tiles): Routine(delay10)
{
    this->unit = unit;
    this->tiles = tiles;
    traveled=0;
    distance=1.0f;
}
bool Path::increment()
{
    std::cout<<"traveled:"<<traveled<<'\n';
    if (traveled > distance)
    {
        distance = 1.0f;
        std::cout<<"distance:"<<distance<<'\n';
        tiles.erase(tiles.begin());
        unit->move(tiles[0]);
        traveled=0;
        std::cout<<"reached1\n";
        if (tiles.size() == 0)
        {
            return true;
        }
        else
        {
            std::cout<<"reached\n";
            return false;
        }
    }
    traveled+=.5f;
    return false;
}
void Path::completion()
{
}