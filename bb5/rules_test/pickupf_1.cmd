# a ce moment, on est en debut de jeu/kickoff
wait

# a ce moment, on est au debut tour1 joueur1
# effectue toutes les actions qu'on veut...
cheat 2 5
declare move 10
move 10 6 8
accept
# ce dernier 'accepte' fait qu'on ne recupere pas la balle - turnover.
# a ce moment, on passe en tour1 joueur2.

# ici, pour que les 'print' aient le temps de s'afficher, il faut continuer
# a jouer 1 tour 'dans le vent'
print BEGIN DIFF
print us 10
print END DIFF

# toujours tour1 joueur2.
wait

# a ce moment, tour2 joueur1. on peut quitter maintenat
quit
