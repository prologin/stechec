# a ce moment, on est en debut de jeu/kickoff

wait
# a ce moment, tour1 joueur1

# effectue toutes les actions qu'on veut...
cheat 2 5
declare move 10
move 10 6 8
accept
# ce dernier 'accepte' fait qu'on ne recupere pas la balle - turnover.
# a ce moment, on passe en tour1 joueur2.
# c'est comme si on avait fait 'end'.

wait
# a ce moment, tour2 joueur1

# on affiche les infos et on se casse.
print BEGIN DIFF
print us 10
print global
print END DIFF

quit
