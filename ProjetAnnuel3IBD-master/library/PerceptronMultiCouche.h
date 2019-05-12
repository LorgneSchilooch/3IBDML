//
// Created by Manitra Ranaivoharison on 19/06/2018.
//

#ifndef INC_2018_3A_IBD_PERCEPTRONMULTICOUCHE_H
#define INC_2018_3A_IBD_PERCEPTRONMULTICOUCHE_H

#endif //INC_2018_3A_IBD_PERCEPTRONMULTICOUCHE_H

typedef struct Neurone Neurone;
struct Neurone
{
    Eigen::MatrixXd neurone;

};


void perceptronMultiCouche(int, int, int, int,int,MatrixXd,double,int);

///***********      Fonction pour la Propagation       ***************
void calculInputLayer( double***,int,VectorXd,double**,int,int);

void calculHiddenLayer(double**,double***,int,int k,int);

void calculOutputLayer(double*,double***,double **,int,int,int,int);

void propagation(VectorXd,double***,int,int,double**,double*,int,int);

double sigmoid(double);

double tanh(double);
///*******************************************************************




///***********      Fonction pour la Retro-Propagation    ************
void retroPropagationGradiant(double **,double ***, int, int, double **, double *,
                         int,double);

///Classification
void classificationKroneckerOutputLayer(double**,double**,double*,int,int);

void classificationKroneckerHiddenLayer(double**,double**,double***,int,int,int,double*);

void classificationUpdateWeight(double***,double,double**,double**,int,int,int);


///Regression
void regressionKroneckerOutputLayer(double**,double**,double *,int,int);

void regressionKroneckerHiddenLayer(double**,double**,double***,int,int,int,double*);

void regressionUpdateWeight(double***,double,double**,double**,int,int,int);
///*******************************************************************