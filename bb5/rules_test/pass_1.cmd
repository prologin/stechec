wait
# Team placement.
place end

wait
# Turn 1.

# Move and pick up the ball.
cheat 3
declare pass 10
move 10 7 7
wait
accept

# Throw the ball to an empty square.
# Fail first, causing a reroll.
cheat 1 5 4
pass 10 4 4
wait
reroll

# Normally, there should be a turnover here, because the ball lands in an empty square.

wait
# Turn 2.

print BEGIN DIFF
print global
print END DIFF

quit
