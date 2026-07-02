
#include <iostream>
#include "routine.h"
bool Routine::increment(){
    return false;
}
void Routine::completion(){

}
Routine::Routine(Routinepath routinepath){
    std::cout<<"ROUTINE ROUTINE"<<std::endl;
    running=true;
    all[routinepath].push_back(this);
}
void Routine::run(Routinepath routinepath){
    for (int i=0; i<all[routinepath].size(); i++){
        Routine* routine=all[routinepath][i];
        std::cout<<routine<<std::endl;
        if (!routine->running){
            continue;
        }
        if (routine->increment()){
            routine->completion();
            delete routine;
            all[routinepath].erase(all[routinepath].begin()+i);
        }
    }
}

std::map<Routinepath, std::vector<Routine*>> Routine::all;