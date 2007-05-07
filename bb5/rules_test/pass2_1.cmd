wait
# Team placement.
place end

wait
# Turn 1.

# Block with reroll.
cheat 2 5
declare block 1
block 1 1
reroll
block push 0
block stay

# Pick up the ball.
cheat 6
declare pass 10
move 10 7 7

# Fail to throw it.
# Ball is scattered and rebound in empty squares.
cheat 2
cheat 1 2 4 6
pass 10 12 4

# Ball position: x:4 y:11
# Normally, there should be a turnover and the following command should be ignored.
# A bug happened here, causing turn continues.

# If turn is not ended, move the ball and end the turn.
cheat 6 6
declare move 2
move 2 11 4
move 2 6 0
end

wait
# Turn 2, not reached.
print BEGIN DIFF
print This should not be printed, in any case.
print END DIFF
