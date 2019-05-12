using UnityEngine;
using System.Linq;
using System.Collections.Generic;
using System.Collections;
using DefaultNamespace;

public class PathSolutionScript {
    ActionSolutionScript[] _actions;

    public ActionSolutionScript[] Actions
    {
        get { return _actions; }
        set { _actions = value; }
    }
    
    
    //Constructeur 1 
    public PathSolutionScript(int numActions)
    {
        _actions = new ActionSolutionScript[numActions];

        for (int i = 0; i < numActions; i++)
            _actions[i] = new ActionSolutionScript();
    }
    
    //Constructeur 2 : Surcharge avec modification sur l'appel de la fonction ActionSolutionScript()
    public PathSolutionScript(List<int> cheminOpti,int nbDePas)
    {
        _actions = new ActionSolutionScript[nbDePas];
     
        for (int i = 0; i < nbDePas; i++)
       
            //Effectue les actions pas à pas 
            _actions[i] = new ActionSolutionScript(cheminOpti[i]);
                   
   
    }
   
}
