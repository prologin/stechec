#! /bin/sh

# Draw tokens for a TBT team.

usage="Usage: $0 <output file> <team abbrev.> <player abbrev.> [<player abbrev.> ...]"

if [ $# -lt 3 ]
then
  echo "$usage"
  exit 1
fi

file=$1
team=$2
nb_players=$(expr $# - 2)

rm -v $file
touch $file

size=40
middle=$(expr $size / 2)

tmpb="temp_b_$file"
tmpr="temp_r_$file"

i=1
while [ "$i" -le "$nb_players" ]
do
  player=$(eval echo \$$(expr $i + 2))
  label="$team $player"

  convert \
    -size "${size}x${size}" xc:none \
    -draw "fill blue fill-opacity 0.8 circle $middle,$middle,$middle,2" \
    -gravity center -font Helvetica-Bold -pointsize 12 \
    -draw "fill black text 0,0 '$team:$player'" \
    "$tmpb"

  convert \
    -size "${size}x${size}" xc:none \
    -draw "fill red fill-opacity 0.8 circle $middle,$middle,$middle,2" \
    -gravity center -font Helvetica-Bold -pointsize 12 \
    -draw "fill black text 0,0 '$team:$player'" \
    "$tmpr"

  montage -background None -geometry +0+0 "$file" "$tmpb" "$tmpr" "$file"
  rm "$tmpb" "$tmpr"

  ((i++))
done

