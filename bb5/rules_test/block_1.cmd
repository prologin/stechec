wait
# Turn 1.

# Block a player, follow.
# Blocked player will be injured.
cheat 5
cheat 6 6 6 6 1
declare block 0
block 0 0
accept
block push 2
block follow

# Blocked player should be injured and out of the field.
print BEGIN DIFF
print players
print END DIFF

# End the turn.
end

wait
# Turn 2, never reached.
print This should not be print, in any case.
