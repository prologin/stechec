/*
** displaymap.h for  in prologin_gui
** 
** Made by Hazgar
** Login   <lapie_t@epitech.net>
** 
** Started on  Fri Mar 13 14:51:17 2009 Hazgar
** Last update Wed Apr 29 14:57:43 2009 user
*/

#ifndef DISPLAYMAP_H_
# define DISPLAYMAP_H_

# include <vector>

# include "Constant.hh"

# include "game.h"
# include "surface.h"
# include "sprite.h"

/* */
struct			MapCaseType
{
  int			type;
  SpriteID		spr_id;
};

/* */
class			DisplayMap
{
 private:
  void			InitFloorSfc(void);
  void			Refresh(void);
  int			_zoom;			/* sfc zoom (%)		*/
  int			_oldZoom;		/* */
  unsigned short	_viewField;		/* view field (%)	*/
  Surface		*_floor;		/* floor texture	*/
  Surface		*_floor_sfc;		/* floor sfc		*/
  Surface		*_sfc;			/* normal sfc		*/
  Surface		*_zsfc;			/* zoom sfc		*/
  std::vector<int>	_case[MAP_SIZE];	/* cases types		*/
  int			_draw_pos[2];		/* drawing position	*/

 public:
  DisplayMap(const Surface &display, Surface *texture = NULL);
  ~DisplayMap(void);
  void			setZoom(int zoom);
  void			setViewField(int size);
  int			getZoom(void) const;
  int			getViewField(void) const;
  void			Blit(Surface &dst, SfcField &pos);
  void			BuildFrom(const Surface &display);
  void			ZoomIn(void);
  void			ZoomOut(void);
  void			setCase(int type, int x, int y);
  void			setDrawPos(int x, int y);
  int			GetRealWidth(void) const;
  int			GetRealHeight(void) const;
  void			FlushCases(void);
};

#endif /* !DISPLAYMAP_H_ */
