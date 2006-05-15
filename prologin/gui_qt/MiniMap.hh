#ifndef VISIO_MINI_MAP_HH
# define VISIO_MINI_MAP_HH

# include <qcanvas.h>

class MiniMap;
# include "WorldMap.hh"

class MiniMap : public QCanvasView
{
public:
  MiniMap(WorldMap* world_map);
  ~MiniMap();

private:
  WorldMap* world_map_;

private:
  QCanvasRectangle* selection_;
};

#endif // !VISIO_MINI_MAP_HH
