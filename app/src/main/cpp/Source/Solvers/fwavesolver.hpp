/**
 * @file
 * \brief
 * This is our own F-Wave Solver
 * main method "computeNetUpdates()" is made compatible
 * with the corresponding function call in WavePropagationBlock.cpp
 * */
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <omp.h>

#define WETDRY 0      /// macro for wet-dry boundary
#define DRYWET 1      /// macro for dry-wet boundary
#define WETWET 2      /// macro for wet-wet boundary

namespace Solvers {

    template<class T>
    class fwavesolver {
    private:
        T gravity_ = static_cast<T>(9.81);
        T dryBoundary = static_cast<T>(0.0);
        T hLeft_;                        //! Height on the left side of the edge (could change during execution).
        T hRight_;                       //! Height on the right side of the edge (could change during execution).
        T huLeft_;                       //! Momentum on the left side of the edge (could change during execution).
        T huRight_;                      //! Momentum on the right side of the edge (could change during execution).
        T bLeft_ = static_cast<T>(0.0); //! Bathymetry on the left side of the edge (could change during execution).
        T bRight_ = static_cast<T>(0.0); //! Bathymetry on the right side of the edge (could change during execution).
        T uLeft_;                        //! Velocity on the left side of the edge (computed by setCellState).
        T uRight_;                       //! Velocity on the right side of the edge (computed by setCellState).

        int wetdrystate; //! wet/dry state of our Riemann-problem (determined by setCellState).

    public:
        fwavesolver() = default;

        ~fwavesolver() = default;

        /** Computes the net updates of the left and right traveling waves based on two adjacent cells
         *
         * The net updates of the traveling waves are determined by the Roe wavespeeds mulitpled by the alphas.
         * UPDATE: now also takes the bathymetry into consideration by recalculating the flux function.
         *
         * @params hLeft,huLeft height and momentum of the left cell.
         * @params hRight,huRight height and momentum of the right cell.
         * @params bLeft bathymetry of the left cell.
         * @params bRight, bathymetry of the right cell.
         * @params o_hUpdateLeft,o_hUpdateRight,o_huUpdateLeft,o_huUpdateRight,o_maxWaveSpeed will be set to with the return values
         * @returns height and momentum net updates of the waves
         * @returns the maximum wave speed calculated
         */
        void computeNetUpdates(
                const T &hLeft,
                const T &hRight,
                const T &huLeft,
                const T &huRight,
                const T &bLeft,
                const T &bRight,
                T &o_hUpdateLeft,
                T &o_hUpdateRight,
                T &o_huUpdateLeft,
                T &o_huUpdateRight,
                T &o_maxWaveSpeed
        ) {

            o_hUpdateLeft = o_hUpdateRight = o_huUpdateLeft = o_huUpdateRight = T(0.0);

            assign_parameters(hLeft, hRight, huLeft, huRight, bLeft, bRight, huLeft / hLeft,
                              huRight / hRight);
            setCellState();

            T roeWaveSpeeds[2]{0, 0};
            T fluxFuncDiff[2]{0, 0};
            T roeEigenMatrix[2][2]{0, 0, 0, 0};
            T alphas[2]{0, 0};
            T waves[2][2]{0, 0, 0, 0};
            T inversed[2][2]{
                    0,
                    0,
                    0,
                    0,
            };
            T z1[2]{0, 0};
            T z2[2]{0, 0};
            T deltaPsi[2]{0, 0};

            computeDeltaPsi(deltaPsi);
            setRoeWaveSpeeds(roeWaveSpeeds);
            setRoeEigenMatrix(roeEigenMatrix, roeWaveSpeeds);
            inverseMatrix(roeEigenMatrix, inversed);
            setfluxFunctionDiff(fluxFuncDiff);
            computeAlphas(alphas, fluxFuncDiff, roeEigenMatrix, inversed); // reduce param
            computeWaves(alphas, roeWaveSpeeds, waves);

            // set z-vectors
            z1[0] = alphas[0];
            z1[1] = alphas[0] * roeWaveSpeeds[0];
            z2[0] = alphas[1];
            z2[1] = alphas[1] * roeWaveSpeeds[1];

            /// recompute the new flux function with bathymetry and decomposition
            setFluxFuncDiffBathymetry(fluxFuncDiff, deltaPsi);
            computeAlphas(alphas, fluxFuncDiff, roeEigenMatrix, inversed);
            computeWaves(alphas, roeWaveSpeeds, waves);

            z1[0] = alphas[0];
            z1[1] = alphas[0] * roeWaveSpeeds[0];
            z2[0] = alphas[1];
            z2[1] = alphas[1] * roeWaveSpeeds[1];

            if (roeWaveSpeeds[0] * roeWaveSpeeds[1] <= 0) {
                o_hUpdateLeft += z1[0];
                o_huUpdateLeft += z1[1];
                o_hUpdateRight += z2[0];
                o_huUpdateRight += z2[1];
            } else {
                T x = roeWaveSpeeds[0] * alphas[0] + roeWaveSpeeds[1] * alphas[1];
                if (roeWaveSpeeds[0] < 0) { // both waves going left
                    o_hUpdateLeft += z1[0] + z2[0];
                    o_huUpdateLeft += z1[1] + z2[1];
                } else { // both going right
                    o_hUpdateRight += z1[0] + z2[0];
                    o_huUpdateRight += z1[1] + z2[1];
                }
            }
            // Compute maximum wave speed (-> CFL-condition)
            o_maxWaveSpeed = std::max(std::fabs(roeWaveSpeeds[0]), std::fabs(roeWaveSpeeds[1]));

            // Zero ghost updates (wall boundary)
            if (wetdrystate == WETDRY) {
                o_hUpdateRight = 0;
                o_huUpdateRight = 0;
            } else if (wetdrystate == DRYWET) {
                o_hUpdateLeft = 0;
                o_huUpdateLeft = 0;
            }
        }

