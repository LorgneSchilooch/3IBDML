//
// Created by Manitra Ranaivoharison on 19/06/2018.

#include <Eigen/Dense>
#include "library.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <math.h>
#include "PerceptronMultiCouche.h"
#include "stdlib.h"
#include "time.h"

using namespace std;

extern "C" {

}

void perceptronMultiCouche(int nbLayer, int nbHiddenLayer, int nbOutputLayer, int nbPerceptronInOneHiddenLayer,
                           int nbNeuroneOutputLayer, MatrixXd X, double alpha, int choiceActivationFunction) {



    ///**************************** INITIALISATION DES VARIABLES ********************************************
    /// Taille des Matrices
    int dimX = nbLayer;
    int dimY = nbPerceptronInOneHiddenLayer;
    int dimZ = nbPerceptronInOneHiddenLayer;
    int dimXy = nbNeuroneOutputLayer;
    int N = 1;
    ///Vecteur de Poids W
    double ***W;
    // begin memory allocation
    W = new double **[dimX];
    for (int x = 0; x < dimX; ++x) {
        W[x] = new double *[dimY];
        for (int y = 0; y < dimY; ++y) {
            W[x][y] = new double[dimZ];
            for (int z = 0; z < dimZ; ++z) { // initialize the values to whatever you want the default to be
                W[x][y][z] = (float) rand() / RAND_MAX;
            }
        }
    }

    ///Indivividus
    ///TEST
    VectorXd xT(X.cols() + 2);
    xT << -4, 0, -10, 1;

    ///Sortie de la couche de Sortie
    double *Y;
    // begin memory allocation
    Y = new double[dimXy];
    for (int x = 0; x < dimXy; ++x) {
        Y[x] = 0;        // initialize the values to whatever you want the default to be
    }

    ///Sortie de chaque neurone caché
    /////Xp[couche][perceptronCoucheSuivant] => Xp[l][i]
    double **Xp;
    // begin memory allocation
    Xp = new double *[dimX];
    for (int x = 0; x < dimX; ++x) {
        Xp[x] = new double[dimY];
        for (int y = 0; y < dimY; ++y) {
            Xp[x][y] = 0;        // initialize the values to whatever you want the default to be
        }
    }

    /// Kronecker
    double **Ke;
    // begin memory allocation
    Ke = new double *[dimX];
    for (int x = 0; x < dimX; ++x) {
        Ke[x] = new double[dimY];
        for (int y = 0; y < dimY; ++y) {
            Ke[x][y] = 0;        // initialize the values to whatever you want the default to be
        }
    }
    ///****************************************************************************************************************


    ///Propagation
    propagation(xT, W, nbHiddenLayer, nbPerceptronInOneHiddenLayer, Xp, Y, nbNeuroneOutputLayer,
                choiceActivationFunction);

    ///A ce niveau l = 3

    ///retro-Propagation
    retroPropagationGradiant(Ke, W, nbHiddenLayer, nbPerceptronInOneHiddenLayer, Xp, Y, nbNeuroneOutputLayer, alpha);

};

///************************* Main de la propagation ************************
void propagation(VectorXd xT, double ***W, int nbHiddenLayer, int nbPerceptronInOneHiddenLayer, double **Xp, double *Y,
                 int nbNeuroneOutputLayer, int choiceActivationFunction) {
    int l = 1;
    ///Calculer la premier couche
    calculInputLayer(W, nbPerceptronInOneHiddenLayer, xT, Xp, l, choiceActivationFunction);
    l += 1;

    ///Calculer les perceptron des couches cachés
    while (l <= nbHiddenLayer) {
        calculHiddenLayer(Xp, W, nbPerceptronInOneHiddenLayer, l, choiceActivationFunction);
        l++;
    }

    ///Calculer la dernière couche
    calculOutputLayer(Y, W, Xp, nbPerceptronInOneHiddenLayer, nbNeuroneOutputLayer, l, choiceActivationFunction);
}
///**************************************************************************



