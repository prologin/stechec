// Editor.hpp -- The Prologin 2008 Map Editor

#ifndef EDITOR_HPP_
# define EDITOR_HPP_

// This *MUST* match the sprite's size (both width and height).
#define GRID_SIZE	64

#define DEFAULT_XSIZE	10
#define DEFAULT_YSIZE	10

// The characted used to represent an empty case
#define GROUND_CHAR	'.'

// The color of the selected case's rectangle.
#define RECT_COLOR	0x0000ff00

#define DEFAULT_FILENAME	"default.map"

#define MAP_MAX_X	20
#define MAP_MAX_Y	20

#define USAGE	"usage: editor [-c] [-g WxH] [-f file]"

// Case-Sprite association
struct		CaseType
{
  char		_c;
  const char	*_spath;
};

#endif
