wait
# Choose to kick off the ball.
choose kickoff

wait
# Team placement.
place end

wait
# Kick-off.
# Ball must be scattered and rebound into receiver's field,
cheat 2 # scatter length 2
cheat 7 # scatter direction: SW
cheat 6 # bounce direction: S
kickoff 0 7 # aim the middle of end zone.

wait
# Turn 1, not reached.
print BEGIN DIFF
print This should not be printed, in any case.
print END DIFF
