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
            throw new Exception(); //Should not trigger
		}

        Unite getPosUnite(Position a)
        {
            Unite[] items = Unites();
            foreach (Unite t in items)
                if (cmpPos(t.Pos, a))
                    return t;

            throw new Exception(); //No unit
        }

        int distPos(Position a, Position b)
        {
            return Math.Max(Math.Abs(a.X - b.X), Math.Abs(a.Y - b.Y));
        }

        Position minDistPos(Position a, Position b, Position goalPos)
        {
            if (distPos(goalPos, a) > distPos(goalPos, b))
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
                Spawn(TypeUnite.CHAT);      //Will be available next turn
            }

            Unite uTiti = new Unite();    //Prevent warning
            foreach (Unite item in t)
                if (item.Ennemi && item.VraiTypeUnite == TypeUnite.PERROQUET)
                {
                    uTiti = item;
                    break;
                }
            bool MADE_ACTIONS = true;
            while (NombrePc() > 0 && MADE_ACTIONS)
            {
                    MADE_ACTIONS = false;
                    if (t.Length > 0)
                    {
                        Unite min = null;
                        foreach (Unite item in t)
                            if (!item.Ennemi && item.VraiTypeUnite == item.TypeUniteActuel &&
                                 item.TypeUniteActuel == TypeUnite.CHAT && item.Ko < 0 && item.Pa > 0)
                                if (min != null)
                                    min = getPosUnite(minDistPos(min.Pos, item.Pos, uTiti.Pos));  //Seeking the closest cat to ennemy's titi
                                else
                                    min = item;
                        if (min != null)
                            if (distPos(min.Pos, uTiti.Pos) == 1 && Attaquer(min.Pos, uTiti.Pos) == Erreur.OK)
                                break;  //We wone
                            else    //We have to move our cat to the closest position to titi
                            {
                                Position closest = min.Pos;
                                TailleTerrain mapbound = TailleTerrainActuelle();
                                int minx = (min.Pos.X - min.Pa) >= mapbound.MinCoord ? min.Pos.X - min.Pa : mapbound.MinCoord;
                                int minyI = (min.Pos.Y - min.Pa) >= mapbound.MinCoord ? min.Pos.Y - min.Pa : mapbound.MinCoord;

                                int maxx = (min.Pos.X + min.Pa) < mapbound.MaxCoord ? min.Pos.X + min.Pa : mapbound.MaxCoord;
                                int maxy = (min.Pos.Y + min.Pa) < mapbound.MaxCoord ? min.Pos.Y + min.Pa : mapbound.MaxCoord;
                                for (; minx < maxx; minx++)
                                    for (int miny = minyI; miny < maxy; miny++)
                                    {
                                        Position tmppos = new Position();
                                        tmppos.X = minx;
                                        tmppos.Y = miny;
                                        try
                                        {
                                            getPosUnite(tmppos); //Will throw an error in case of no Unite
                                        }
                                        catch
                                        {
                                            closest = minDistPos(closest, tmppos, uTiti.Pos);
                                        }
                                    }
                                AfficherErreur(Deplacer(min.Pos, closest));
                            }
                    }
            }
		}

		public void EndGame()
		{
		}

	}
}
