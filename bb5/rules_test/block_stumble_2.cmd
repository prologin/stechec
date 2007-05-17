wait
# Choose to kick off the ball.
choose kickoff

wait
# Team placement.
place end

wait
# Kick-off.
# Ball is scattered and rebounds in 7 7.
cheat 1 1 1
kickoff 9 9

wait
# Choose to ignore the Dodge skill.
skill ignore SK_DODGE

wait
# Turn 1.

# Player 0 should be injured and out of the field.
print BEGIN DIFF
print us 0
print END DIFF

quit
