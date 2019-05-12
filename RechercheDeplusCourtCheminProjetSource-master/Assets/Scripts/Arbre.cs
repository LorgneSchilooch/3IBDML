using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Runtime.CompilerServices;
using NUnit.Framework;
using UnityEngine;

namespace DefaultNamespace
{
    public class Arbre
    {
        /// <summary> Position de départ de l'algorithme </summary>
        Position ptStart;

        /// <summary> Position de fin de l'algorithme </summary>
        Position ptEnd;

        /// <summary> Liste des position déjà connues par l'arbre </summary>
        Dictionary<String, Sommet> dictPosition = new Dictionary<string, Sommet>();

        /// <summary> Liste des routes possibles </summary>
        List<Sommet> solutions = new List<Sommet>();

        /// <summary> Image du terrain </summary>
        bool[][] terrain;
        
        // On va récupérer le chemin utilisé par la solution optimale pour affichage
        public List<Sommet> cheminOptimal = new List<Sommet>();
        
        /// <summary> Premier Sommet</summary>
        int sommetNameIndice = 1;
        
        /// <summary>Tableau de direction (voir pathSolutionScript pour les direction)</summary>
        /// <summary> direction[0,1,2,3] = Gauche,Droite,Haut,Bas </summary>
        private int[] direction;
        
        /// <summary> Nombre de fois que l'on a appelé la fonction chercheFilsAtPosition chercheFilsAtPosition</summary>
        int nbCallSearchSonAtPosition = 0;
        
        /// <summary> Nombre de fois que l'on a appelé la fonction chercheFilsAtPosition
        /// pour vérifier que la position n'est pas déjà couverte par les autres chemins</summary>
        int nbCheckPositionNonDecouvert = 0;
        
        /// <summary> Chemin d'enregistrement des resultats </summary>
        private String Arbre_N_Air = "Resultats/Problem5/Arbre_N_Air.txt";
        private String Arbre_N_AirBis = "Resultats/Problem5/Arbre_N_AirBis.txt";
        private String VerificationFils = "Resultats/Problem5/VerificationFils.txt";
        private String ChercheSolution = "Resultats/Problem5/ChercheSolution.txt";
        private String Djikstra_PlusCourtChemin = "Resultats/Problem5/Djikstra_PlusCourtChemin.txt";
        private String Djikstra_PlusCourtCheminBis = "Resultats/Problem5/Djikstra_PlusCourtCheminBis.txt";
            
        //Constructeur 1
        public Arbre(Position ptStart, Position ptEnd, bool[][] terrain)
        {
            this.ptStart = ptStart;
            this.ptEnd = ptEnd;
            this.terrain = terrain;
            

            // On crée le premier sommet au démarrage de l'algo
            Sommet premierSommet = new Sommet(this, null, ptStart, 0, -1, 0);
            dictPosition.Add(ptStart.posX + "-" + ptStart.posY, premierSommet);

            // On appelle la fonction recursive qui va aller chercher toutes les solutions possible pour la route
            chercheFils(new List<Sommet> { premierSommet }, 1);

            // Appel de l'algorithme du plus court-chemin : On cherche la solution optimale et on l'affiche
           chercheSolutionOpti();
           
        }

        /// <summary> Fonction recherchant les sommet fils d'une liste de sommets </summary>
        /// <param name="sommetBase"> Liste des sommets de base pour lesquels on recherche les fils </param>
        /// <param name="nbStep"> Nombre d'étape déjà réalisée par les différents chemins </param>
        private void chercheFils(List<Sommet> sommetBase, int nbStep)
        {
            List<Sommet> sommetFils = new List<Sommet>();

            // On parcourt les sommet de base
            foreach (Sommet sommet in sommetBase)
            {
                // On cherche les fils dans les 4 position pour le sommet
                chercheFilsAtPosition(sommet, sommet.pos.getPositionUp(), sommetFils,2, nbStep);
                chercheFilsAtPosition(sommet, sommet.pos.getPositionDown(), sommetFils,3, nbStep);
                chercheFilsAtPosition(sommet, sommet.pos.getPositionRight(), sommetFils,1, nbStep);
                chercheFilsAtPosition(sommet, sommet.pos.getPositionLeft(), sommetFils,0, nbStep);
                
                //Ecriture dans un fichier
                using (StreamWriter file = new StreamWriter(Arbre_N_Air, true))
                {
                    file.WriteLine(sommet.nbStep + " : " + "("+ sommet.pos.posX + " , " + sommet.pos.posY +")" +  " - " +  sommet.direction);
                    file.Flush();
                    file.Close();
                }
                
                //Ecriture dans le fichier pour Excel
                using (StreamWriter file = new StreamWriter(Arbre_N_AirBis, true))
                {
                    file.WriteLine(sommet.nbStep + ";"+ sommet.pos.posX + ";" + sommet.pos.posY +";"+  sommet.direction);
                    file.Flush();
                    file.Close();
                }
            }
            using (StreamWriter file = new StreamWriter(Arbre_N_Air, true))
            {
                file.WriteLine("\t");
                file.Flush();
                file.Close();
            }
            
           
            // Si ce sommet n'a plus aucun fils valide, on sort de la fonction récursive
            if (sommetFils.Count == 0) return;

            // On va chercher les enfants des enfants que l'on vient de trouver
            chercheFils(sommetFils, nbStep+1);
            
        }

