# kickoff classique
cheat 1 1 1
kickoff 2 2

wait
# tour 1, joueur 2

# on va simplement bouger un joueur
cheat 1 6
declare move 2
move 2 14 3
reroll

cheat 4
declare block 3
block 3 3
# la il devrait demainder reroll ou accept ? mais il ne le fait pas.
# a mon avis, le probleme est la.
# accept
block push 1

# et a ce moment... c'est le drame.
# le coredump:

# Program terminated with signal 11, Segmentation fault.
# #0  0xa6e5b550 in SPlayer::setPosition (this=0x0, pos=@0xa6e3a934, advertise_client=true) at SPlayer.cc:50
#   50        if (f_->intoField(pos_))
# (gdb) bt
# #0  0xa6e5b550 in SPlayer::setPosition (this=0x0, pos=@0xa6e3a934, advertise_client=true) at SPlayer.cc:50
# #1  0xa6e607ab in SPlayer::blockPush (this=0x80a9810, chosen_square=1) at SPlayer.cc:481
# #2  0xa6e522ac in STeam::msgBlockPush (this=0x809dc60, m=0x80a5ba8) at STeam.cc:186
# #3  0xa6e595c6 in PacketHandler<42, STeam>::handle (this=0x809de28, p=0x80a5ba8) at ./../common/Player.hh:56
