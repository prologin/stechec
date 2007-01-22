wait
cheat 6
declare move 10
move 10 6 8
move 10 9 9

#block du joueur 9
cheat 5 6 6 6 4 3
declare block 9
block 9 9
accept
push 0
follow

#block du joueur 8
cheat 3
declare block 8
block 8 8
accept
push 0
follow

#fin du tour
end
wait

#positionnement joueur 9
declare move 9
move 9 18 13

#deplacement et passe joueur 10
declare pass 10
move 10 12 13
cheat 6 6
pass 10 13 18

#Error: Network: Connection was unexpertly closed on other side.

print BEGIN DIFF
print global
print END DIFF
