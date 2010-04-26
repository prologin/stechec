// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_cs.rb

using System;

namespace Prologin {

	class Prologin : Api {

		public void InitGame()
		{
		}

        bool cmpPos(Position a, Position b)
        {
            return a.X == b.X && a.Y == b.Y;
        }

		public Position RetirerKo()
		{
            Unite[] t = Unites();
            for (int i = 0; i < t.Length; i++)
                if (!t[i].Ennemi)
                    return t[i].Pos;
            return new Position(); //Ne devrait jamais arriver
		}

		public void Jouer()
		{
            Unite[] t = Unites();
            Position posSpawn = PosSpawn(false);
            for (int i = 0; i < t.Length; i++)
                if (!cmpPos(t[i].Pos, posSpawn)) //Faux si le Spawn est occupe
                {
                }
		}

		public void EndGame()
		{
		}

	}
}
