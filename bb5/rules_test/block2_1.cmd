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
accept

# Block with another player, to push someone.
cheat 3
declare block 3
block 3 3
accept
block push 1

# A bug happened here, server crashed or pushed a wrong player.

block follow

print BEGIN DIFF
print players
print END DIFF

quit