///************************* Main de la retro-propagation ************************
void
retroPropagationGradiant(double **Ke, double ***W, int nbHiddenLayer, int nbPerceptronInOneHiddenLayer, double **Xp,
                         double *Y,
                         int nbNeuroneOutputLayer, double alpha) {

    int l = 3;

    int choix = 0;
    std::cout << "*********************************************************" << std::endl;
    std::cout << "Retro-propagation" << std::endl << std::endl;
    std::cout << "Vous voulez utiliser [1]Classification ou [2]Regression ?" << std::endl;
    std::cin >> choix;

    if (choix == 1) {
        ///Si classification
        //Pour tous les neurones j de la dernière couche L calculer
        classificationKroneckerOutputLayer(Ke, Xp, Y, nbNeuroneOutputLayer, l);

        //En déduire pour tous les autres neurones de l’avant dernière couche à la première
        classificationKroneckerHiddenLayer(Ke, Xp, W, nbPerceptronInOneHiddenLayer, nbNeuroneOutputLayer, l, Y);

        //Puis Mise à jour de tous les 􏰆􏰏W
        classificationUpdateWeight(W, alpha, Xp, Ke, nbPerceptronInOneHiddenLayer, nbNeuroneOutputLayer, l);
    } else {
        ///Si Regression
        //Pour tous les neurones j de la dernière couche 􏰃 calculer
        regressionKroneckerOutputLayer(Ke, Xp, Y, nbNeuroneOutputLayer, l);

        //En déduire pour tous les autres neurones de l’avant dernière couche à la première
        regressionKroneckerHiddenLayer(Ke, Xp, W, nbPerceptronInOneHiddenLayer, nbNeuroneOutputLayer, l, Y);

        //Puis Mise à jour de tous les 􏰆􏰏W
        regressionUpdateWeight(W, alpha, Xp, Ke, nbPerceptronInOneHiddenLayer, nbNeuroneOutputLayer, l);
    }
}

///*******************************************************************************

///####################################     PROPAGATION   FUNCTION      ##################################################

///Fonction pour les couche caché
void calculInputLayer(double ***W, int nbPerceptronInOneHiddenLayer, VectorXd X, double **Xp, int l,
                      int choiceActivationFunction) {

    ///Calcul de la somme Xi*Wi de chaque Perceptron
    ///Applique le signoid sur chaque Perceptron

    std::cout << "CALCUL DE LA COUCHE D ENTREE => COUCHE CACHE " << l << " :" << std::endl << std::endl;

    //******** Affichage **********
    std::cout <<"X(0)= " << X(0) << endl;
    std::cout <<"X(1)= " << X(1) << endl;
    std::cout <<"X(2)= " << X(2) << endl;
    std::cout <<"X(3)= " << X(3) << endl << endl;
    //********* FIN Affichage **********


    if (choiceActivationFunction == 1) {
        ///Utilisation de Tanh
        std::cout << "Fonction d'activation Tanh :" <<endl;
        for (int i = 0; i < nbPerceptronInOneHiddenLayer; i++) {


            //********* Affichage **************
            std::cout <<"W[0][1][" << i <<"]= " << W[0][1][i] << endl;
            std::cout <<"W[0][2][" << i <<"]= " << W[0][2][i] << endl;
            std::cout <<"W[0][3][" << i <<"]= " << W[0][3][i] << endl;
            std::cout <<"W[0][4][" << i <<"]= " << W[0][4][i] << endl;
            std::cout << "Avant fct Activation : " << X(0) * W[0][1][i] + X(1) * W[0][2][i] + X(2) * W[0][3][i] + X(3) * W[0][4][i] << std::endl;
            //********* FIN Affichage **********

            ///*****    CALCUL A GARDER     *******
            ///Calcul à la sortie du Perceptron
            Xp[l][i] = tanh(X(0) * W[0][1][i] + X(1) * W[0][2][i] + X(2) * W[0][3][i] + X(3) * W[0][4][i]);
            ///************************************

            //********* Affichage **********
            std::cout << "Xp[" << l << "][" << i << "] = " << Xp[l][i] << std::endl << std::endl;
            //********* FIN Affichage **********
        }
    } else {
        ///Utilisation de Sigmoid
        std::cout << "Fonction d'activation Sigmoid :" <<endl;
        for (int i = 0; i < nbPerceptronInOneHiddenLayer; i++) {

            //********* Affichage **********
            std::cout <<"W[0][1][" << i <<"]= " << W[0][1][i] << endl;
            std::cout <<"W[0][2][" << i <<"]= " << W[0][2][i] << endl;
            std::cout <<"W[0][3][" << i <<"]= " << W[0][3][i] << endl;
            std::cout <<"W[0][4][" << i <<"]= " << W[0][4][i] << endl;
            std::cout << "Avant fct Activation : " << X(0) * W[0][1][i] + X(1) * W[0][2][i] + X(2) * W[0][3][i] + X(3) * W[0][4][i] << std::endl << std::endl;
            //********* FIN Affichage **********

            ///*****    CALCUL A GARDER     *******
            ///Calcul à la sortie du Perceptron
            Xp[l][i] = sigmoid(X(0) * W[0][1][i] + X(1) * W[0][2][i] + X(2) * W[0][3][i] + X(3) * W[0][4][i]);
            ///************************************

            //********* Affichage **********
            std::cout << "Xp[" << l << "][" << i << "] = " << Xp[l][i] << std::endl << std::endl;
            //********* FIN Affichage **********
        }
    }


    std::cout << std::endl;
};


