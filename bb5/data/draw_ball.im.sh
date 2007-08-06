#! /bin/sh

# Draw a TBT ball tile.

usage="Usage: $0 <output file>"

if [ $# -ne 1 ]
then
  echo "$usage"
  exit 1
fi

convert \
  -size 40x40 xc:none \
  -gravity center \
  -font Helvetica-Bold -pointsize 14 \
  -draw "
  translate 20,20 rotate 45
  fill white stroke black ellipse 0,0 20,12 0,360
  fill black stroke none text -27,1 'ball'
  " \
  "$1"

