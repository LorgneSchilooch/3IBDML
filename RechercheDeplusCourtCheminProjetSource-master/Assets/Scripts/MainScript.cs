using System;
using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using DefaultNamespace;
using UnityEngine.UI;
using Random = UnityEngine.Random;

/// <summary>
/// Classe principale � utiliser pour impl�menter vos algorithmes
/// Si vous souhaitez utiliser plusieurs scripts (1 par algorithme), 
/// vous le pouvez aussi.
/// </summary>
public class MainScript : MonoBehaviour
{

    /// <summary>
    /// Indique si un algorithme est en cours d'ex�cution
    /// </summary>
    private bool _isRunning = false;

    /// <summary>
    /// Indique si une evaluation de solution est en cours
    /// </summary>
    private bool _inSimulation = false;
    
    
    private IEnumerator coroutine;

    /// <summary>
    /// M�thode utilis�e pour g�rer les informations et 
    /// boutons de l'interface utilisateur
    /// </summary>
    public void OnGUI()
    {
        // D�marrage d'une liste de composants visuels verticale
        GUILayout.BeginVertical();

        // Affiche un bouton permettant le lancement de la recherche locale na�ve
        if (GUILayout.Button("DEMARRAGE RECHERCHE LOCALE NAIVE"))
        {
            // Le bouton est inactif si un algorithme est en cours d'ex�cution
            if (!_isRunning)
            {
                // D�marrage de la recherche locale na�ve en pseudo asynchrone
                StartCoroutine("NaiveLocalSearch");
            }
        }

        // Affiche un bouton permettant le lancement de la recherche locale na�ve
        if (GUILayout.Button("DEMARRAGE RECUIT SIMULE"))
        {
            // Le bouton est inactif si un algorithme est en cours d'ex�cution
            if (!_isRunning)
            {
                // D�marrage du recuit simul� en pseudo asynchrone
                StartCoroutine("SimulatedAnnealing");
            }
        }

        // Affiche un bouton permettant le lancement de l'algorithme g�n�tique
        if (GUILayout.Button("DEMARRAGE ALGORITHME GENETIQUE"))
        {
            // Le bouton est inactif si un algorithme est en cours d'ex�cution
            if (!_isRunning)
            {
                // D�marrage de l'algorithme g�n�tique en pseudo asynchrone
                StartCoroutine("GeneticAlgorithm");
            }
        }

        // Affiche un bouton permettant le lancement de l'algorithme de Djikstra
        if (GUILayout.Button("DEMARRAGE DJIKSTRA"))
        {
            // Le bouton est inactif si un algorithme est en cours d'ex�cution
            if (!_isRunning)
            {
                // D�marrage de l'algorithme de Djikstra en pseudo asynchrone
                coroutine = Djikstra();
                StartCoroutine(coroutine);
            }
        }

        // Affiche un bouton permettant le lancement de l'algorithme A*
        if (GUILayout.Button("DEMARRAGE A*"))
        {
            // Le bouton est inactif si un algorithme est en cours d'ex�cution
            if (!_isRunning)
            {
                // D�marrage de l'algorithme A* en pseudo asynchrone
                StartCoroutine("AStar");
            }
        }

        // Fin de la liste de composants visuels verticale
        GUILayout.EndVertical();
    }

    /// <summary>
    /// Initialisation du script
    /// </summary>
    void Start()
    {
        // Pour faire en sorte que l'algorithme puisse continuer d'�tre actif m�me
        // en t�che de fond.
        Application.runInBackground = true;
    }

    /// <summary>
    /// Impl�mentation possible de la recherche locale naive
    /// sous forme de coroutine pour le mode pseudo asynchone
    /// </summary>
    /// <returns></returns>
    public IEnumerator NaiveLocalSearch()
    {
        // Indique que l'algorithme est en cours d'ex�cution
        _isRunning = true;

        // G�n�re une solution initiale au hazard (ici une s�quence
        // de 42 mouvements)
        var currentSolution = new PathSolutionScript(42);

        // R�cup�re le score de la solution initiale
        // Sachant que l'�valuation peut n�cessiter une 
        // simulation, pour pouvoir la visualiser nous
        // avons recours � une coroutine
        var scoreEnumerator = GetError(currentSolution);
        yield return StartCoroutine(scoreEnumerator);
        float currentError = scoreEnumerator.Current;

        // Nous r�cup�rons l'erreur minimum atteignable
        // Ceci est optionnel et d�pendant de la fonction
        // d'erreur
        var minimumError = GetMinError();

        // Affichage de l'erreur initiale
        Debug.Log(currentError);

        // Initialisation du nombre d'it�rations
        int iterations = 0;

        // Tout pendant que l'erreur minimale n'est pas atteinte
        while (currentError != GetMinError())
        {
            // On obtient une copie de la solution courante
            // pour ne pas la modifier dans le cas ou la modification
            // ne soit pas conserv�e.
            var newsolution = CopySolution(currentSolution);

            // On proc�de � une petite modification de la solution
            // courante.
            RandomChangeInSolution(newsolution);

            // R�cup�re le score de la nouvelle solution
            // Sachant que l'�valuation peut n�cessiter une 
            // simulation, pour pouvoir la visualiser nous
            // avons recours � une coroutine
            var newscoreEnumerator = GetError(newsolution);
            yield return StartCoroutine(newscoreEnumerator);
            float newError = newscoreEnumerator.Current;

            // On affiche pour des raisons de Debug et de suivi
            // la comparaison entre l'erreur courante et la
            // nouvelle erreur
            Debug.Log(currentError + "   -   " + newError);

            // Si la solution a �t� am�lior�e
            if (newError <= currentError)
            {
                // On met � jour la solution courante
                currentSolution = newsolution;

                // On met � jour l'erreur courante
                currentError = newError;
            }

            // On incr�mente le nombre d'it�rations
            iterations++;

            // On rend la main au moteur Unity3D
            yield return 0;
        }

        // Fin de l'algorithme, on indique que son ex�cution est stopp�e
        _isRunning = false;

        // On affiche le nombre d'it�rations n�cessaire � l'algorithme pour trouver la solution
        Debug.Log("CONGRATULATIONS !!! Solution Found in " + iterations + " iterations !");
    }
    
    
    
