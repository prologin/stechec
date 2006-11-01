# le premier wait est fait pour attendre le debut du kickoff
wait

# a ce moment, on est dans la phase kick-off
cheat 1 1 1 1
kickoff 10 8

# fait 'print global' apres le kick off
print BEGIN DIFF
print global
print END DIFF

# a ce moment, vu qu'on a fait 'kickoff', on passe en phase tour1 joueur1 
# ce wait sert a attendre patiemment le tour1 joueur2, sinon ce client quitte
# (plus de commande), et le test foire
wait

# a ce moment, tour1 joueur2

# termine son tour immediatement, on veut rien faire.
end

# a ce moment, tour2 joueur1. le joueur1 va quitter la partie, et
# nous faire quitter en meme temps.
wait

# - never reached -
