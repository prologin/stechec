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
            foreach (Unite item in t)
                if (!item.Ennemi)
                    return item.Pos;
            return new Position(); //Ne devrait jamais arriver
		}

        Unite getPosUnite(Position a)
        {
            foreach (Unite item in t)
                if (cmpPos(t.Pos, a))
        }

        Position minDistPost(Position a, Position b, Position, Position goalPos)
        {
            Position tmp;
            int distA = Math.Abs(goalPos.X - a.X) + Math.Abs(goalPos.Y - a.Y);
            int distB = Math.Abs(goalPos.X - b.X) + Math.Abs(goalPos.Y - b.Y);

            if (distA > distB)
                return b;

            return a;
        }

		public void Jouer()
		{
            Unite[] t = Unites();
            Position posSpawn = PosSpawn(false);
            

            foreach (Unite item in t)
                if (!cmpPos(t[i].Pos, posSpawn)) //Faux si le Spawn est occupe
                    Spawn(TypeUnite.CHAT);      //Accessible au prochain tour
            foreach (Unite item in t)
                if (!item.Ennemi)
                
		}

		public void EndGame()
		{
		}

	}
}
