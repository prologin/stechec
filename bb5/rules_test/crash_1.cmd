wait
#Team placement.
place end

wait
# Turn 1.

# Player 10 moves and catches the ball.
cheat 6
declare move 10
move 10 6 8
move 10 9 9

# Block with players 9.
cheat 5
# Blocked player is injured.
cheat 6 6 6 6 1
declare block 9
block 9 9
accept
block push 0
block follow

# Block with players 8.
cheat 5
# Blocked player is stunned.
cheat 6 6 1 1
declare block 8
block 8 8
accept
block push 0
block follow

# End of turn.
end

wait
# Turn 2.

# Move player 9.
declare move 9
move 9 18 13

# Move player 10 and pass to player 9.
declare pass 10
move 10 12 13
cheat 6 6
pass 10 18 13

# A bug happened here, causing a server crash.

print BEGIN DIFF
print global
print END DIFF

quit
