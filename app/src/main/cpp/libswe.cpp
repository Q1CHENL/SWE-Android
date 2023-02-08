#include <jni.h>
#include <string>

#include <iostream>
#include <fstream>
#include "Source/Blocks/DimensionalSplitting.hpp"
#include "Source/Scenarios/RadialDamBreakScenario.hpp"
#include "Source/Scenarios/BathymetryDamBreakScenario.hpp"
#include "Source/Scenarios/ArtificialTsunamiScenario.hpp"
#include <sstream>
#include "Source/Writers/Writer.hpp"
#include "Source/BoundaryEdge.hpp"

std::string
runner_main(std::string &scenarioName, int domain_x, int domain_y, int checkpoints, int end_time,
            std::string &baseName, const std::string &dir_name);

Scenarios::Scenario *getScenarioBasedOnName(const std::string &name);
std::string jstring2string(JNIEnv *env, jstring jStr);

/**
 * The jni interface function between c++ and java.
 * It gets called by java code.
 * */
extern "C"
JNIEXPORT jstring JNICALL
Java_com_tsunamisim_swe_SWE_main(JNIEnv *env, jobject thiz, jstring scenarioName, jint x, jint y,
                                  jint checkpoints, jint end_time, jstring baseName,
                                  jstring dir) {
    std::string sname = jstring2string(env, scenarioName);
    std::string name = jstring2string(env, baseName);
    std::string dir_name = jstring2string(env, dir);

    return env->NewStringUTF(
            runner_main(
                    sname,
                    x,
                    y,
                    checkpoints,
                    end_time,
                    name,
                    dir_name).c_str());
}



/**
 * @note:
 * The functions that runs the simulation.
 * The name indicates that it used to be the main function in the original SWE C++ code.
 * */
std::string runner_main(std::string &scenarioName, int x, int y, int checkpoints, int end_time, std::string &baseName,
                        const std::string &dir_name) {

    std::stringstream output;

    int numberOfGridCellsX = x;
    int numberOfGridCellsY = y;
    int numberOfCheckPoints = checkpoints;

    auto &scenario = *getScenarioBasedOnName(scenarioName);

    RealType cellSize_x = (scenario.getBoundaryPos(BoundaryEdge::Right) -
                           scenario.getBoundaryPos(BoundaryEdge::Left))
                          / numberOfGridCellsX;
    RealType cellSize_y = (scenario.getBoundaryPos(BoundaryEdge::Top) -
                           scenario.getBoundaryPos(BoundaryEdge::Bottom))
                          / numberOfGridCellsY;

    auto wave_block = Blocks::Block::getBlockInstance(numberOfGridCellsX, numberOfGridCellsY,
                                                      cellSize_x, cellSize_y);
    wave_block->initialiseScenario(0, 0, scenario, false);

    auto *checkPoints = new double[numberOfCheckPoints + 1];

    // Compute the checkpoints in time
    for (int cp = 0; cp <= numberOfCheckPoints; cp++) {
        checkPoints[cp] = cp * ((double) end_time / numberOfCheckPoints);
    }

    // Boundary size of the ghost layers
    Writers::BoundarySize boundarySize = {{1, 1, 1, 1}};

    // path to write output files
    std::string absolute_path = "/sdcard/" + dir_name + "/";
    std::string fileName = Writers::generateBaseFileName(baseName, absolute_path, 0, 0);

    auto writer = Writers::Writer::createWriterInstance(
            fileName,
            wave_block->getBathymetry(),
            boundarySize,
            numberOfGridCellsX,
            numberOfGridCellsY,
            cellSize_x,
            cellSize_y,
            0 * 16,
            0 * 16,
            0,
            0,
            0
    );

    auto start = std::chrono::system_clock::now();
    std::time_t start_t = std::chrono::system_clock::to_time_t(start);
    // Write basic info
    output << std::ctime(&start_t) << "\n";
    output << "Scenario: " << scenarioName << "\n";
    output << "Domain size: " << x << " x " << y << "\n";
    output << "checkpoints: " << numberOfCheckPoints << "\n";
    output << "Time: " << scenario.getEndSimulationTime() << "\n";
    output << "===========================" << "\n";

    // Write zero time step
    writer->writeTimeStep(wave_block->getWaterHeight(), wave_block->getDischargeHu(),
                          wave_block->getDischargeHv(), 0.0);

    unsigned int iterations = 0;
    double simulationTime = 0.0;

    // int cp = 1;
    // Loop over checkpoints
    for (int cp = 1; cp <= numberOfCheckPoints; cp++) {
        // Do time steps until next checkpoint is reached
        while (simulationTime < checkPoints[cp]) {
            output << "Running simulation at time " << simulationTime << std::endl;
            // Set values in ghost cells
            wave_block->setGhostLayer();

            // Compute numerical flux on each edge
            wave_block->computeNumericalFluxes();

            RealType maxTimeStepWidth = wave_block->getMaxTimeStep();

            // Update the cell values
            wave_block->updateUnknowns(maxTimeStepWidth);

            // Update simulation time with time step width
            simulationTime += maxTimeStepWidth;
            iterations++;
        }

        output << "NEW CHECKPOINT AFTER " << iterations << " ITERATIONS\n";
        // Write output
        writer->writeTimeStep(
                wave_block->getWaterHeight(), wave_block->getDischargeHu(),
                wave_block->getDischargeHv(), simulationTime
        );
    }

    std::ofstream output_text(absolute_path + "output.txt");
    output_text << output.str();

    // free space
    delete wave_block;
    delete &scenario;
    delete[] checkPoints;

    return output.str();
}

/**
 * A function that chooses different Scenario based on the name passed in
 * @param name the name of Scenario to choose
 * To avoid object slicing, Scenario is changed to be marked as pure virtual class through making functions "=0",
 * so that correct getWaterHeight etc can be called.
 * Note: new functions are implemented in classes such as ArtificialTsunamiScenario (as a derived class of Scenario)
 */
Scenarios::Scenario *getScenarioBasedOnName(const std::string &name) {
    if (name == "RadialDamBreakScenario") {
        auto *scenario = new Scenarios::RadialDamBreakScenario;
        return scenario;
    } else if (name == "BathymetryDamBreakScenario") {
        auto *scenario = new Scenarios::BathymetryDamBreakScenario;
        return scenario;
    } else {
        auto *scenario = new Scenarios::ArtificialTsunamiScenario;
        return scenario;
    }
}


/**
 * A function that converts jstring into std::string
 * @param jStr jstring that is going to be converted
 * @return std::string
 * Source: https://stackoverflow.com/questions/41820039/jstringjni-to-stdstringc-with-utf8-characters @Slerte
 */
std::string jstring2string(JNIEnv *env, jstring jStr) {
    if (!jStr)
        return "";

    const jclass stringClass = env->GetObjectClass(jStr);
    const jmethodID getBytes = env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
    const jbyteArray stringJbytes = (jbyteArray) env->CallObjectMethod(jStr, getBytes,
                                                                       env->NewStringUTF("UTF-8"));

    size_t length = (size_t) env->GetArrayLength(stringJbytes);
    jbyte *pBytes = env->GetByteArrayElements(stringJbytes, NULL);

    std::string ret = std::string((char *) pBytes, length);
    env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

    env->DeleteLocalRef(stringJbytes);
    env->DeleteLocalRef(stringClass);
    return ret;
}