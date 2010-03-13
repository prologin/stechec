cd _build/prologin
#mv Makefile save

path="\/opt\/local\/lib\/ocaml\/site-lib"
cp Makefile save
cat save | \
  gsed -e "s/\(+cryptokit\)/$path\/cryptokit/g" | \
  gsed -e "s/\(+ssl\)/$path\/ssl/g" | \
  gsed -e "s/\(+zip\)/$path\/zip/g" > Makefile
make

cp prologin.exe ../../
