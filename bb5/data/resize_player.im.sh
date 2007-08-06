#! /bin/sh

# Resize a player picture to the size required by TBT.

usage="Usage: $0 <input file> <output file>"

if [ $# -ne 2 ]
then
  echo "$usage"
  exit 1
fi

if [ ! -f "$1" ]
then
  echo "$0: $1 doesn't exist."
  echo "$usage"
  exit 1
fi

if [ -e "$2" ]
then
  echo "$0: $2 already exists."
  echo "$usage"
  exit 1
fi

convert -verbose -resize 139x147 -gravity Center -extent 139x147 "$1" "$2"

