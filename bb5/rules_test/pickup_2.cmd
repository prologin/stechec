wait
# Choose to kick off.
choose kickoff

wait
#Team placement.
place end

wait
# Kickoff.
# Ball
cheat 1 1 1 1
kickoff 10 8

print BEGIN DIFF
print global
print END DIFF

wait
# Turn 1, never reached.
print This should not be printed, in any case.
