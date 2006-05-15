#include <fstream>

#include "Pixmap.hh"

Pixmap::Pixmap(Gui2D* gui, std::string name)
  : gui_(gui),
    name_(name)
{
  std::string filename = gui->path() + name + ".png";
  std::ifstream info;
  int n = 1;

  gui->send(std::string("loading pixmap: ") + filename);

  info.open((gui->path() + name + "/info").c_str());
  if (info)
    {
      info >> n;
      data_ = new QCanvasPixmapArray(gui->path() + name + "/%1.png", n);
      info.close();
    }
  else
    data_ = new QCanvasPixmapArray(filename);

  if (!data_->isValid())
    {
      ERR("loading pixmap '" << filename << "' failed");
      gui->send("loading failed");
      delete data_;
      data_ = 0;
    }
  gui->add(this);
}

Pixmap::~Pixmap()
{
  delete data_;
}
