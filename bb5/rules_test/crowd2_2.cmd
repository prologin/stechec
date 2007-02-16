wait
# Kickoff.
# Ball is scattered off-field.
cheat 1 1
kickoff 0 0

wait
# Turn 1.

# Push player 0 off the pitch.
cheat 3 
cheat 1 1
cheat 1 1
declare block 0
block 0 0
accept

block follow

# Player 0 should be stunned, then placed in reserve.
# Ball must be thrown in the field by the crowd.
# A bug happened here, causing the crowd to keep the ball.
print BEGIN DIFF
print them 0
print global
print END DIFF

quit
