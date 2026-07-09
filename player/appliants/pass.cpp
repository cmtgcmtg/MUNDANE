#include "pass.h"
#include "selection.h"

#include <algorithm>
void Physical::render() {}
Selectcallback Physical::select(Ray ray) {}
void Overlay::render() {}
Selectcallback Overlay::select(Vector2 point) {}
void Interface::render() {}
Selectcallback Interface::select(Vector2 point) {}
Physical::Physical()
{
    Physical::all.push_back(this);
}
Physical::~Physical()
{
    Physical::all.erase(std::find(Physical::all.begin(),Physical::all.end(),this));
}
Overlay::Overlay()
{
    Overlay::all.push_back(this);
}
Overlay::~Overlay()
{
    Overlay::all.erase(std::find(Overlay::all.begin(),Overlay::all.end(),this));
}
Interface::Interface()
{
    Interface::all.push_back(this);
}
Interface::~Interface()
{
    Interface::all.erase(std::find(Interface::all.begin(),Interface::all.end(),this));
}

std::vector<Physical *> Physical::all;

std::vector<Overlay *> Overlay::all;

std::vector<Interface *> Interface::all;

Camera3D* Physical::camera=nullptr;

Camera3D* Overlay::physicalcamera=nullptr;
Camera2D* Overlay::interfacecamera=nullptr;

Camera2D* Interface::camera=nullptr;