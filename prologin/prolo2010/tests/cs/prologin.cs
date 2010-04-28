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
            throw new Exception("RetirerKo this should not trigger.");
		}

        Unite getPosUnite(Position a)
        {
            Unite[] t = Unites();
            foreach (Unite item in t)
                if (cmpPos(item.Pos, a))
                    return item;
            throw new Exception("Not found.");
        }

        Position minDistPost(Position a, Position b, Position goalPos)
        {
            int distA = Math.Abs(goalPos.X - a.X) + Math.Abs(goalPos.Y - a.Y);
            int distB = Math.Abs(goalPos.X - b.X) + Math.Abs(goalPos.Y - b.Y);

            if (distA > distB)
                return b;

            return a;
        }

		public void Jouer()
		{
            Unite[] t = Unites();
            try
            {
                getPosUnite(PosSpawn(false));
            }catch
            {
                Spawn(TypeUnite.CHAT);      //Accessible au prochain tour
            }

//            foreach (Unite item in t)
//                if (!item.Ennemi)
		}

		public void EndGame()
		{
		}

	}
}
