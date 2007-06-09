#! /bin/sh

if [ -z "$1" -a -z "$2" ]
then
  echo "Usage: $0 SOURCE CIBLE"
  exit 1
fi

if [ ! -f "$1" ]
then
  echo "$0: $1 doesn't exist."
  exit 1
fi

if [ -e "$2" ]
then
  echo "$0: $2 already exists."
  exit 1
fi

convert -verbose -resize 139x147 -gravity Center -extent 139x147 "$1" "$2"

