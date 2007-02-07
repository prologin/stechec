# test a tricky mix of reroll and failed pass.
# kickoff on 9,9, scatter on 7,7

wait
# player 1, turn 1

# block with reroll
cheat 2 5
declare block 1
block 1 1
reroll
block push 0
block stay

# pick up the ball
cheat 6
declare pass 10
move 10 7 7
# Fail to throw it.
# Ball will be scattered and rebound in empty squares.
cheat 2
cheat 1 2 4 6
pass 10 12 4

# Ball position: x:4 y:11
# Normally, there should be a turnover and the following command should be ignored.
# But a _BUG_ happens here!

# If it is still player 1 to play, move the ball and end the turn.
cheat 6 6
declare move 2
move 2 11 4
move 2 6 0
end

wait
# player 1, turn 2
# not reached