    /*--------------------------------------------------------------------
     * Algorithme de Djikstra : 
     * -------------------------------------------------------------------
     */
  
    // Coroutine a utiliser pour implementer l'algorithme de Djikstra
    public IEnumerator Djikstra()
    {    
        
        //Partie Simulation
        
        // Indique que l'algorithme est en cours d'ex�cution
        _isRunning = true;
        
        // Recuperation de l'environnement sous forme de matrice
        var matrix = MatrixFromRaycast.CreateMatrixFromRayCast();

        bool[][] booleanGrid = new bool[matrix.Length][];
        
        // Conversion de la grille proposee par le probleme en grille booleenne (case vide / obstacle)
        for (int i = 0; i < matrix.Length; i++)
        {
            booleanGrid[i] = new bool[matrix[i].Length];
            for (int j = 0; j < matrix[i].Length; j++)
            {
                booleanGrid[i][j] = (matrix[i][j] == LayerMask.NameToLayer("Obstacle")) ? true : false;
            }
        }

        // Récupération des positions de depart et d'arrivee
        var startPosX = PlayerScript.StartXPositionInMatrix;
        var startPosY = PlayerScript.StartYPositionInMatrix;
        var endPosX = PlayerScript.GoalXPositionInMatrix;
        var endPosY = PlayerScript.GoalYPositionInMatrix;
        
       
        //Création de l'arbre
        Arbre arbre = new Arbre(new Position(startPosX, startPosY), new Position(endPosX, endPosY), booleanGrid);
        List<int> cheminOpti = new List<int>();
        int nbDePas =0 ;
            
        
        //Appel de la fonction pour le plus court chemin
        foreach (Sommet etapeSommet in arbre.cheminOptimal)
        {
            cheminOpti.Add(etapeSommet.direction);
        }
            
        //Suppression de la case de départ pour le nombre de pas
        cheminOpti.RemoveAt(0);
            
        //Boucle qui recupère le nombre de pas
        foreach (int interator in cheminOpti)
        {
            Console.WriteLine(interator);
            nbDePas++;
        }
        Console.WriteLine();
        Console.WriteLine(nbDePas);
       
        // Génére une solution initiale au hazard (ici une séquence
        // de 42 mouvements)
        var currentSolution = new PathSolutionScript(cheminOpti,nbDePas);
        
        // Récupère le score de la solution initiale
        // Sachant que l'évaluation peut nécessiter une 
        // simulation, pour pouvoir la visualiser nous
        // avons recours à une coroutine
        var scoreEnumerator = GetError(currentSolution);
          
        //Deuxième relance : Rappel de la fonction Djikstra
        
        yield return StartCoroutine(scoreEnumerator);
    }
    
    /*--------------------------------------------------------------------
     *
     *
     *
     *
     * 
     */
    
    
    // Coroutine � utiliser pour impl�menter l'algorithme d' A*
    public IEnumerator AStar()
    {
        //TODO
        yield return null;
    }

    // Coroutine � utiliser pour impl�menter l'algorithme du recuit simul�
    public IEnumerator SimulatedAnnealing()
    {
        //TODO
        yield return null;
    }

    // Coroutine � utiliser pour impl�menter un algorithme g�n�tique
    public IEnumerator GeneticAlgorithm()
    {
        //TODO
        yield return null;
    }

    /// <summary>
    /// Exemple d'erreur minimum (pas forc�ment toujours juste) renvoyant
    /// la distance de manhattan entre la case d'arriv�e et la case de d�part.
    /// </summary>
    /// <returns></returns>
    int GetMinError()
    {
        return (int)(Mathf.Abs(PlayerScript.GoalXPositionInMatrix - PlayerScript.StartXPositionInMatrix) +
            Mathf.Abs(PlayerScript.GoalYPositionInMatrix - PlayerScript.StartYPositionInMatrix));
    }

