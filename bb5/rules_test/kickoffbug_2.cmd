wait
cheat 1 7 6
kickoff 12 7
print BEGIN DIFF
print global
print END DIFF
wait

# Bug d'engagement.
# Quand le ballon n'atterit pas, apr�s dispersion, dans la zone du joueur qui re�oit, il doit y avoir "Touchback".
# Ici, le ballon �tait envoy� dans le camp adverse, mais dispers� vers la premi�re ligne de l'envoyeur qui arrivait � conserver la balle.
