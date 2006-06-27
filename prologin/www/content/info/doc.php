<?php
/*

    ACU Intranet project is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    ACU Intranet project is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the ACU Intranet project; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Copyright ACU EPITA Promo 2004

	* Becoulet Alexandre <becoul_a@epita.fr>
	* Bizon Maxime <bizon_m@epita.fr>

*/

#include "include/style.php"

style_title("Comment ca marche ? Petit mode d'emploi");

?>

<pre>
Allez dans la section Jeux -> Liste.
Sélectionnez 'fourmis', et rendez-vous dans Jeux -> Fichiers.
Téléchargez le sujet ainsi que la tarball 'code-v?.tar.gz'.

La tarball contient le code que vous devez remplir, ainsi que les Makefile permettant
de compiler une bibliothèque dynamique (.so). Chaque langage supporté est dans un
répertoire différent. Tous devraient fonctionner. Les Makefile ne fonctionnent
que sous un système unix pas trop exotique (Linux). Vous pouvez néanmoins coder
sous Windows, à condition de faire vous même une tarball contenant tous les
fichiers sources.

Les champions C, Pascal et Java contiennent un petit exemple de code qui prend 4
fourmis et leur fait faire des allez-retour en prenant la nourriture au passage.

Une fois que votre code vous plaît, et que vous avez envie de l'essayer sur le
serveur, utilisez la cible tar du Makefile :
$ make tar

Une fois que vous avez votre tarball, allez dans Jeux -> Champion.
Ici, donnez un nom à votre champion, et indiquez le chemin vers votre tarball.
Vous devriez le voir apparaitre dans la liste, d'abord en avancement 'Nouveau',
puis, quelques secondes plus tard, dans l'état 'Non Compilable' ou 'Validé'.

Si il est validé, allez dans la section Jeux -> Match, pour pouvoir l'essayer.
La liste de tous les champions contre lequel il est possible de se battre est indiqué.

Pour lancez un match, vous devez renseigner la case "Option du match". Les
choix disponibles sont indiquées dans Jeux -> Informations.
Voici 2 exemples :
 -map hell.map -turn 10
 -map duel.map

Dans la case "champions participant", les numéros des champions, séparés par des ':',
que vous voulez faire participer. Deux exemples :
  132
  12:15

Si vous voulez que les autres puissent utiliser votre champion, cliquez sur
'switch' dans la page Champion. Il passera alors dans l'état 'Compétition'.

Voilà, ca devrait déjà être pas mal. Si vous avez des questions, ou si vous avez
remarqué des bugs, plaignez vous dans le forum de prologin :)

Amusez vous bien !

</pre>
