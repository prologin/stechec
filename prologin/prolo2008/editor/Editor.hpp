// Editor.hpp -- The Prologin 2008 Map Editor

#ifndef EDITOR_HPP_
# define EDITOR_HPP_

# define INSTALL_PREFIX	"/opt/share/stechec/prolo2008/graphics/"

// This *MUST* match the sprite's size (both width and height).
# define GRID_SIZE	47

# define DEFAULT_XSIZE	10
# define DEFAULT_YSIZE	10

// The color of the selected case's rectangle.
# define RECT_COLOR	0x0000ff00

# define DEFAULT_FILENAME	"default.map"

# define MAP_MAX_X	20
# define MAP_MAX_Y	20

# define USAGE	"usage: editor [-c] [-g WxH] [-f file]"

# define MAP_ROCK               'X'
# define MAP_EMPTY              '.'
# define MAP_HOLE               'O'
# define MAP_APPLE              'P'
# define MAP_ROBOT_TEAM1        'H'
# define MAP_ROBOT_TEAM2        'h'

# define MAP_ROCK_KEY		'x'
# define MAP_EMPTY_KEY		'.'
# define MAP_HOLE_KEY		'o'
# define MAP_APPLE_KEY		'p'
# define MAP_ROBOT_TEAM1_KEY	'H'
# define MAP_ROBOT_TEAM2_KEY	'h'

// Case-Sprite association
struct		CaseType
{
  char		_key;
  char		_map_char;
  const char	*_spath;
};

#endif