///Fonction pour la couche d'entrée
void
calculHiddenLayer(double **Xp, double ***W, int nbPerceptronInOneHiddenLayer, int l, int choiceActivationFunction) {

    std::cout << "CALCUL DU COUCHE CACHE " << l - 1 << " => COUCHE CACHEE " << l << " :" << std::endl << std::endl;

    //******   Affichage   ******
    for(int i = 0 ; i < nbPerceptronInOneHiddenLayer ; i ++ ){
        std::cout << "Xp[" << l-1 << "][" << i << "]= " << Xp[l-1][i] << std::endl;
        std::cout << "Xp[" << l << "][" << i << "]= " << Xp[l][i] << std::endl << std::endl;
    }
    std::cout << endl;
    //***************************


    for (int i = 0; i < nbPerceptronInOneHiddenLayer; i++) {
        std::cout << "calcul de perceptron : " << i <<" dans couche : " << l << endl;
        for (int j = 0; j < nbPerceptronInOneHiddenLayer; j++) {

            //Calcul avant la fonction d'activation
            //******   Affichage   ******
            std::cout << "Xp[" << l-1 << "][" << i << "]= " << Xp[l-1][i] << std::endl;
            std::cout << "W[" << l <<"][" << j <<"][" <<i <<"]=" << W[l][j][i] << std::endl;
            std::cout << "Xp[" << l << "][" << i << "]= " << Xp[l][i] << std::endl << std::endl;
            std::cout << "resultat : " << j << " = " << Xp[l - 1][j] * W[l][j][i] << std::endl << std::endl;
            //***************************

            ///*****    CALCUL A GARDER     *******
            Xp[l][i] = Xp[l - 1][j] * W[l][j][i] + Xp[l][i] ;
            ///************************************

            //******   Affichage   ******
            std::cout <<"resultat :\t";
            std::cout << "Xp[" << l << "][" << i << "]= " << Xp[l][i] << std::endl;
            //***************************
        }



        if (choiceActivationFunction == 1) {
            ///*****    CALCUL A GARDER     *******
            ///Calcul de Tanh
            Xp[l][i] = tanh(Xp[l][i]);
            ///************************************

            //******   Affichage   ******
            std::cout << "Tanh(Xp[" << l << "][" << i << "]) = " << Xp[l][i] << std::endl << std::endl;
            //***************************

        } else {
            ///*****    CALCUL A GARDER     *******
            ///Calcul de Sigmoid
            Xp[l][i] = sigmoid(Xp[l][i]);
            ///************************************

            //******   Affichage   ******
            std::cout << "Sigmoid(Xp[" << l << "][" << i << "]) = " << Xp[l][i] << std::endl << std::endl;
            //***************************
        }
    }
};


