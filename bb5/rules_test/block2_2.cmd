wait
# Choose to kick off the ball.
choose kickoff

wait
# Team placement.
place end

wait
# Kick-off.
cheat 1 1 1
kickoff 2 2

wait
# Turn 1, not reached.
print BEGIN DIFF
print This should not be printed, in any case.
print END DIFF
