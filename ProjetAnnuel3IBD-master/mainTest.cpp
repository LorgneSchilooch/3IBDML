#include <iostream>
#include <fstream>
#include <Eigen/Dense>
#include <array>
#include <vector>
#include "library/library.h"
#include "library/PerceptronMultiCouche.h"


using namespace std;
using namespace Eigen;
using namespace std;


/*****************************************************************************************************
 *
 *                                  Main pour les tests de calculs
 *
 *****************************************************************************************************/


///CONDITION
/// Vecteur X : -1 : Bleu ; 1 : Rouge
/// Bleu : 0 ; Rouge : 1 ; Vert : -1

// bad
#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))

// good
template <typename T,unsigned S>
inline unsigned arraysize(const T (&v)[S]) { return S; }

int main() {

    ///*****************************************
    /*
     * PARTIE BRUT POUR LE TEST DES CALCULS
     */
    ///*****************************************



    ///*****************************************
    ///DataSet d'entrainement
    ///*****************************************

    //Matrice de la problèmatique numéro 2
    Eigen::MatrixXd X2(11, 2);

    X2 << -10, -6,
            -12, -3,
            -10, -2,
            -10, -4,
            -8, -5,
            -7, -7,
            -13, -5,
            2, 4,
            2, 6,
            5, 4,
            0, 5;

    //std::cout << "dataset d'apprentissage du modele X2 : \n" << X2 << endl << endl;

    // Matrice Non classé pour problèmatique 1
    Eigen::MatrixXd Xe2(11, 4);

    Xe2 << -14,0,10,1,
            2,0, 4,1,
            -9,0, -6,1,
            -10,0, -5,1,
            4,0, 10,1,
            -8,0, -2,1,
            -8,0, -4,1,
            -2,0, -1,1,
            6,0, 12,1,
            -20,0, -18,1,
            0,0, 8,1;

    //std::cout << "X2: \n" << X2 << endl;


    ///*****************************************
    ///Determine la sortie attendu
    //MatrixXd outMat(X2.rows(), 1);
    //outMat = deterOut(X2, outMat);



    ///*****************************************************
    ///TEST DU PERCEPTRON SIMPLE
    ///*****************************************************
    //perceptronSimple(X2,outMat,Xe2);

    ///*****************************************************
    ///TEST DU RESEAU DE NEURONE (PERCEPTRON MULTICOUCHE)
    ///*****************************************************
    //perceptronMultiCouche(int nbLayer, int nbHiddenLayer, int nbOutputLayer, int nbPerceptronInOneHiddenLayer,int nbNeuroneOutputLayer, MatrixXd X, double delta)
    perceptronMultiCouche(4,2,1,5,3,X2,0.05,0);






    ///*****************************************************
    ///PARTIE TEST RBF
    ///*****************************************************
    /*
    double alpha = 0.10000000001;
    MatrixXd Xs(11,4);

    for(int i = 0 ; i < Xe2.rows() ; i ++){
        for(int j = 0 ; j < Xe2.cols() ; j ++){

            ///Valeur différence entre les entrée en valeur absolue
            Xs(i,j) =  abs(Xe2(i, 0) - Xe2(0, j));

            ///Mise à la puissance + multiplication par Gamma
            Xs(i,j) = exp(-alpha * (Xs(i,j) * Xs(i,j)));

        }
        std::cout << "Xs(" << i <<") = " << Xs.row(i) <<endl;
    }

    std::cout << endl << "Xs : " << endl << Xs << endl <<endl;
    //std::cout << endl << "Xs : " << endl << Xs.inverse() << endl <<endl;


    //cout << Xe2.array().tanh() << endl;

    */



}


