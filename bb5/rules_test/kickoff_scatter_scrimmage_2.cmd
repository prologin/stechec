wait
# Choose to kick off the ball.
choose kickoff

wait
# Team placement.
place end

wait
# Kick-off.
# Ball is scattered to kicking team's line of scrimmage.
# There must be a touchback.
cheat 1 # scatter length: 1
cheat 7 # scatter direction: SW
cheat 6 # success to eventually catch the ball
kickoff 12 7

wait
# Turn 1, not reached.
print BEGIN DIFF
print This should not be printed, in any case.
print END DIFF