///Fonction pour la couche de sortie
void calculOutputLayer(double *Y, double ***W, double **Xp, int nbPerceptronInOneHiddenLayer, int nbNeuroneOutputLayer,
                       int l, int choiceActivationFunction) {

    std::cout << "CALCUL DU COUCHE CACHE " << l - 1 << " => COUCHE " << l  <<" (SORTIE): " << std::endl << std::endl;

    //******   Affichage   ******
    for(int i = 0 ; i < nbPerceptronInOneHiddenLayer ; i ++ ){
        std::cout << "Xp[" << l-1 << "][" << i << "]= " << Xp[l-1][i] << std::endl;
    }
    std::cout << endl;

    for(int i = 0 ; i < nbNeuroneOutputLayer ; i ++ ){
        std::cout << "Xp[" << l << "][" << i << "]= " << Xp[l][i] << std::endl;
    }
    std::cout << endl;
    //***************************


    for (int i = 0; i < nbNeuroneOutputLayer; i++) {
        for (int j = 0; j < nbPerceptronInOneHiddenLayer; j++) {

            //******   Affichage   ******
            std::cout <<  "Xp[" <<l - 1 << "][" <<i <<"]=" << Xp[l - 1][i] << std::endl;
            std::cout <<  "W[" << l <<"][" <<j <<"][" << i <<"]= " << W[l][j][i] << std::endl;
            std::cout << "Y[" << i <<"]=" << Y[i] << std::endl ;
            std::cout << "calcul perceptron " << j <<" Neurone " << i  <<" = " << Xp[l - 1][i] * W[l][j][i] << std::endl << std::endl;
            //***************************

            ///*****    CALCUL A GARDER     *******
            Y[i] = Xp[l - 1][i] * W[l][j][i] + Y[i];
            ///************************************

            //******   Affichage   ******
            std::cout << "Calcul de Somme sur Y" << endl;
            std::cout << "Y[" << i << "]= " << Y[i] << std::endl <<std::endl;
            //***************************
        }

        //******   Affichage   ******
        std::cout << "Y[" << i << "]= " << Y[i] << std::endl;
        //***************************

        if (choiceActivationFunction == 1) {

            ///*****    CALCUL A GARDER     *******
            ///Calcul de Tanh
            Y[i] = tanh(Y[i]);
            ///************************************

            std::cout << "Tanh(Y[" << i << "]) = " << Y[i] << std::endl << std::endl;
        } else {

            ///*****    CALCUL A GARDER     *******
            ///Calcul de Sigmoid
            Y[i] = sigmoid(Y[i]);
            ///************************************

            std::cout << "Sigmoid(Y[" << i << "]) = " << Y[i] << std::endl << std::endl;
        }

    }
};




///#################################################################################################################

///####################################     RETRO PROPAGATION   FUNCTION      ##############################################


///****************************************************************************************************
///                                         Pour la classification
///****************************************************************************************************

///Calcul de la derniere couche
void classificationKroneckerOutputLayer(double **Ke, double **Xp, double *Y, int nbNeuroneOutputLayer, int l) {

    ///Pour tous les neurones j de la dernière couche 􏰃 calculer :
    for (int j = 0; j < nbNeuroneOutputLayer; j++) {

        //*********** Affichage ***************
        std::cout << "Y[" << j << "]= " << Y[j] << std::endl;
        std::cout << "Xp[" << l << "][" << j << "]=" << Xp[l][j] << std::endl;
        std::cout << "calcul "<<j <<":" << Xp[l][j] - Y[j] << std::endl;
        std::cout << "pow(Xp[" << l << "][" << j << "],2)=" << pow(Xp[l][j], 2) << std::endl << std::endl;
        //*************************************

        ///*****    CALCUL A GARDER     *******
        Ke[l][j] = (1 - (pow(Xp[l][j], 2)) * (Xp[l][j] - Y[j]));
        ///************************************

        //*********** Affichage ***************
        std::cout << "Ke[" << l << "][" << j << "]=" << Ke[l][j] << std::endl << std::endl;
        //*************************************
    }
}

void classificationKroneckerHiddenLayer(double **Ke, double **Xp, double ***W, int nbPerceptronInOneHiddenLayer,
                                        int nbNeuroneOutputLayer, int l, double *Y) {

    double *SommeTmp;
    // begin memory allocation
    SommeTmp = new double[nbPerceptronInOneHiddenLayer];
    for (int x = 0; x < nbPerceptronInOneHiddenLayer; ++x) {
        SommeTmp[x] = 0;        // initialize the values to whatever you want the default to be
    }


    ///En déduire pour tous les autres neurones de l’avant dernière couche à la première
    for (int i = 0; i < nbPerceptronInOneHiddenLayer; i++) {
        for (int j = 0; j < nbNeuroneOutputLayer; j++) {
            SommeTmp[i] = W[l][i][j] * Ke[l][j] + SommeTmp[i];
            std::cout << "SommeTmp[" << i << "]=" << SommeTmp[i] << std::endl;
        }
    }

    for (int j = 0; j < nbPerceptronInOneHiddenLayer; j++) {
        Ke[l - 1][j] = (1 - pow(Xp[l - 1][j], 2)) * SommeTmp[j];
        std::cout << "Ke[" << l - 1 << "][" << j << "] * SommeTmp =" << Ke[l - 1][j] << std::endl << std::endl;
    }
}

