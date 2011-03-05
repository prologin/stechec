#! /bin/sh

# Draw a TBT playground and reserve, depending of the weather (0 to 4).

usage="Usage: $0 <0-4>"

if [ $# -ne 1 ]
then
  echo "$usage"
  exit 1
fi

case "$1" in
  "0") bg_color="rgb(90,180,20)";;
  "1") bg_color="rgb(220,230,10)";;
  "2") bg_color="rgb(210,140,10)";;
  "3") bg_color="rgb(110,100,30)";;
  "4") bg_color="rgb(190,220,140)";;
  *)
    echo "$usage"
    exit 1
    ;;
esac

set -e -x

# Generate playground
convert \
  -size 723x1054 xc:"$bg_color" \
  -fill none \
  -stroke white \
  -strokewidth 5 \
  -draw "
    stroke-dasharray 19 2 19 0
    path 'M   7,    7   L   7, 1047'
    path 'M   7,    7   L 607,    7'
    path 'M 607,    7   L 607, 1047'
    path 'M 7  , 1047   L 607, 1047'

    stroke-dasharray 18 4 18 0
    path 'M   7,  527   L 607,  527'
  " \
  -strokewidth 4 \
  -draw "
    stroke-dasharray 16 8 16 0
    path 'M   7,   47   L 607,   47'
    path 'M   7, 1007   L 607, 1007'
  " \
  -strokewidth 3 \
  -draw "
    stroke-dasharray 14 12 14 0
    path 'M 167,   47   L 167, 1007'
    path 'M 447,   47   L 447, 1007'
  " \
  -fill gray \
  -stroke black \
  -strokewidth 10 \
  -draw "
    rectangle 629,40 711,138
    rectangle 629,148 711,246
    rectangle 629,256 711,354

    rectangle 629,698 711,796
    rectangle 629,806 711,904
    rectangle 629,914 711,1012
  " \
  -font Helvetica-Bold -pointsize 20 \
  -stroke white -strokewidth 2 \
  -draw "gravity center translate 309 -438 rotate 90 text 0,0 'Ejected'" \
  -draw "gravity center translate 309 -330 rotate 90 text 0,0 'OUT'" \
  -draw "gravity center translate 309 -222 rotate 90 text 0,0 'KO'" \
  -draw "gravity center translate 309 222 rotate 90 text 0,0 'KO'" \
  -draw "gravity center translate 309 330 rotate 90 text 0,0 'OUT'" \
  -draw "gravity center translate 309 438 rotate 90 text 0,0 'Ejected'" \
  -channel RGBA -blur 0x2 \
  -stroke none -fill black \
  -draw "gravity center translate 309 -438 rotate 90 text 0,0 'Ejected'" \
  -draw "gravity center translate 309 -330 rotate 90 text 0,0 'OUT'" \
  -draw "gravity center translate 309 -222 rotate 90 text 0,0 'KO'" \
  -draw "gravity center translate 309 222 rotate 90 text 0,0 'KO'" \
  -draw "gravity center translate 309 330 rotate 90 text 0,0 'OUT'" \
  -draw "gravity center translate 309 438 rotate 90 text 0,0 'Ejected'" \
  playground_${1}.jpg

# Generate reserve 619,365
convert \
  playground_${1}.jpg \
  -fill gray \
  -stroke black \
  -strokewidth 3 \
  -draw "
    stroke-dasharray 17 6 17 0
    rectangle 629,374 711,496
    rectangle 629,556 711,678
  " \
  -font Helvetica-Bold -pointsize 24 \
  -stroke white -strokewidth 2 \
  -draw "gravity center translate 309 -90 rotate 90 text 0,0 'Reserve'" \
  -draw "gravity center translate 309 90 rotate 90 text 0,0 'Reserve'" \
  -channel RGBA -blur 0x2 \
  -stroke none -fill black \
  -draw "gravity center translate 309 -90 rotate 90 text 0,0 'Reserve'" \
  -draw "gravity center translate 309 90 rotate 90 text 0,0 'Reserve'" \
  -crop 104x324+619+365 \
  reserve_${1}.jpg

