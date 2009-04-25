/*
** displaymap.h for  in prologin_gui
** 
** Made by Hazgar
** Login   <lapie_t@epitech.net>
** 
** Started on  Fri Mar 13 14:51:17 2009 Hazgar
** Last update Sat Apr 25 13:24:20 2009 Hazgar
*/

#ifndef DISPLAYMAP_H_
# define DISPLAYMAP_H_

# include "game.h"
# include "surface.h"
# include "sprite.h"

struct			MapCaseType
{
  int			type;
  SpriteID		spr_id;
};

class			DisplayMap
{
 private:
  void			InitFloorSfc(void);
  void			Refresh(void);
  int			_zoom;		/* sfc zoom (%)		*/
  int			_oldZoom;
  unsigned short	_viewField;	/* view field (%)	*/
  Surface		*_floor;	/* floor texture	*/
  Surface		*_floor_sfc;	/* floor sfc		*/
  Surface		*_sfc;		/* normal sfc		*/
  Surface		*_zsfc;		/* zoom sfc		*/
  int			_case[MAP_SIZE];/* cases types		*/
  int			_draw_pos[2];	/* drawing position	*/

 public:
  DisplayMap(const Surface &display, Surface *texture = NULL);
  ~DisplayMap(void);
  void			setZoom(unsigned short zoom);
  void			setViewField(unsigned short size);
  void			Blit(Surface &dst, SfcField &pos);
  void			BuildFrom(const Surface &display);
  void			ZoomIn(void);
  void			ZoomOut(void);
  void			setCase(int type, int x, int y);
  void			setDrawPos(int x, int y);
};

#endif /* !DISPLAYMAP_H_ */
