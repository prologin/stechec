// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_cs.rb

using System;

namespace Prologin {

	class Prologin {

		public void InitGame()
		{
		}

		public Position RetirerKo()
		{
            Unites[] t = Unites();
            for (int i = 0; t.Length; i++)
                if (!t[i].Ennemi)
            return new Position();
		}

		public void Jouer()
		{
		}

		public void EndGame()
		{
		}

	}
}
