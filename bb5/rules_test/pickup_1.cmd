wait
# Team placement.
place end

wait
# Turn 1.

# Move and pick up the ball.
cheat 6
declare move 10
move 10 6 8
wait
accept

# Move to check that ball position changes too.
move 10 9 9

print BEGIN DIFF
print global
print END DIFF

quit
