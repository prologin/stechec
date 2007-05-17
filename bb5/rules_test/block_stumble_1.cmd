wait
# Team placement.
place end

wait
# Turn 1.

# Block a player, follow.
cheat 4 # Blocked player will be stumbled.
cheat 6 6 6 6 1
declare block 0
block 0 0
accept

wait
# Opponent chooses to NOT use the Dodge skill.
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
