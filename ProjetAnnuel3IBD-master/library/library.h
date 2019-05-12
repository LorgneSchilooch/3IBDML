#ifndef INC_2018_3A_IBD_LIBRARY_H
#define INC_2018_3A_IBD_LIBRARY_H

#include <Eigen/Dense>


using namespace Eigen;
using namespace std;

/*****************************************************************************************************
 *
 *                                  Fonctions pour la regression lineaire
 *
 *****************************************************************************************************/

///Fonction principal
void perceptronSimple(MatrixXd,MatrixXd,MatrixXd);

///Fonction d'apprentissage
MatrixXd apprentissage(MatrixXd,MatrixXd,double,MatrixXd,MatrixXd);

///Fonction de calcul du W
MatrixXd learningAlgorithm(MatrixXd,double,MatrixXd,MatrixXd,int);

MatrixXd Rosenblatt(MatrixXd,double,MatrixXd, MatrixXd,int);


//Fonction de test
void testAlgorithm(MatrixXd,MatrixXd);

//Fonction d'activation
double activationPrediction(VectorXd,VectorXd);


MatrixXd calculSignMat(Vector2d, Vector2d);

MatrixXd deterOut(MatrixXd,MatrixXd);

MatrixXd addBiasXo(MatrixXd,MatrixXd,MatrixXd);

/*****************************************************************************************************
 *
 *                                  Fonctions pour perceptron multicouche
 *
 *****************************************************************************************************/


#endif