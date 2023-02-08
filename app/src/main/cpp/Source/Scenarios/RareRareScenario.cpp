/**@file*/

#include "RareRareScenario.hpp"
#include  <cassert>
Scenarios::RareRareScenario::RareRareScenario(double h, double hu, unsigned int size) :
        h_(h), hul_(hu), size_(size) {
    assert(hul_<0);
}

RealType Scenarios::RareRareScenario::getCellSize() const { return RealType(1000) / size_; }

RealType Scenarios::RareRareScenario::getHeight([[maybe_unused]] unsigned int pos) const {return h_;}


//simplify to one method, to match call in Main
RealType Scenarios::RareRareScenario::getMomentum(unsigned int pos) const {
    if(pos <= size_ / 2){
        return hul_;
    }
    return -hul_;
}