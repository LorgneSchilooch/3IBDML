namespace DefaultNamespace
{
    /// <summary> Classe representant une position dans la grille </summary>
    public class Position
    {
        public int posX;

        public int posY;

        public Position(int posX, int posY)
        {
            this.posX = posX;
            this.posY = posY;
        }

        public Position getPositionUp() { return new Position(posX, posY-1); }

        public Position getPositionDown() { return new Position(posX, posY+1); }

        public Position getPositionRight() { return new Position(posX-1, posY); }

        public Position getPositionLeft() { return new Position(posX+1, posY); }

        override public string ToString() { return posX + "-" + posY; } 
    }
}
