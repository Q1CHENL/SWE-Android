#include <catch2/catch_test_macros.hpp>
#include <iostream>

#include "../Source/Blocks/Block.hpp"
#include "../Source/Scenarios/RadialDamBreakScenario.hpp"
#include "../Source/Tools/RealType.hpp"
#include "../Source/Solvers/fwavesolver.hpp"

TEST_CASE("catch2 test") {
    REQUIRE(1==1);
    std::cout << "catch compiles" << std::endl;
}

TEST_CASE("testing 2D implementation") {
    // hard coding values for ¡RadialDamBreakScenario!
    Scenarios::RadialDamBreakScenario scenario;
    Solvers::fwavesolver<RealType> solver;
    int x = 16;
    int y = 16;
    auto wave_block = Blocks::Block::getBlockInstance(x, y, 1000.0 / x, 1000.0 / y);
    wave_block->initialiseScenario(0, 0, scenario, false);
    wave_block->computeNumericalFluxes();
    RealType timeStep = wave_block->getMaxTimeStep();
    //check for calculated value for maxTimeStep after 1 iteration with default arguments
    REQUIRE(timeStep == 2.0609140284562417);

    x = 20;
    y = 20;
    wave_block = Blocks::Block::getBlockInstance(x, y, 1000.0/x, 1000.0/y);
    wave_block->initialiseScenario(0, 0, scenario, false);
    wave_block->setGhostLayer();
    wave_block->computeNumericalFluxes();
    timeStep = wave_block->getMaxTimeStep();
    REQUIRE(timeStep == 1.6487312227649931);

    x = 40;
    y = 40;
    wave_block = Blocks::Block::getBlockInstance(x, y, 1000.0/x, 1000.0/y);
    wave_block->initialiseScenario(0, 0, scenario, false);
    wave_block->setGhostLayer();
    wave_block->computeNumericalFluxes();
    timeStep = wave_block->getMaxTimeStep();
    REQUIRE(timeStep == 0.82436561138249653);
}