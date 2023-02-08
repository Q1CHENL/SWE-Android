#include "SupercriticalFlowScenario.hpp"
#include <cmath>

RealType Scenarios::SupercriticalFlowScenario::getCellSize() const {return RealType(25)/size_;}

RealType Scenarios::SupercriticalFlowScenario::getBathymetry(unsigned int pos) const {
    if (pos>8 && pos<12) {
        return -0.13-0.05*std::pow((((double)pos)-10), 2);
    }
    return -0.33;
}

RealType Scenarios::SupercriticalFlowScenario::getHeight(unsigned int pos) const {return -getBathymetry(pos);}

RealType Scenarios::SupercriticalFlowScenario::getMomentum(unsigned int pos) const {return 0.18;}