void classificationUpdateWeight(double ***W, double alpha, double **Xp, double **Ke, int nbPerceptronInOneHiddenLayer,
                                int nbNeuroneOutputLayer, int l) {

    ///Mise à jour de tout les W[l][i][j]
    for (int i = 0; i < nbPerceptronInOneHiddenLayer; i++) {
        for (int j = 0; j < nbNeuroneOutputLayer; j++) {
            W[l][i][j] = W[l][i][j] - alpha * Xp[l - 1][i] * Ke[l][j];
            std::cout << "W[" << l << "][" << i << "][" << j << "]= " << Ke[l - 1][j] << std::endl << std::endl;
        }
    }
}


///****************************************************************************************************
///                                         Pour la Regression
///****************************************************************************************************
///Calcul de la derniere couche
void regressionKroneckerOutputLayer(double **Ke, double **Xp, double *Y, int nbNeuroneOutputLayer, int l) {

    ///Pour tous les neurones j de la dernière couche 􏰃 calculer :
    for (int j = 0; j < nbNeuroneOutputLayer; j++) {
        Ke[l][j] = Xp[l][j] - Y[j];
        std::cout << "Ke[" << l << "][" << j << "]= " << Ke[l][j] << std::endl;
    }
}

void regressionKroneckerHiddenLayer(double **Ke, double **Xp, double ***W, int nbPerceptronInOneHiddenLayer,
                                    int nbNeuroneOutputLayer, int l, double *Y) {

    double *SommeTmp;
    // begin memory allocation
    SommeTmp = new double[nbPerceptronInOneHiddenLayer];
    for (int x = 0; x < nbPerceptronInOneHiddenLayer; ++x) {
        SommeTmp[x] = 0;        // initialize the values to whatever you want the default to be
    }

    ///En déduire pour tous les autres neurones de l’avant dernière couche à la première
    for (int i = 0; i < nbPerceptronInOneHiddenLayer; i++) {
        for (int j = 0; j < nbNeuroneOutputLayer; j++) {
            SommeTmp[i] = W[l][i][j] * Ke[l][j] + SommeTmp[i];
            std::cout << "SommeTmp[" << i << "]=" << SommeTmp[i] << std::endl;
        }
    }

    for (int j = 0; j < nbPerceptronInOneHiddenLayer; j++) {
        Ke[l - 1][j] = (1 - pow(Xp[l - 1][j], 2)) * SommeTmp[j];
        std::cout << "Ke[" << l - 1 << "][" << j << "] * SommeTmp =" << Ke[l - 1][j] << std::endl << std::endl;
    }
}

void regressionUpdateWeight(double ***W, double alpha, double **Xp, double **Ke, int nbPerceptronInOneHiddenLayer,
                            int nbNeuroneOutputLayer, int l) {

    ///Mise à jour de tout les W[l][i][j]
    for (int i = 0; i < nbPerceptronInOneHiddenLayer; i++) {
        for (int j = 0; j < nbNeuroneOutputLayer; j++) {
            W[l][i][j] = W[l][i][j] - alpha * Xp[l - 1][i] * Ke[l][j];
            std::cout << "W[" << l << "][" << i << "][" << j << "]= " << Ke[l - 1][j] << std::endl << std::endl;
        }
    }
}

///#################################################################################################################


double sigmoid(double x) {
    double exp_value;
    double return_value;

    /*** Exponential calculation ***/
    exp_value = exp(-x);

    /*** Final sigmoid value ***/
    return_value = 1 / (1 + exp_value);

    return return_value;
}

double tanh(double x) {
    double exp_value;
    double return_value;

    /*** Exponential calculation ***/
    exp_value = exp(-2 * x);

    /*** Final sigmoid value ***/
    return_value = (1 - exp_value) / (1 + exp_value);

    return return_value;
}

