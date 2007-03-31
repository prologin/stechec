wait
# Choose to kick off.
choose kickoff

wait
#Team placement.
place end

wait
# Kickoff.
# Ball scattered on player 10, who fails to catch it.
cheat 1 1 1 1
kickoff 10 8

wait
# Turn 1.

# Player 0 should be injured and out of the field.
print BEGIN DIFF
print us 0
print END DIFF

quit