        /// <summary> Fonction qui va chercher si une position est valide pour un fils </summary>
        /// <param name="sommetBase"> Sommet parent </param>
        /// <param name="pos"> Position du nouvel objet </param>
        /// <param name="sommetFils"> Objets enfants (pour ajout de l'objet si il est valide) </param>
        /// <param name="direction"> Direction prise pour acceder a ce nouveau fils </param>
        /// <param name="nbStep"> Nombre de step déjà réalisé par le chemin en cours </param>
        private void chercheFilsAtPosition(Sommet sommetBase, Position pos, List<Sommet> sommetFils, int direction, int nbStep)
        {
            // Controle de bordure pour eviter de sortir du terrain
            if (pos.posX < 0) return;
            if (pos.posY < 0) return;
            if (pos.posY >= terrain.Length) return;
            if (pos.posX >= terrain[0].Length) return;

            // Si la case du terrain est non utilisable, on ne retient pas ce chemin
            if (terrain[pos.posX][pos.posY]) return;

            // On vérifie que la position n'est pas déjà couverte par les autres chemins
            if (!dictPosition.ContainsKey(pos.ToString()))
            {    
                //Envoie le nombre de vérification dans un fichier
                using (StreamWriter file = new StreamWriter(VerificationFils, true))
                {
                    file.WriteLine("Nb fois de verification : " + nbCheckPositionNonDecouvert);
                    file.Flush();
                    file.Close();
                }
                // On crée un nouvel objet sommet et on l'ajoute au sommet parent
                Sommet som = new Sommet(this, sommetBase, pos, sommetNameIndice, direction, nbStep);
                sommetBase.listChilds.Add(som);
               
                // Le sommet est il à la position finale ?
                if (pos.posX == ptEnd.posX && pos.posY == ptEnd.posY)
                {
                    // Si oui on l'ajoute à la liste des solutions possibles
                    solutions.Add(som);
                }
                else
                {
                    // Sinon on l'ajoute à la liste des fils en cours, et au dictionnaire pour notifier que la case est decouverte
                    sommetFils.Add(som);
                    dictPosition.Add(pos.ToString(), som);
                }

                nbCheckPositionNonDecouvert++;
            }
            
            //stock le Nb de fois que l'on appelé la fonction chercheFilsAtPosition
            nbCallSearchSonAtPosition++;
            
            //Envoie le nombre de fois que l'on a appelé le sous programme de recherche
            using (StreamWriter file = new StreamWriter(ChercheSolution, true))
            {
                file.WriteLine("Nb fois appel de la fonction : " + nbCallSearchSonAtPosition);
                file.Flush();
                file.Close();
            }

            Console.ReadLine();
        }
        
        /// <summary>Algorithme du plus court-chemin</summary>>
        /// <summary> onction allant chercher les solutions trouvées pour determiner la solution optimale et l'afficher </summary>
        private void chercheSolutionOpti()
        {

            Sommet solutionOpti = null;
            int maxStep = 5000;

            // On parcourt les solutions pour trouver celle qui prends le moins d'étape
            foreach (Sommet som in solutions)
            {
                if (som.nbStep < maxStep)
                {
                    solutionOpti = som;
                    maxStep = som.nbStep;
                }
            }

            // On va récupérer le chemin utilisé par la solution optimale pour affichage
        
            solutionOpti.getChemin(cheminOptimal);
           
            foreach (Sommet som in cheminOptimal)
            {
                Debug.Log(som.nbStep + " : " + "("+ som.pos.posX + " , " + som.pos.posY +")" +  " - " +  som.direction);
                
                //Ecriture dans un fichier
                using (StreamWriter file = new StreamWriter(Djikstra_PlusCourtChemin, true))
                {
                    file.WriteLine(som.nbStep + " : " + "("+ som.pos.posX + " , " + som.pos.posY +")" +  " - " +  som.direction);
                    file.Flush();
                    file.Close();
                }
                
                //Ecriture dans le fichier pour le Excel
                using (StreamWriter file = new StreamWriter(Djikstra_PlusCourtCheminBis, true))
                {
                    file.WriteLine(som.nbStep + ";" + som.pos.posX + ";" + som.pos.posY + ";" +  som.direction);
                    file.Flush();
                    file.Close();
                }
            }
        }
    }
}
