wait
# Choose to kick off the ball.
choose kickoff

wait
# Team placement.
place end

wait
# Kick-off.
# Ball is scattered, rebounds and stops in 7 7.
cheat 1 1 1
kickoff 9 9

wait
# Turn 1.
# Do nothing.
end

wait
# Turn 2, not reached.
print BEGIN DIFF
print This should not be printed, in any case.
print END DIFF