        // assign arguments except for gravity
        void assign_parameters(
                const T &hLeft,
                const T &hRight,
                const T &huLeft,
                const T &huRight,
                const T &bLeft,
                const T &bRight,
                const T &uLeft,
                const T &uRight
        ) {
            hLeft_ = hLeft;
            hRight_ = hRight;
            huLeft_ = huLeft;
            huRight_ = huRight;
            bLeft_ = bLeft;
            bRight_ = bRight;
            uLeft_ = uLeft;
            uRight_ = uRight;
        }

        T roeHight() { return (hLeft_ + hRight_) / 2; }

        T roeSpeed() {
            T sqrt_hl = std::sqrt(hLeft_);
            T sqrt_hr = std::sqrt(hRight_);
            return (uLeft_ * sqrt_hl + uRight_ * sqrt_hr) / (sqrt_hl + sqrt_hr);
        }

        void setRoeWaveSpeeds(T (&roeWaveSpeeds)[2]) {
            roeWaveSpeeds[0] = roeSpeed() - std::sqrt(gravity_ * roeHight());
            roeWaveSpeeds[1] = roeSpeed() + std::sqrt(gravity_ * roeHight());
        }

        void setRoeEigenMatrix(T (&roeEigenMatrix)[2][2], const T (&roeWaveSpeeds)[2]) {
            roeEigenMatrix[0][0] = 1;
            roeEigenMatrix[0][1] = roeWaveSpeeds[0];
            roeEigenMatrix[1][0] = 1;
            roeEigenMatrix[1][1] = roeWaveSpeeds[1];
        }

        void inverseMatrix(const T (&roeEigenMatrix)[2][2], T inversed[2][2]) {
            T a = roeEigenMatrix[0][0];
            T b = roeEigenMatrix[1][0];
            T c = roeEigenMatrix[0][1];
            T d = roeEigenMatrix[1][1];

            T coeff = 1 / (a * d - b * c);
            inversed[0][0] = d * coeff;
            inversed[0][1] = -c * coeff;
            inversed[1][0] = -b * coeff;
            inversed[1][1] = a * coeff;
        }

        void setfluxFunctionDiff(T (&fluxFuncDiff)[2]) {
            T fluxFuncR[2]{huRight_, huRight_ * uRight_ +
                                     static_cast<T>(0.5) * gravity_ * std::pow(hRight_, 2)};
            T fluxFuncL[2]{huLeft_,
                           huLeft_ * uLeft_ + static_cast<T>(0.5) * gravity_ * std::pow(hLeft_, 2)};
            fluxFuncDiff[0] = fluxFuncR[0] - fluxFuncL[0];
            fluxFuncDiff[1] = fluxFuncR[1] - fluxFuncL[1];
        }

        void computeAlphas(
                T (&alphas)[2], const T (&fluxFuncDiff)[2], const T (&roeEigenMatrix)[2][2],
                const T (&inversed)[2][2]
        ) {
            alphas[0] = inversed[0][0] * fluxFuncDiff[0] + inversed[1][0] * fluxFuncDiff[1];
            alphas[1] = inversed[0][1] * fluxFuncDiff[0] + inversed[1][1] * fluxFuncDiff[1];
        }

        void computeWaves(const T (&alphas)[2], const T (&roeWaveSpeeds)[2], T (&waves)[2][2]) {
            waves[0][0] = alphas[0];
            waves[0][1] = alphas[0] * roeWaveSpeeds[0];
            waves[1][0] = alphas[1];
            waves[1][1] = alphas[1] * roeWaveSpeeds[1];
        }

        // for bathymetry
        void computeDeltaPsi(T (&deltaPsi)[2]) {
            deltaPsi[0] = 0;
            deltaPsi[1] = -gravity_ * (bRight_ - bLeft_) * ((hLeft_ + hRight_) / 2);
        }

        // 2.version
        void setFluxFuncDiffBathymetry(T (&fluxFuncDiff)[2], const T (&deltaPsi)[2]) {
            fluxFuncDiff[0] -= deltaPsi[0];
            fluxFuncDiff[1] -= deltaPsi[1];
        }

        void setCellState() {
            if (hLeft_ < dryBoundary) { // Left cell dry, right cell wet
                // Set wall boundary conditions.
                // This is not correct in the case of inundation problems.
                uRight_ = huRight_ / hRight_;

                hLeft_ = hRight_;
                bLeft_ = bRight_;
                huLeft_ = -huRight_;
                uLeft_ = -uRight_;
                wetdrystate = DRYWET;
            } else if (hRight_ < dryBoundary) { // Left cell wet, right cell dry
                // Set wall boundary conditions.
                // This is not correct in the case of inundation problems.
                uLeft_ = huLeft_ / hLeft_;

                hRight_ = hLeft_;
                bRight_ = bLeft_;
                huRight_ = -huLeft_;
                uLeft_ = -uRight_;
                wetdrystate = WETDRY;
            } else {// Both left and right cell wet
                uLeft_ = huLeft_ / hLeft_;
                uRight_ = huRight_ / hRight_;
                wetdrystate = WETWET;
            }
        }
    };

} // namespace Solvers