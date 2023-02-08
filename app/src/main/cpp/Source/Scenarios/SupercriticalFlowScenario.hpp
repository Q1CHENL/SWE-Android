#pragma once

#include "../Tools/RealType.hpp"
#include "Scenario1D.hpp"

namespace Scenarios {
    class SupercriticalFlowScenario: public Scenario1D{
        /** Number of cells */
        const unsigned int size_ = 25;
        RealType bathymetry;
        RealType height;
        RealType momentum;

    public: 
        SupercriticalFlowScenario() = default;
        ~SupercriticalFlowScenario() = default;
        /**
         * @return Initial bathymetry at pos
         * */
        RealType getBathymetry(unsigned int pos) const override;
        /**
         * @return Initial height at pos
         * */
        RealType getHeight(unsigned int pos) const override;
        /**
         * @return Initial momentum at pos
         * */
        RealType getMomentum(unsigned int pos) const override;
        /**
         * @return Cell size of one cell (in this scenario it is going to return 1 we have a domain of 25 discretized cells)
         * */
        RealType getCellSize() const override;
    };
}