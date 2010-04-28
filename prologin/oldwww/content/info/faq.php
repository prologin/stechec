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

?>

<?php
style_title("Pourquoi mon champion ou la partie reste dans l'état \"nouveau\"");
?>

<pre>

Cela indique qu'il y a un problème dans la machinerie derrière le site web. Il n'y 
a réellement pas grand chose que vous puissiez faire, contactez nous pour que nous
résolvions le problème.
</pre>

<?php
style_title("Que contient le \"Log du Champion\"");
?>

<pre> 
Ce log contient la sortie standard et d'erreur de votre code. On y ajoute
juste quelques renseignements (commencant toujours par [Client]) utiles.
</pre>

<?php
style_title("Qu'est-ce que le \"log visio\"");
?>

<pre> 
C'est un fichier qui contient toutes les informations d'un match afin de
pourvoir le re-visionner plus tard avec une interface graphique.

Pour l'instant, cette fonctionalité est desactivé, vous n'aurez que des echec
si vous essayez de telecharger ce fichier.
</pre>
<!--
Ce fichier est _binaire_, c'est à dire que si vous essayez de l'ouvrir
avec un éditeur texte, vous verrez n'importe quoi. Des fois, des caractères
ascii bizarres formant un mot s'affichent, ce n'est que la partie visible
d'optimisations internes faisant appels à des algorithmes compliqués.
-->


<?php
style_title("Les matchs que je lance restent déséspérément dans l'état 'erreur'. Que faire ?")
?>

<pre>
Cela indique que, pour une raison ou une autre, le serveur n'a pas réussi
à faire jouer les champions. Cliquez sur "détails", et regardez tour à tour
les "Logs Serveur" et "Log du champion". En général, les raisons peuvent être
les suivantes:

[Ant-rules] Map isn't specified. Try ... -- -h 
 => Vous n'avez pas indiqué '-map une_carte' dans les options du match. Cette
    option est obligatoire. 

---
[Ant-rules] Can't open map `chiche.map': No such file or directory 
  => Vérifiez dans Jeux -> Informations que la carte existe bien.

---
[Server] Timeout: no connection since 10 seconds.
 => Il faut aller voir les logs des champions, un problème est arrivé à l'un d'eux.

[...]/fourmis.so: undefined symbol: init_game
 => Le champion que vous avez soumis ne contient pas de fonction 'init_game'.
    Elle est obligatoire, rajoutez là et ré-uploadez votre champion (pareil
    pour la fonction new_turn).

--

</pre>


