wait
# Choose to kick off the ball.
choose kickoff

wait
# Team placement.
place end

wait
# Kick-off.
# Ball
cheat 3 1 5
kickoff 10 8

print BEGIN DIFF
print global
print END DIFF

wait
# Turn 1, never reached.
print This should not be printed, in any case.
