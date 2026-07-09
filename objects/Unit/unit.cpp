

#include "unit.h"
#include "Routines/path.h"
#include "Passes/selfPhysical.h"
#include "Passes/selfOverlay.h"

#include "../Tile/tile.h"

#include <iostream>
#include <algorithm>

#include "raylib.h"
#include "raymath.h"

#include "../../player/appliants/selection.h"

selfPhysical::selfPhysical(Unit* unit) : Physical(){
    this->unit=unit;
}
selfPhysical::~selfPhysical(){
    unit->physical=nullptr;
}
selfOverlay::selfOverlay(Unit* unit) : Overlay(){
    this->unit=unit;
}
selfOverlay::~selfOverlay(){
    unit->overlay=nullptr;
}

std::vector<Unit *> Unit::all;

Unit::Unit(Tile *tile,RenderState renderState)
{
    all.push_back(this);
    this->tile = tile;
    this->position = Vector3Scale(tile->getPosition(), 1.005f);
    this->path=nullptr;
    switch (renderState){
        case RenderState::SELF: {
            physical=new selfPhysical(this);
            overlay = new selfOverlay(this);
            interface = nullptr;
        }
    }
}

void Unit::setRender(RenderState RenderState)
{

}

void selfPhysical::render(){
    DrawSphere(unit->getPosition(),0.01f,RED);
}

void selfOverlay::render(){
    Vector2 point=Vector2Subtract(GetWorldToScreen(Vector3Scale(unit->getPosition(),1.05f),*Overlay::physicalcamera),{10.0f,15.0f});

    float scale=tan(Overlay::physicalcamera->fovy)*(1.0f/Vector3Distance(Overlay::physicalcamera->position,unit->getPosition()));
    DrawRectangleV(point,{20.0f*scale,30.0f*scale},GREEN);
    std::cout<<"I am selfOverlay"<<scale<<'\n';
}
Selectcallback selfPhysical::select(Ray ray){
    if (GetRayCollisionSphere(ray,unit->getPosition(),0.01f).hit){
        return Selectcallback::SELECT;
    } else {
        return Selectcallback::NONE;
    }
}
Selectcallback selfOverlay::select(Vector2 click){
    Vector2 point=GetWorldToScreen(unit->getPosition(),*Overlay::physicalcamera);
    float scale=tan(Overlay::physicalcamera->fovy)*Vector3Distance(Overlay::physicalcamera->position,unit->getPosition());
    Rectangle rectangle;
    rectangle.width=20.0f*scale;
    rectangle.height=30.0f*scale;
    rectangle.x=point.x;
    rectangle.y=point.y;
    if (CheckCollisionPointRec(click,rectangle)){
        return Selectcallback::SELECT;
    } else {
        return Selectcallback::NONE;
    }
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
        float nextangle = Vector3Angle(Vector3Normalize(Vector3Subtract(neighbortile->getPosition(),tile->getPosition())), direction);
        std::cout<<nextangle<<std::endl;
        if (nextangle==0){
            return neighbortile;
        }
        if (previousangle > nextangle)
        {
            std::cout<<"neighbortile"<<neighbortile<<std::endl;
            previousangle = nextangle;
            nexttile = neighbortile;
        }
    }
    return nexttile;
}
Commandcallback Unit::createPath(std::vector<Tile *> tiles)
{
    std::vector<Tile *> pathtiles;
    pathtiles.reserve(30);
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
            if (pathtiles.size()>30){
                return Commandcallback::UNITIMPOSSIBLEREACH;
            }
        }
    }
    path = new Path(this, pathtiles);
    return Commandcallback::UNITPOSSIBLEREACH;
}
//getters
Tile* Unit::getTile(){
    return tile;
}
Vector3 Unit::getPosition(){
    return position;
}
Path::Path(Unit *unit, std::vector<Tile *> tiles): Routine(delay10)
{
    this->unit = unit;
    this->tiles = tiles;
    traveled=0;
    distance=1.0f;
}
Path::~Path(){
    std::cout<<"destroyed path from:"<<unit<<'\n';
}
bool Path::increment()
{
    std::cout<<"traveled:"<<traveled<<'\n';
    if (traveled > distance)
    {
        distance = 1.0f;
        std::cout<<"distance:"<<distance<<'\n';
        unit->move(tiles[0]);
        tiles.erase(tiles.begin());
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