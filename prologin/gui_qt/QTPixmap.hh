/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2005, 2006 Prologin
*/

#ifndef QTPIXMAP_HH_
# define QTPIXMAP_HH_

# include <qcanvas.h>

/*!
** A QT pixmap, can be an image sequence.
*/
class QTPixmap
{
public:
  QTPixmap();
  ~QTPixmap();

  //! @brief Load a pixmap from a file
  //! @param path Full directory to the image.
  //! @param pixname File to load, without file extension.
  //! @return true on success.
  bool                  load(const std::string& path,
                             const std::string& pixname);

  //! @brief Get the pixmap name (ie: the filename).
  const std::string&    getName() const;

  //! @brief Get pixmap data.
  QCanvasPixmapArray*   getData() const;

private:
  std::string           pixname_;
  QCanvasPixmapArray*   data_;
};


inline const std::string& QTPixmap::getName() const
{
  return pixname_;
}

inline QCanvasPixmapArray* QTPixmap::getData() const
{
  return data_;
}

#endif // !QTPIXMAP_HH_
