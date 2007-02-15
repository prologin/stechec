wait
# Give ball to player 0.
giveBall 0

# Turn 1, team 1.

# Player 3 blocks, pushes and follows player 2.
cheat 3
declare block 3
block 3 2
accept
block push 2
block follow

# Player 4 blocks, pushes and follows player 3.
cheat 3
declare block 4
block 4 3
accept

# A bug happened here: server crashes.
# Normally, player 3 must be pushed to the only empty square.

block follow

# Move one more player, just to make sure that server is still responsive.
declare move 10
move 10 7 7

print BEGIN DIFF
print players
print END DIFF

quit
