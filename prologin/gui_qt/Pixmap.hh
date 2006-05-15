#ifndef VISIO_PIXMAP_HH
# define VISIO_PIXMAP_HH

# include <qcanvas.h>

class Pixmap;
# include "Gui2D.hh"

class Pixmap
{
public:
  Pixmap(Gui2D* gui, std::string name);
  ~Pixmap();

  std::string name();
  QCanvasPixmapArray* data();

private:
  Gui2D* gui_;

  std::string name_;
  QCanvasPixmapArray* data_;
};


inline std::string Pixmap::name ()
{
  return name_;
}

inline QCanvasPixmapArray* Pixmap::data ()
{
  return data_;
}

#endif // !VISIO_PIXMAP_HH
