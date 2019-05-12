#include "PerceptronSimple.h"
#include "library.h"
#include <iostream>
#include <cmath>
#include <vector>

/*
 * Classe DLL Synthétisé pour le perceptron Simple
 */

extern "C" {

double *createLinearModel() {
    double *w = new double[3];
    return w;
}

void deleteLinearModel(double *w) {
    delete w;
}


void trainLinearModel(double *w, double x[], double y[], double result[], double alpha, int len, bool rosenblatt) {

    MatrixXd xMat(len, 3);
    for (int i = 0; i < len; i++) {
        xMat.row(i).col(0) << 1;
        xMat.row(i).col(1) << x[i];
        xMat.row(i).col(2) << y[i];
    }

    MatrixXd resultMat(len, 1);

    for (int i = 0; i < len; i++) {
        resultMat.row(i).col(0) << result[i];
    }

    dllTrainLinearModel(w, xMat, resultMat, alpha, rosenblatt);
}

int predictLinearModel(double *w, double x, double y) {

    VectorXd xp(3);
    VectorXd wp(3);
    xp(0) = 1;
    xp(1) = x;
    xp(2) = y;
    wp(0) = w[0];
    wp(1) = w[1];
    wp(2) = w[2];
    return dllPredictLinearModel(wp, xp);

}

}

// ######################### Fonctions internes #########################


void dllTrainLinearModel(double *w, MatrixXd x, MatrixXd result, double alpha, bool choice) {

    VectorXd wResult(3);
    wResult << 1, 1, 1;

    wResult << dllLearningLinearModel(wResult, x, result, alpha, choice);
    w[0] = wResult(0);
    w[1] = wResult(1);
    w[2] = wResult(2);
}

MatrixXd dllLearningLinearModel(VectorXd w, MatrixXd x, MatrixXd result, double alpha, bool choice) {

    VectorXd xt(x.cols());
    for (int i = 0; i < x.rows(); i++) {
        xt = x.row(i);
        if (!choice) {
            while (dllPredictLinearModel(xt, w) != result.coeff(i, 0)) {
                w(0) = w(0) + alpha * result.coeff(i, 0) * xt(0);
                w(1) = w(1) + alpha * result.coeff(i, 0) * xt(1);
                w(2) = w(2) + alpha * result.coeff(i, 0) * xt(2);
            }
        } else {
            w(0) = w(0) + alpha * (result.coeff(i, 0) - dllPredictLinearModel(xt, w)) * xt(0);
            w(1) = w(1) + alpha * (result.coeff(i, 0) - dllPredictLinearModel(xt, w)) * xt(1);
            w(2) = w(2) + alpha * (result.coeff(i, 0) - dllPredictLinearModel(xt, w)) * xt(2);
        }
    }

    return w;
}

bool testAlgorithm(VectorXd w, VectorXd x, int result) {

    return dllPredictLinearModel(w, x) == result;

};

int dllPredictLinearModel(VectorXd w, VectorXd x) {
    return (w.transpose() * x).determinant() < 0 ? -1 : 1;
};


