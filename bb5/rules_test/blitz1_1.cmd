wait
# Turn 1.

# Blitz with player 0.
# Blocked player will be injured.
cheat 5
cheat 6 6 6 6 1
declare blitz 0
block 0 0
accept
block push 2
block follow
cheat 6
move 0 14 0
move 0 17 0

# Player 10 throws the ball to player 0.
cheat 6 6
declare pass 10
move 10 9 1
pass 10 17 0

print BEGIN DIFF
print global
print players
print END DIFF

quit
