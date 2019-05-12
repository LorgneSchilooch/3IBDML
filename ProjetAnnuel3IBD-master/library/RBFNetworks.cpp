//
// Created by Manitra Ranaivoharison on 25/06/2018.
//
#include <Eigen/Dense>
#include "library.h"
#include <iostream>
#include <cmath>
#include <vector>


#include "RBFNetworks.h"

using namespace std;

extern "C" {

}
///
/// \param X Temporaire, sera transformé en Tableau à 3 dimensions plus tard
/// \param Y
/// \param nbLayer
/// \param nbPerceptronInOneHiddenLayer
void regressionRBF(MatrixXd X,MatrixXd Y, int nbLayer,MatrixXd phi, int nbPerceptronInOneHiddenLayer,double Gamma) {


    ///*****************************************
    ///Conversion des tableaux => Matrice
    ///*****************************************








    ///*****************************************
    ///FIN de la conversion
    ///*****************************************


    /// Taille des Matrices
    int dimL = nbLayer;
    int dimI = nbPerceptronInOneHiddenLayer;
    int dimJ = nbPerceptronInOneHiddenLayer;

    ///Vecteur de Poids W
    double ***W;
    // begin memory allocation
    W = new double **[dimL];
    for (int l = 0; l < dimL; ++l) {
        W[l] = new double *[dimI];
        for (int i = 0; i < dimI; ++i) {
            W[l][i] = new double[dimJ];
            for (int j = 0; j < dimJ; ++j) { // initialize the values to whatever you want the default to be
                W[l][i][j] = 1;
            }
        }
    }

    ///Appel de calcul de phi


    //calculphi(X,Gamma,phi);


    //calculW(W,phi,Y);
}


void classificationRBF(MatrixXd output, MatrixXd W) {

}


void calculOutputLayer(MatrixXd outPut,MatrixXd X,MatrixXd W,double Gamma) {
    VectorXd Xe;

    //On inserte dans le vector un individus
    for(int j = 0 ; j < X.cols() ; j++){
        Xe(0) = X(0,j);
    }

      //= W * exp(-Gamma * ((abs(X - Xe)) * (abs(X - Xe)));
}


void calculphi(MatrixXd X, double Gamma, MatrixXd phi) {
    for (int i = 0; i < X.rows(); i++) {
        for (int j = 0; j < X.cols(); j++) {

            ///Valeur différence entre les entrée en valeur absolue
            phi(i, j) = abs(X(i, 0) - X(0, j));

            ///Mise à la puissance + multiplication par Gamma
            phi(i, j) = -Gamma * (phi(i, j) * phi(i, j));

            std::cout << "Xs(" << i << "," << j << ")=" << phi(i, j) << endl;
        }
    }
}


MatrixXd calculW(MatrixXd W, MatrixXd phi, MatrixXd Y) {

    if (phi.determinant() != 0) {
        W = phi.inverse() * Y.transpose();
        return W;
    } else std::cout << "Attention le determinant de Phi vaux 0";
    return W;
}







