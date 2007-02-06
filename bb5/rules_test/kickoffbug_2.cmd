wait
cheat 1 7 6
kickoff 12 7
print BEGIN DIFF
print global
print END DIFF
wait

# Bug d'engagement.
# Quand le ballon n'atterit pas, après dispersion, dans la zone du joueur qui reçoit, il doit y avoir "Touchback".
# Ici, le ballon était envoyé dans le camp adverse, mais dispersé vers la première ligne de l'envoyeur qui arrivait à conserver la balle.
