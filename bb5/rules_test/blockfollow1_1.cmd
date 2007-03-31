wait
#Team placement.
place end

wait
# Give ball to player 0.
giveBall 0

# Turn 1, team 1.

# Player 0 blocks, pushes and follows player 1.
cheat 3
declare block 0
block 0 1
accept
block push 1
block follow

# A bug happened here: player 0 dropped his/her ball.

print BEGIN DIFF
print global
print us 0
print END DIFF

quit
