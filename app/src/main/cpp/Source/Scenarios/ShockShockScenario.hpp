/**@file
 * */

#pragma once

#include "../Tools/RealType.hpp"
#include "Scenario1D.hpp"

namespace Scenarios {
    class ShockShockScenario: public Scenario1D {
        const unsigned int size_;
        RealType h_; // heights of the left and the right wave respectively
        RealType hul_; // Momentum of the left wave as we can use its negative to find out the momentum of the right one

    public:
        ShockShockScenario(double h, double hu, unsigned int size);
        ~ShockShockScenario() = default;
        /**
         * @return Cell size of one cell (= domain size/number of cells)
         */
        RealType getCellSize() const override;
        /**
         * @return Height of the left Wave
         */
        RealType getHeight([[maybe_unused]] unsigned int pos) const override;
        /**
         * @return Momentum of the Wave
         */
        RealType getMomentum(unsigned int pos) const override;
}; // namespace Scenarios
}