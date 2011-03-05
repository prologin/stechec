wait
# Team placement.
place end

wait
# Turn 1.

# Move a player, so that he uses a team reroll.
cheat 1 6 6
declare move 2
move 2 14 3
wait
reroll
wait
accept

# Block with another player, to push someone.
cheat 3
declare block 3
block 3 3
block push 1

# A bug happened here, server crashed or pushed a wrong player.

block follow

# End the turn.
end

wait
# Turn 2, not reached.
print BEGIN DIFF
print This should not be printed, in any case.
print END DIFF
