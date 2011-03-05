wait
# Choose to kick off the ball.
choose kickoff

wait
# Team placement.
place end

wait
# Kick-off.
# Ball is scattered to the receiving team's endzone,
# and then rebounds out of the field.
# There must be a touchback.
cheat 5 # scatter length: 5
cheat 3 # scatter direction: NE
cheat 1 # rebound direction: NW
kickoff 5 5

wait
# Turn 1, not reached.
print BEGIN DIFF
print This should not be printed, in any case.
print END DIFF
