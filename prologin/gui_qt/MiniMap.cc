#include "MiniMap.hh"

MiniMap::MiniMap(WorldMap* world_map)
  : QCanvasView(new QCanvas(0, "Mini Map Canvas"), world_map, "Mini Map"),
    world_map_(world_map)
{
  canvas()->resize(200, 150);
  canvas()->setBackgroundColor(QColor(150, 150, 150));

  resize(204, 154);

  selection_ = new QCanvasRectangle(0, 0, 60, 45, canvas());
  selection_->show();

  show();
}

MiniMap::~MiniMap()
{
}
