//
// Created by Manitra Ranaivoharison on 05/06/2018.
//

#ifndef INC_2018_3A_IBD_PERCEPTRONSIMPLE_H
#define INC_2018_3A_IBD_PERCEPTRONSIMPLE_H
#include "Eigen/Dense"

using namespace Eigen;
using namespace std;

void dllTrainLinearModel(double *, MatrixXd, MatrixXd, double, bool);

MatrixXd dllLearningLinearModel(VectorXd, MatrixXd, MatrixXd, double, bool);

bool testAlgorithm(MatrixXd, MatrixXd, int result);

int dllPredictLinearModel(VectorXd, VectorXd);

#endif //INC_2018_3A_IBD_PERCEPTRONSIMPLE_H
