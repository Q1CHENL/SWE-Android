//
// Created by liuqichen on 2/2/23.
//

#ifndef TSNAMISIMMOBILE_SCENARIO1D_HPP
#define TSNAMISIMMOBILE_SCENARIO1D_HPP

#include "../Tools/RealType.hpp"

namespace Scenarios {

    class Scenario1D {
    public:
        /**
         * @return Initial bathymetry at pos
         * */
        virtual RealType getBathymetry(unsigned int pos) const;
        /**
         * @return Initial height at pos
         * */
        virtual RealType getHeight([[maybe_unused]] unsigned int pos) const;
        /**
         * @return Initial momentum at pos
         * */
        virtual RealType getMomentum(unsigned int pos) const;
        /**
         * @return Cell size of one cell (in this scenario it is going to return 1 we have a domain of 25 discretized cells)
         * */
        virtual RealType getCellSize() const;

        //virtual RealType getHeight() const;



    };

} // Scenarios

#endif //TSNAMISIMMOBILE_SCENARIO1D_HPP
