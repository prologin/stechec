wait
# Choose to kick off the ball.
choose kickoff

wait
# Team placement.
place end

wait
# Kick-off.
# Ball is scattered out of the field.
cheat 1 1
kickoff 0 0

wait
# Turn 2, not reached.
print BEGIN DIFF
print This should not be printed, in any case.
print END DIFF
