#include <cmath>
#include <qtimer.h>

#include "QTWorld.hh"
#include "QTObject.hh"

QTObject::QTObject(QTWorld* world)
  : world_(world),
    x_(-1),
    y_(-1),
    x_goal_(-1),
    y_goal_(-1),
    speed_(0),
    dead_(false),
    sprite_(NULL),
    origin_(NULL)
{
}

QTObject::~QTObject()
{
  delete sprite_;
}


void QTObject::move(int x, int y)
{
  if (!sprite_ || x_ < 0 || y_ < 0)
    jump(x, y);
  else
    {
      x_goal_ = x * world_->getCaseLength() + world_->getCaseLength() - sprite_->width() / 2;
      y_goal_ = y * world_->getCaseLength() + world_->getCaseLength() - sprite_->height() / 2;

      double dx = x_goal_ - x_;
      double dy = y_goal_ - y_;
      
      sprite_->setVelocity(dx / 4.0, dy / 4.0);

      QTimer* timer = new QTimer(this);
      connect(timer, SIGNAL(timeout()), this, SLOT(slotStopMoving()));
      timer->start(200, 1);
    }
}

void QTObject::jump(int x, int y)
{
  x_ = x * world_->getCaseLength();
  y_ = y * world_->getCaseLength();

  if (sprite_)
    {
      x_ += world_->getCaseLength() - sprite_->width() / 2;
      y_ += world_->getCaseLength() - sprite_->height() / 2;
      sprite_->move(x_,  y_, -1);
    }
}

void QTObject::show(bool state)
{
  if (sprite_)
    {
      if (state)
        sprite_->show();
      else
	sprite_->hide();
    }
}

void QTObject::setZ(int z)
{
  if (sprite_)
    sprite_->setZ(z); 
}

void QTObject::setAnim(const std::string& pixname, bool loop, int ms)
{
  const QTPixmap* pixmap = world_->getPixmap(pixname);
  if (pixmap == NULL)
    return;
  
  QCanvasPixmapArray* sequence = pixmap->getData();
  if (!sequence)
    return;

  if (!sprite_)
    {
      sprite_ = new QCanvasSprite(sequence, world_->canvas());

      x_ += world_->getCaseLength() - sprite_->width() / 2;
      y_ += world_->getCaseLength() - sprite_->height() / 2;
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
      connect(timer, SIGNAL(timeout()), this, SLOT(slotBackToOrigin()));

      if (ms < 0)
	return;

      if (ms > 0)
	timer->start(ms, 1);
      else
	timer->start(50 * sprite_->frameCount(), 1);
    }
}

void QTObject::setKilled()
{
  if (sprite_)
    {
      delete sprite_;
      sprite_ = NULL;
      origin_ = NULL;
    }

  dead_ = true;
}

void QTObject::setKillAnim(const std::string& pixname, int ms)
{
  setAnim(pixname);

  QTimer* timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(slotSetKilled()));

  if (ms > 0)
    timer->start(ms, true);
  else if (ms == 0)
    timer->start(50 * sprite_->frameCount(), true);
}

void QTObject::slotBackToOrigin()
{
  if (!sprite_)
    return;

  if (origin_)
    sprite_->setSequence(origin_);
  else
    {
      delete sprite_;
      sprite_ = NULL;
    }
}

void QTObject::slotStopMoving()
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
  x_goal_ = -1;
  y_goal_ = -1;
}

void QTObject::slotSetKilled()
{
  setKilled();
}
