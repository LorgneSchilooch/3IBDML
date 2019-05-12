#include "library.h"
#include <iostream>
#include <math.h>
#include <vector>
#include <Eigen/Dense>

/*****************************************************************************************************
 *
 *                                 Fichier d'extraction du DLL vers C#
 *
 *****************************************************************************************************/


extern "C" {

int return42() {

    return 42;
}


}

//***************************************************************************
// Fonction Principal
//***************************************************************************
/// Fonction perceptronSimple pour le percetron Simple
/// \param X
/// \param outMat
/// \param dataTest
void perceptronSimple(MatrixXd X, MatrixXd outMat, MatrixXd dataTest) {

    ///Variable principal
    double alpha = 0.01;
    //Ajout du biais (x0=1)
    MatrixXd X0(X.rows(), 1);

    MatrixXd Xe(X.rows(), X.cols() + 1);
    Xe = addBiasXo(X0, X, Xe);
    ///******************************************
    ///Matrice de Poid et vecteur (temporaire) de poid
    //Initialisation de la Matrice de poid W
    MatrixXd W(Xe.rows(), Xe.cols());
    for (int i = 0; i < W.rows(); i++) {
        for (int j = 0; j < W.cols(); j++) {
            W.row(i).col(j) << 1;
        }
    }
    std::cout << "Matrice de poid initial W : \n" << W << endl << endl;

    //Initialisation de la matrice We (extraction du Vecteur de la matrice W)
    //We : Matrice temporaire pour le calcul de W
    MatrixXd We(1, Xe.cols());

    for (int i = 0; i < Xe.cols(); i++) {
        We.col(i) << 1;
    }
    std::cout << "Matrice de poid initial We pour le calcul par vecteur : \n" << We << endl << endl;
    std::cout << endl;



    ///*****************************************
    /// Phase d'Apprentissage
    ///*****************************************
    W << apprentissage(W, We, alpha, Xe, outMat);


    ///*****************************************
    /// Phase de Test
    ///*****************************************

    testAlgorithm(W, dataTest);


};


/// Fonction d'apprentissage du modèle
/// \param W
/// \param We
/// \param alpha
/// \param Xe
/// \param outMat
/// \return
MatrixXd apprentissage(MatrixXd W, MatrixXd We, double alpha, MatrixXd Xe, MatrixXd outMat) {
    VectorXd Wt(W.cols());
    VectorXd Xt(Xe.cols());


    std::cout << "************APPRENTISSAGE DU MODEL***************" << endl;
    //Apprentissage
    int j = 0;
    for (int i = 0; i < W.rows(); i++) {
        std::cout << "--------------- BOUCLE Apprentissage n°: " << i + 1 << "---------------" << endl;

        // le - devant Y est pour l'apprentissage
        We = learningAlgorithm(We, alpha, -outMat, Xe, i);

        Wt = We.row(0);
        Xt = Xe.row(i);

        std::cout << "------ Matrice ------" << endl;
        std::cout << "Xe[" << i << "]= " << Xe.row(i) << endl;

        std::cout << "------ Vecteur ------" << endl;
        std::cout << "Wt[" << i << "]" << endl << Wt << endl << endl;
        std::cout << "Xt[" << i << "]" << endl << Xt << endl << endl;

        std::cout << "OutMat[" << i << "]= " << outMat(i, 0) << endl;

        while (activationPrediction(Xt, Wt) != outMat(i, 0)) {
            std::cout << "ATTENTION : nb appel boucle WHile : " << j << endl << endl ;
            We = learningAlgorithm(We, alpha, outMat, Xe, i);
            Wt.col(0) = We.row(0);
            Xt.col(0) = Xe.row(i);
            j++;
        }
        W.row(i) << We;
        std::cout << "---------------FIN BOUCLE Apprentissage n°: " << i + 1 << "-------------" << endl << endl << endl;
    }
    std::cout << "Matrice de poid final W : \n" << W << endl << endl;

    std::cout << "************FIN APPRENTISSAGE DU MODEL************" << endl << endl;
    return W;
}


