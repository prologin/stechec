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
            return new Exception(); //Should not trigger
		}

        Unite getPosUnite(Position a)
        {
            Unite[] items = Unites();
            foreach (Unite t in items)
                if (cmpPos(t.Pos, a))
                    return t;

            throw new Exception(); //No unit
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
            try
            {
                getPosUnite(PosSpawn(false));
            }
            catch
            {
                Spawn(TypeUnite.CHAT);      //Accessible au prochain tour
            }
            Unite uTiti;
            foreach (Unite item in t)
                if (item.Ennemi && item.VraiTypeUnite == TypeUnite.PERROQUET)
                {
                    uTiti = item;
                    break;
                }
            Unite min;
            if (t.Length > 0)
            {
                min = t[0];
//                foreach (Unite item in t)
//                if
            }
		}

		public void EndGame()
		{
		}

	}
}
