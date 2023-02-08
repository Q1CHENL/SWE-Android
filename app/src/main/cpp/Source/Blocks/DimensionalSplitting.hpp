#include "../Solvers/fwavesolver.hpp"
//#include "FWaveSolver.hpp"
#include "../Tools/RealType.hpp"
#include "Block.hpp"
#include "../Tools/Float2D.hpp"

namespace Blocks{
    
    

    class DimensionalSplitting : public Block

    {
    private:

    //h
    Tools::Float2D<RealType> hNetUpdate_left;
    Tools::Float2D<RealType> hNetUpdate_right;
    Tools::Float2D<RealType> hNetUpdate_below;
    Tools::Float2D<RealType> hNetUpdate_above;

    //x
    Tools::Float2D<RealType> huNetUpdate_left;
    Tools::Float2D<RealType> huNetUpdate_right;

    //y
    Tools::Float2D<RealType> hvNetUpdate_above;
    Tools::Float2D<RealType> hvNetUpdate_below;
    
    //solver
    Solvers::fwavesolver<RealType> solver;
    
    public:
        DimensionalSplitting(int size_x, int size_y, RealType dx, RealType dy);
        ~DimensionalSplitting() override = default;
        /**
         * Compute net updates for the block.
         * The member variable #maxTimestep will be updated with the
         * maximum allowed time step size
         */
        void computeNumericalFluxes() override;

        /**
         * Updates the unknowns with the already computed net-updates.
         *
         * @param dt time step width used in the update.
         */
        void updateUnknowns(RealType dt) override;

        /**
         * Does the x-sweep
         * @param maxWaveSpeed is the maximum wave speed
         * */

        void updateDirection_X(RealType &maxWaveSpeed);

        /**
         * Does the y-sweep
         * @param maxWaveSpeed is the maximum wave speed
         * */
        void updateDirection_Y(RealType &maxWaveSpeed);


    };




    



}