/// Fonction pour la partie Test
/// \param W
/// \param X
void testAlgorithm(MatrixXd W, MatrixXd X) {

    double tab[X.rows()];

    std::cout << "*************DEBUT DE LA PHASE DE TEST***************" << endl;
    MatrixXd We(W.rows(), X.cols());
    We = W.rightCols(2);

    std::cout << "We: " << endl << " W0    W1 \n" << We << endl << endl;

    MatrixXd X0(X.rows(), 1);
    VectorXd Wt(2);
    VectorXd Xt(2);

    for (int i = 0; i < W.rows(); i++) {
        Wt = We.row(i);
        Xt = X.row(i);

        tab[i] = activationPrediction(Xt, Wt);
    }
    for (int i = 0; i < W.rows(); i++) {
        std::cout << "Sorti Test: " << tab[i] << endl;
    }

    std::cout << "*************FIN DE LA PHASE DE TEST***************" << endl;

};


//***************************************************************************
// Fonction Secondaire
//***************************************************************************

/// Calcul du W avec le learningAlgorithm
/// \param W
/// \param alpha
/// \param Y
/// \param X
/// \param i
/// \return
MatrixXd learningAlgorithm(MatrixXd W, double alpha, MatrixXd Y, MatrixXd X, int i) {

    std::cout << "*******  learningAlgorithm  *******" << endl;
    W.col(0) = W.col(0) + alpha * Y.row(i) * X.row(i).col(0);
    std::cout << "We.col(0): " << W.col(0) << endl;

    W.col(1) = W.col(1) + alpha * Y.row(i) * X.row(i).col(1);
    std::cout << "We.col(1): " << W.col(1) << endl;

    W.col(2) = W.col(2) + alpha * Y.row(i) * X.row(i).col(2);
    std::cout << "We.col(2): " << W.col(2) << endl;

    std::cout << "*******  END learningAlgorithm  *******" << endl;
    return W;
}


MatrixXd Rosenblatt(MatrixXd W, double alpha, MatrixXd Y, MatrixXd X, int i) {

    W.col(0) = W.col(0) + alpha * (Y.row(i) - calculSignMat(X.row(i), W.row(i))) * X.row(i).col(0);
    std::cout << "We.col(0): " << W.col(0) << endl;

    W.col(1) = W.col(1) + alpha * (Y.row(i) - calculSignMat(X.row(i), W.row(i))) * X.row(i).col(1);
    std::cout << "We.col(1): " << W.col(1) << endl;

    W.col(2) = W.col(2) + alpha * (Y.row(i) - calculSignMat(X.row(i), W.row(i))) * X.row(i).col(2);
    std::cout << "We.col(2): " << W.col(2) << endl;

    return W;
}


/// Fonction utilisé de l'aprentissage pour vérifier si on respecte bien la sortie attendu ou pas
/// Permet de prédire dans la partie test si l'individu est bleu ou pas bleu
/// \param X
/// \param W
/// \return
double activationPrediction(VectorXd X, VectorXd W) {
    std::cout << "Activation prediction : " << (W.transpose() * X).determinant() << endl;
    if ((W.transpose() * X).determinant() < 0) {

        return (-1);
    } else return (1);
};


///Fonction d'ajout du bias x0
/// \param X0
/// \param X
/// \param Xe
/// \return Xe la nouvelle matrice avec le bias x0 = 1
MatrixXd addBiasXo(MatrixXd X0, MatrixXd X, MatrixXd Xe) {
    //Préparation de la matrice pour x0 = 1
    for (int i = 0; i < X.rows(); i++) {
        X0.row(i) << 1;
    }

    ///Ajout dans Xe pour (bias) xo = 1;
    for (int i = 0; i < X.rows(); i++) {
        Xe.col(0) << X0;
    }
    //Donnée préparé pour le calcul
    for (int i = 1; i < Xe.cols(); i++) {
        Xe.col(i) << X.col(i - 1);
    }
    std::cout << "Xe: " << endl << "  Xo  x   y\n" << Xe << endl;
    return Xe;
}


//*************************************************
//Fonction utilisé dans le main pour la simulation
//*************************************************

/// Fonction qui determine la fonction attendu pour la matrice d'entrée du perceptron
/// A modifier par rapport au data set d'entrée
/// \param X
/// \param outMat
/// \return
MatrixXd deterOut(MatrixXd X, MatrixXd outMat) {
    Vector2d W2(1, 1);

    std::cout << "Sortie attendu Yk: \n";
    for (int i = 0; i < X.rows(); i++) {

        outMat.row(i) = calculSignMat(X.row(i), W2);
        std::cout << "OutMat[" << i << "]= " << outMat.row(i) << endl;
    }

    std::cout << endl;
    return outMat;
}


/// Calcul de la sortie avec des matrices
/// \param X
/// \param W
/// \return
MatrixXd calculSignMat(Vector2d X, Vector2d W) {
    return (W.transpose() * X).array().sign();
};
