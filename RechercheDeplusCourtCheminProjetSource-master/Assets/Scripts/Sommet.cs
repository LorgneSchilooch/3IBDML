using System.Collections.Generic;

namespace DefaultNamespace
{
    public class Sommet
    {
        public Arbre arbre;

        public Sommet parent;

        public Position pos;

        public int sommetName;

        public int direction;

        public List<Sommet> listChilds = new List<Sommet>();

        public int nbStep;

        public Sommet(Arbre arbre, Sommet parent, Position pos, int sommetName, int direction, int nbStep)
        {
            this.arbre = arbre;
            this.parent = parent;
            this.pos = pos;
            this.sommetName = sommetName;
            this.direction = direction;
            this.nbStep = nbStep;
        }

        public void getChemin(List<Sommet> chemins)
        {
            chemins.Insert(0, this);
            if (parent != null) parent.getChemin(chemins);
        }
    }
}
