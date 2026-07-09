

#include "../../objects/Tile/tile.h"
#include "../../objects/Unit/unit.h"

#include "pass.h"

#include "selection.h"

#include "pass.h"

#include <iostream>

void Selection::selectphysical(Ray ray)
{
    RayCollision rc= GetRayCollisionSphere(ray,{0, 0, 0}, 1);
    if (!rc.hit){
        return;
    }
    Vector3 point=rc.point;
    switch (findselectionstate())
    {
    case Selectionstate::NORMAL:
    {
        Unit *thisunit=nullptr;
        for (Unit *unit : Unit::all)
        {
            if (unit->physical->select(ray)==Selectcallback::SELECT)
            {
                thisunit = unit;
            }
        }
        std::cout<<thisunit<<'\n';
        if (thisunit != nullptr)
        {
            selectedunits.push_back(thisunit);
            return;
        }
        Tile *thistile=nullptr;
        for (Tile *tile : Tile::all)
        {
            if (tile->select(point))
            {
                thistile = tile;
            }
        }
        if (thistile != nullptr)
        {
            selectedtiles.push_back(thistile);
            return;
        }
        selectedtiles.clear();
        selectedunits.clear();
    }
    case Selectionstate::UNIT:
    {
        Tile *thistile=nullptr;
        for (Tile *tile : Tile::all)
        {
            if (tile->select(point))
            {
                thistile = tile;
            }
        }
        if (thistile == nullptr)
        {
            return;
        } else {
            selectedtiles.push_back(thistile);
        }
        for (Unit *unit : selectedunits)
        {
            unit->createPath(selectedtiles);
        }
        selectedtiles.clear();
        selectedunits.clear();
    }
    }
}
Selectionstate Selection::findselectionstate()
{
    if (selectedtiles.size() == 0 && selectedunits.size() == 0)
    {
        return Selectionstate::NORMAL;
    }
    else if (selectedtiles.size() != 0 && selectedunits.size() == 0)
    {
        return Selectionstate::NORMAL;
    }
    else if (selectedtiles.size() == 0 && selectedunits.size() != 0)
    {
        return Selectionstate::UNIT;
    }
    else if (selectedtiles.size() != 0 && selectedunits.size() != 0)
    {
        return Selectionstate::UNIT;
    }
    return Selectionstate::NORMAL;
}

Selectedobjects Selection::getselectedobjects(){
    Selectedobjects selectedobjects;
    selectedobjects.tiles=selectedtiles;
    selectedobjects.units=selectedunits;
}