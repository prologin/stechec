wait
# Choose to kick off the ball.
choose kickoff

wait
# Team placement.
place end

wait
# Kick-off.
print BEGIN DIFF
print hello world from 2
print END DIFF
cheat 1 1 1
kickoff 4 4

wait
# Turn 1, not reached.
print BEGIN DIFF
print This should not be printed, in any case.
print END DIFF
