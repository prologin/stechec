wait
# Team placement.
place end

wait
# Turn 1.

# Move player 10 on ball's square.
cheat 2 # Fail to pick up the ball.
cheat 4 # Rebound to 7 8
declare move 10
move 10 6 8
wait
accept

# Turnover.

wait
# Turn 2.

print BEGIN DIFF
print us 10
print global
print END DIFF

quit
