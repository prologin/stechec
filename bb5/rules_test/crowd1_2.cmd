wait
# Choose to kick off the ball.
choose kickoff

wait
# Team placement.
place end

wait
# Kick-off.
# Ball is scattered off-field.
cheat 1 1
kickoff 0 0

wait
# Turn 1.

# Push player 0 off the pitch.
cheat 3 # Pushed.
cheat 1 1
declare block 0
block 0 0
accept

# A bug happened here, causing the server to crash.

block follow

# Player 0 should be stunned, then placed in reserve.
print BEGIN DIFF
print them 0
print END DIFF

quit
