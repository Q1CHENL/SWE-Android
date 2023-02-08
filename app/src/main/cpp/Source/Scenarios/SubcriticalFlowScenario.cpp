#include "SubcriticalFlowScenario.hpp"
#include <cmath>
#include <cassert>
#include <iostream>
RealType Scenarios::SubcriticalFlowScenario::getCellSize() const {return RealType(25)/size_;}

RealType Scenarios::SubcriticalFlowScenario::getBathymetry(unsigned int pos) const {
    if (pos>8 && pos<12) {
        return -1.8 - 0.05*std::pow((((double)pos)-10),2);
    }
    return -2;
}


RealType Scenarios::SubcriticalFlowScenario::getHeight(unsigned int pos) const {
    //assert(pos > 0 && pos < 25);
    return -getBathymetry(pos);
    }

RealType Scenarios::SubcriticalFlowScenario::getMomentum(unsigned int pos) const {
    //assert(pos > 0 && pos < 25); //assert causes app crash
    return 4.42;
    }