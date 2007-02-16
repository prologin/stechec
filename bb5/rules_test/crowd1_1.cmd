wait
# Give ball to player 10
giveBall 10

# Turn 1.

# Move player 0 near the crowd.
cheat 6
declare move 0
move 0 13 0

# Do nothing else.
end

# Drop End Turn event.
wait

wait
# Turn 2, not reached.
print This should not be printed, in any case.
