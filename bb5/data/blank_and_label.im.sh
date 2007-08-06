#! /bin/sh

# Replace an image by a label featuring filename and image size.

usage="Usage: $0 <image file> [<label>]
  label can use %f for filename, %h for height and %w for width."

if [ $# -ne 1 -a $# -ne 2 ]
then
  echo "$usage"
  exit 1
fi

file=$1

if [ ! -r "$file" ]
then
  echo "File '$file' is not readable."
  echo "$usage"
  exit 1
fi

if [ ! -w "$file" ]
then
  echo "File '$file' is not writable."
  echo "$usage"
  exit 1
fi

bw=3
iw=$(expr $(identify $file | sed 's/^.* \([0-9]*\)x.*$/\1/') "-" "(" "$bw" "*" "2" ")")
ih=$(expr $(identify $file | sed 's/^.*x\([0-9]*\) .*$/\1/') "-" "(" "$bw" "*" "2" ")")

default_label='%f\n-\n%wx%h'
label="${2:-$default_label}"

label="${label//\%f/$file}"
label="${label//\%h/$ih}"
label="${label//\%w/$iw}"

rm -v "$file"

convert \
  -background white \
  -border $bw \
  -bordercolor black \
  -fill black \
  -gravity center \
  -size "${iw}x${ih}" \
  label:"$label" \
  $file

