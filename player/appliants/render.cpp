#include "render.h"

void Render::renderphysical(){
    for (Physical* physical : Physical::all){
        physical->render();
    }
}
void Render::renderoverlay(){
    for (Overlay* overlay : Overlay::all){
        overlay->render();
    }
}
void Render::renderinterface(){
    for (Interface* interface : Interface::all){
        interface->render();
    }
}