#include "DimensionalSplitting.hpp"
#include <iostream>

Blocks::DimensionalSplitting::DimensionalSplitting(int size_x, int size_y, RealType dx, RealType dy):
        Block(size_x, size_y, dx, dy),
        hNetUpdate_left(size_x + 1, size_y),
        hNetUpdate_right(size_x + 1, size_y),
        hNetUpdate_below(size_x, size_y + 1),
        hNetUpdate_above(size_x, size_y + 1),

        huNetUpdate_left(size_x + 1, size_y),
        huNetUpdate_right(size_x + 1, size_y),

        hvNetUpdate_below(size_x, size_y + 1),
        hvNetUpdate_above(size_x, size_y + 1) {}

void Blocks::DimensionalSplitting::computeNumericalFluxes() {
    RealType maxWaveSpeed = RealType(0.0);

    updateDirection_X(maxWaveSpeed);
    updateDirection_Y(maxWaveSpeed);

    maxTimeStep_ = RealType(0.4) * (dx_ / maxWaveSpeed);
}

void Blocks::DimensionalSplitting::updateDirection_X(RealType& maxWaveSpeed) {
    for (int i = 1; i < nx_ + 2; i++) {
        for (int j = 1; j < ny_ + 1; j++) {
            RealType maxEdgeSpeed = RealType(0.0);

            solver.computeNetUpdates(
                    h_[i - 1][j],
                    h_[i][j],
                    hu_[i - 1][j],
                    hu_[i][j],
                    b_[i - 1][j],
                    b_[i][j],
                    hNetUpdate_left[i - 1][j - 1],
                    hNetUpdate_right[i - 1][j - 1],
                    huNetUpdate_left[i - 1][j - 1],
                    huNetUpdate_right[i - 1][j - 1],
                    maxEdgeSpeed
            );
            maxWaveSpeed = std::max(maxWaveSpeed, maxEdgeSpeed);
        }
    }
}
void Blocks::DimensionalSplitting::updateDirection_Y(RealType& maxWaveSpeed) {
    for (int i = 1; i < nx_ + 1; i++) {
        for (int j = 1; j < ny_ + 2; j++) {
            RealType maxEdgeSpeed = RealType(0.0);

            solver.computeNetUpdates(
                    h_[i][j - 1],
                    h_[i][j],
                    hv_[i][j - 1],
                    hv_[i][j],
                    b_[i][j - 1],
                    b_[i][j],
                    hNetUpdate_below[i - 1][j - 1],
                    hNetUpdate_above[i - 1][j - 1],
                    hvNetUpdate_below[i - 1][j - 1],
                    hvNetUpdate_above[i - 1][j - 1],
                    maxEdgeSpeed
            );
            maxWaveSpeed = std::max(maxWaveSpeed, maxEdgeSpeed);
        }
    }
#ifndef NDEBUG
    if(!(maxTimeStep_<0.5*(dx_/maxWaveSpeed) && maxTimeStep_<0.5*(dy_/maxWaveSpeed))) {
        std::cerr<<"The CFL Condition is not met";
    }
#endif
}

// two directions for momentum
void Blocks::DimensionalSplitting::updateUnknowns(RealType dt) {
    for (int i = 1; i < nx_ + 1; i++) {
        for (int j = 1; j < ny_ + 1; j++) {
            h_[i][j] -= dt / dx_ * (hNetUpdate_left[i][j - 1] + hNetUpdate_right[i - 1][j - 1])
                        + dt / dy_ * (hNetUpdate_above[i - 1][j - 1] + hNetUpdate_below[i - 1][j]);
            hv_[i][j] -= dt / dx_ * (hvNetUpdate_above[i - 1][j - 1] + hvNetUpdate_below[i - 1][j]);
        }
    }

    for (int i = 1; i < nx_ + 1; i++) {
        for (int j = 1; j < ny_ + 1; j++) {
            hu_[i][j] -= dt / dy_ * (huNetUpdate_left[i][j - 1] + huNetUpdate_right[i - 1][j - 1]);
        }
    }
}
