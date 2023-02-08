/**@file*/

#include "ShockShockScenario.hpp"
//#include  <cassert>
Scenarios::ShockShockScenario::ShockShockScenario(double h, double hu, unsigned int size) :
        h_(h), hul_(hu), size_(size) {
    //assert(hul_>0);
}


RealType Scenarios::ShockShockScenario::getCellSize() const { return RealType(1000) / size_; }

RealType Scenarios::ShockShockScenario::getHeight([[maybe_unused]] unsigned int pos) const {return h_;}

RealType Scenarios::ShockShockScenario::getMomentum(unsigned int pos) const{
    if(pos <= size_ / 2){
        return hul_;
    }
    return -hul_;
}