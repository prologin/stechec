# Kickoff on 0 0, scatter out of the field.

wait
# Give ball to player 0.
giveBall 0

# Turn 1, team 1.

# Player 0 blocks, pushes and follows player 1.
cheat 5
declare block 0
block 0 1
accept
block push 1
block follow

# _BUG_: Damn it, player 0 dropped his/her ball!

print BEGIN DIFF
print Note: Ball and player 0 should be at the same position.
print global
print us 0
print END DIFF

quit
