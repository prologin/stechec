wait
# Choose to kick off.
choose kickoff

wait
#Team placement.
place end

wait
# Kickoff.
# Ball is scattered on player 10, who fails to catch it.
# Ball finally rebounds to 8 6.
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
