wait
# Turn 1.

# effectue toutes les actions qu'on veut...
# Move player 10 on ball's square.
# Fail to catch it.
cheat 2 5
declare move 10
move 10 6 8
accept

# Turnover.

wait
# Turn 2.

print BEGIN DIFF
print us 10
print global
print END DIFF

quit
