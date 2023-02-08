//
// Created by liuqichen on 2/2/23.
//

#include "Scenario1D.hpp"
#include "../Tools/RealType.hpp"


    RealType Scenarios::Scenario1D::getBathymetry(unsigned int pos) const {
        return RealType(0.0);;
    }


    RealType Scenarios::Scenario1D::getMomentum(unsigned int pos) const {
        return RealType(0.0);;
    }

    RealType Scenarios::Scenario1D::getCellSize() const {
        return RealType(0.0);;
    }


    RealType Scenarios::Scenario1D::getHeight([[maybe_unused]]unsigned int pos) const {
    return 0;
}