    /// <summary>
    /// Exemple d'oracle nous renvoyant un score que l'on essaye de minimiser
    /// Ici est utilisé la position de la case d'arrivée, la position finale
    /// atteinte par la solution. Il est recommandé d'essayer plusieurs oracles
    /// pour étudier le comportement des algorithmes selon la qualitée de ces
    /// derniers
    /// 
    /// Parmi les paramètres pouvant être utilisés pour calculer le score/erreur :
    /// 
    ///  - position de la case d'arrivée    : PlayerScript.GoalXPositionInMatrix
    ///                                       PlayerScript.GoalYPositionInMatrix
    ///  - position du joueur               : player.PlayerXPositionInMatrix
    ///                                       player.PlayerYPositionInMatrix
    ///  - position de depart du joueur     : PlayerScript.StartXPositionInMatrix
    ///                                       PlayerScript.StartYPositionInMatrix
    ///  - nombre de cases explorées        : player.ExploredPuts
    ///  - nombre d'actions exécut�es       : player.PerformedActionsNumber
    ///  - vrai si le la balle a touché la case d'arrivée : player.FoundGoal
    ///  - vrai si le la balle a touché un obstacle : player.FoundObstacle
    ///  - interrogation de la matrice      :
    ///       � la case de coordonn�e (i, j) est elle un obstacle (i et j entre 0 et 49) :
    ///           player.GetPutTypeAtCoordinates(i, j) == LayerMask.NameToLayer("Obstacle")
    ///       � la case de coordonn�e (i, j) est elle explor�e (i et j entre 0 et 49) :
    ///           player.GetPutTypeAtCoordinates(i, j) == 1
    ///       � la case de coordonn�e (i, j) est elle inexplor�e (i et j entre 0 et 49) :
    ///           player.GetPutTypeAtCoordinates(i, j) == 0
    /// </summary>
    /// <param name="solution"></param>
    /// <returns></returns>
    IEnumerator<float> GetError(PathSolutionScript solution)
    {
        // On indique que l'on s'apprète à lancer la simulation
        _inSimulation = true;

        // On cree notre objet que va exécuter notre séquence d'action
        var player = PlayerScript.CreatePlayer();

        // Pour pouvoir visualiser la simulation (moins rapide)
        //A PASSER EN TRUE OU FALSE SELON NOS BESOIN
        player.RunWithoutSimulation = false;

        // On lance la simulation en specifiant
        // la sequence d'action a executer
        player.LaunchSimulation(solution);

        // Tout pendant que la simulation n'est pas terminée
        while (player.InSimulation)
        {
            // On rend la main au moteur Unity3D
            yield return -1f;
        }

        // Calcule la distance de Manhattan entre la case d'arrivée et la case finale de
        // notre objet, la pondere (la multiplie par zéro si le but a été trouvé) 
        // et ajoute le nombre d'actions jouées
        var error = (Mathf.Abs(PlayerScript.GoalXPositionInMatrix - player.PlayerXPositionInMatrix)
            + Mathf.Abs(PlayerScript.GoalYPositionInMatrix - player.PlayerYPositionInMatrix))
            * (player.FoundGoal ? 0 : 100) +
            player.PerformedActionsNumber;

        Debug.Log(player.FoundGoal);

        // Détruit  l'objet de la simulation
        Destroy(player.gameObject);

        // Renvoie l'erreur précédemment calculée
        yield return error;

        // Indique que la phase de simulation est terminée
        _inSimulation = false;
    }

    /// <summary>
    /// Execute un changement aléatoire sur une solution
    /// ici, une action de la séquence est tirée au hasard et remplacée
    /// par une nouvelle au hasard.
    /// </summary>
    /// <param name="sol"></param>
    public void RandomChangeInSolution(PathSolutionScript sol)
    {
        sol.Actions[Random.Range(0, sol.Actions.Length)] = new ActionSolutionScript();
    }

    /// <summary>
    /// Fonction utilitaire ayant pour but de copier
    /// dans un nouvel espace mémoire une solution
    /// </summary>
    /// <param name="sol">La solution est copier</param>
    /// <returns>Une copie de la solution</returns>
    public PathSolutionScript CopySolution(PathSolutionScript sol)
    {
        // Initialisation de la nouvelle s�quence d'action
        // de la même longueur que celle que l'on souhaite copier
        var newSol = new PathSolutionScript(sol.Actions.Length);

        // Pour chaque action de la s�quence originale,
        // on copie le type d'action.
        for (int i = 0; i < sol.Actions.Length; i++)
        {
            newSol.Actions[i].Action = sol.Actions[i].Action;
        }

        // Renvoi de la solution copi�e
        return newSol;
    }
    
    //------------------------------------------------------------------
}




