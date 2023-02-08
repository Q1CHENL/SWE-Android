#include <jni.h>
#include <string>
#include <iostream>
#include <sstream>
#include <cmath>

using namespace std;

double getHRoe(double hl, double hr) {
    return (hl + hr) / 2;
}


double getURoe(double ul, double ur, double hl, double hr) {
    return (ul * sqrt(hl) + ur * sqrt(hr)) / (sqrt(hl) + sqrt(hr));
}

double getRoeEigenValue1(double uRoe, double hRoe, double g) {
    return uRoe - sqrt(g * hRoe);
}

double getRoeEigenValue2(double uRoe, double hRoe, double g) {
    return uRoe + sqrt(g * hRoe);
}

void makeInverseMatrix(double evmatrix[2][2], double inverseevmatrix[2][2]) {
    double determinant{0};
    determinant = evmatrix[0][0] * evmatrix[1][1] - evmatrix[0][1] * evmatrix[1][0];
    if (determinant != 0) {
        inverseevmatrix[0][0] = evmatrix[1][1] * (1 / (determinant));
        inverseevmatrix[0][1] = -evmatrix[0][1] * (1 / (determinant));
        inverseevmatrix[1][0] = -evmatrix[1][0] * (1 / (determinant));
        inverseevmatrix[1][1] = evmatrix[0][0] * (1 / (determinant));
    } else {
        cout << "The matrix does not have an inverse" << endl;
    }
}

void makeFluxFunctionR(double fluxFunctionR[2][1], double ur, double hr, double g) {
    double rightSide{0}, leftSide{0};
    rightSide = hr * pow(ur, 2);
    leftSide = g * pow(hr, 2) / 2;
    fluxFunctionR[1][0] = rightSide + leftSide;
}

void makeFluxFunctionL(double fluxFunctionL[2][1], double ul, double hl, double g) {
    double rightSide{0}, leftSide{0};
    rightSide = hl * pow(ul, 2);
    leftSide = g * pow(hl, 2) / 2;
    fluxFunctionL[1][0] = rightSide + leftSide;
}

void inverseAndJumpMultiplication(double inverse[2][2], double jump[2][1], double alfas[2][1]) {
    alfas[0][0] = inverse[0][0] * jump[0][0] + inverse[0][1] * jump[1][0];
    alfas[1][0] = inverse[1][0] * jump[0][0] + inverse[1][1] * jump[1][0];
}

/**
 * The jni interface function between c++ and java.
 * Unlike in libswe and libswe1d, this function runs the simulation directly.
 * It gets called by java code.
 * */
extern "C"
JNIEXPORT jstring
JNICALL Java_com_tsunamisim_swe_WaveSpeed_main(JNIEnv *env, jobject thiz, jint a, jint b, jint c,
                                               jint d) {
    double ql[2]{0, 0}, qr[2]{0, 0};
    double ul{0}, ur{0}, hRoe{0}, hl{0}, hr{0}, uRoe{0};
    double e1{0}, e2{0}; //eigenvalue 1 is e1, eigenvalue 2 is e2
    const double g{9.81};
    double evmatrix[2][2]{1, 1}, inverseevmatrix[2][2]{0};
    double alfas[2][1]{0};
    double alfa1{0}, alfa2{0};

//    Input value
    ql[0] = a;
    ql[1] = b;
    qr[0] = c;
    qr[1] = d;
//    Initialization
    double fluxFunctionR[2][1]{qr[1], 0}, fluxFunctionL[2][1]{ql[1], 0}, jumpInFluxFunction[2][1]{
            0};

    hl = ql[0];
    hr = qr[0];
    ul = ql[1] / ql[0]; // (h*u)/h
    ur = qr[1] / qr[0]; // (h*u)/h
    hRoe = getHRoe(hl, hr);
    uRoe = getURoe(ul, ur, hl, hr);

//    Implementation
    e1 = getRoeEigenValue1(uRoe, hRoe, g);
    e2 = getRoeEigenValue2(uRoe, hRoe, g);
    evmatrix[1][0] = e1;
    evmatrix[1][1] = e2;
    makeInverseMatrix(evmatrix, inverseevmatrix);
    makeFluxFunctionR(fluxFunctionR, ur, hr, g);
    makeFluxFunctionL(fluxFunctionL, ul, hl, g);
//    calculating the jump in the flux function
    jumpInFluxFunction[0][0] = fluxFunctionR[0][0] - fluxFunctionL[0][0];
    jumpInFluxFunction[1][0] = fluxFunctionR[1][0] - fluxFunctionL[1][0];
//    calculating the alfas w/ result stored in alfas
    inverseAndJumpMultiplication(inverseevmatrix, jumpInFluxFunction, alfas);
    alfa1 = alfas[0][0];
    alfa2 = alfas[1][0];
//    calculating the z vectors
    double z1[2][1]{alfa1}, z2[2][1]{alfa2};
    double x{0}, y{0};
    x = e1 * alfa1;
    y = e2 * alfa2;
    z1[1][0] = ((x == 0) ? 0 : x);
    z2[1][0] = ((y == 0) ? 0 : y);

    cout << "--------------" << endl;
    cout << endl;
    cout << "This is A^-Q :" << endl;
    if (e1 < 0) {
        cout << z1[0][0] << endl;
        cout << z1[1][0] << endl;
        cout << "This is A^+Q :" << endl;
        cout << z2[0][0] << endl;
        cout << z2[1][0] << endl;
    } else {
        cout << z2[0][0] << endl;
        cout << z2[1][0] << endl;
        cout << "This is A^+Q :" << endl;
        cout << z1[0][0] << endl;
        cout << z1[1][0] << endl;
    }
    cout << endl;
    stringstream msg;
    if (e1 * e2 > 0) { // the eigenvalues have the same sign have the same sign
        if (e1 < 0) {
            msg << "This is the wave speed of left going wave: " << e1 << "\n"
                << "This is the wave speed of right going wave: 0";

        }
        if (e1 > 0) {
            msg << "This is the wave speed of left going wave: 0" << "\n"
                << "This is the wave speed of right going wave: " << e2;

        }
    } else {
        msg << "This is the wave speed of left going wave: " << e1 << "\n"
            << "This is the wave speed of right going wave: " << e2;

    }
    string ret = msg.str();
    return env->NewStringUTF(ret.c_str());

}

