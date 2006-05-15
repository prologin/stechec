#include <cmath>
#include <qtimer.h>

#include "Object2D.hh"
#include "Pixmap.hh"

Object2D::Object2D(Gui2D* gui, std::string name)
  : gui_(gui),
    name_(name),
    x_(-1),
    y_(-1),
    sprite_(0),
    x_goal_(0),
    y_goal_(0),
    origin_(0),
    dead_(false)
{
  gui_->add(this);
}

Object2D::~Object2D()
{
  delete sprite_;
}

void Object2D::load(std::string name)
{
  if (!gui_->pixmap(name))
    new Pixmap(gui_, name);
}

void Object2D::move(int x, int y)
{
  if (!sprite_ || x_ < 0 || y_ < 0)
    jump(x, y);
  else
    {
      x_goal_ = x * gui_->case_length_ + gui_->case_length_ - sprite_->width() / 2;
      y_goal_ = y * gui_->case_length_ + gui_->case_length_ - sprite_->height() / 2;

      double dx = x_goal_ - x_;
      double dy = y_goal_ - y_;
      
      sprite_->setVelocity(dx / 4.0, dy / 4.0);

      QTimer* timer = new QTimer(this);
      connect(timer, SIGNAL(timeout()), this, SLOT(stop_moving_()));
      timer->start(200, 1);
    }
}

void Object2D::jump(int x, int y)
{
  x_ = x * gui_->case_length_;
  y_ = y * gui_->case_length_;

  if (sprite_)
    {
      x_ += gui_->case_length_ - sprite_->width() / 2;
      y_ += gui_->case_length_ - sprite_->height() / 2;
      sprite_->move(x_,  y_, -1);
    }
}

void Object2D::anim(std::string name, bool loop, int ms)
{
  Pixmap* pixmap = gui_->pixmap(name);

  if (!pixmap)
    pixmap = new Pixmap(gui_, name);

  QCanvasPixmapArray* sequence = pixmap->data();

  if (!sequence)
    return;

  if (!sprite_)
    {
      sprite_ = new QCanvasSprite(sequence, gui_->main_window()->world_map()->canvas());

      x_ += gui_->case_length_ - sprite_->width() / 2;
      y_ += gui_->case_length_ - sprite_->height() / 2;

      sprite_->setZ(0);
      sprite_->setFrameAnimation();
      sprite_->show();
    }
  else
    sprite_->setSequence(sequence);

  if (loop)
    origin_ = sequence;
  else
    {
      QTimer* timer = new QTimer(this);
      connect(timer, SIGNAL(timeout()), this, SLOT(back_to_origin_()));

      if (ms < 0)
	return;

      if (ms > 0)
	timer->start(ms, 1);
      else
	timer->start(50 * sprite_->frameCount(), 1);
    }
}

void Object2D::setZ(int z)
{
  if (sprite_)
    sprite_->setZ(z); 
}

void Object2D::kill()
{
  if (sprite_)
    {
      delete sprite_;
      sprite_ = 0;
      origin_ = 0;
    }

  dead_ = true;
}

void Object2D::kill(std::string name, int ms)
{
  anim(name);

  QTimer* timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(kill()));

  if (ms < 0)
    return;
  if (ms > 0)
    timer->start(ms, true);
  else
    timer->start(50 * sprite_->frameCount(), true);
}

void Object2D::back_to_origin_()
{
  if (!sprite_)
    return;

  if (origin_)
    sprite_->setSequence(origin_);
  else
    {
      delete sprite_;
      sprite_ = 0;
    }
}

void Object2D::stop_moving_()
{
  if (!sprite_)
    {
      x_ = x_goal_;
      y_ = y_goal_;
    }
  else
    {
      x_ = x_goal_;
      y_ = y_goal_;

      sprite_->move(x_, y_);
      sprite_->setVelocity(0, 0);
    }
}
