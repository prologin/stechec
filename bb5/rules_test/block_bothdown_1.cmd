wait
# Team placement.
place end

wait
# Turn 1.

# Block a player without Block skill.
# Blocked player will be injured.
cheat 2
cheat 6 6 6 6 1
declare block 2
block 2 3
accept
wait
skill use Block

# Blocked player should be injured and out of the field.
print BEGIN DIFF
print players
print END DIFF

# End the turn.
end

wait
# Turn 2, never reached.
print BEGIN DIFF
print This should not be printed, in any case.
print END DIFF
