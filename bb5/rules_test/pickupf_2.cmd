wait
# Choose to kick off the ball.
choose kickoff

wait
# Team placement.
place end

wait
# Kick-off.
# Ball is scattered on player 10, who fails to catch it.
# Ball finally rebounds to 6 8.
cheat 1 1 1 1
kickoff 10 8

print BEGIN DIFF
print global
print END DIFF

wait
# Turn 1.
# Do nothing.
end

wait
# Turn 2, never reached.
print This should not be printed, in any case.
