#pragma once

#include "../Tools/RealType.hpp"
#include "Scenario1D.hpp"

namespace Scenarios {

    class SubcriticalFlowScenario: public Scenario1D {
        /**number of cells*/
        const unsigned int size_ = 25;
        RealType bathymetry;
        RealType height;
        RealType momentum;

    public:
        SubcriticalFlowScenario() = default;
        ~SubcriticalFlowScenario() = default;
        /**
         * @return Initial bathymetry at pos
         * */
        RealType getBathymetry(unsigned int pos) const;
        /**
         * @return Initial height at pos
         * */
        RealType getHeight(unsigned int pos) const;
        /**
         * @return Initial momentum at pos
         * */
        RealType getMomentum(unsigned int pos) const;
        /**
         * @return Cell size of one cell (in this scenario it is going to return 1 we have a domain of 25 discretized cells)
         * */
        RealType getCellSize() const;
    };
}