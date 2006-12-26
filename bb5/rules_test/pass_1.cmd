# test a simple ball pickup and throw.
# kickoff on 9,9, scatter on 7,7

wait
# player 1, turn 1

# move, and throw will a reroll.
cheat 3
declare pass 10
move 10 7 7
cheat 1 5 4
pass 10 4 4
reroll
# turnover here.

print BEGIN DIFF
print global
print END DIFF

quit
