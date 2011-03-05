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

# include "game.h"
# include "surface.h"
# include "sprite.h"

/* ROAD check */
# define ROAD_N		0x1
# define ROAD_S		0x2
# define ROAD_E		0x4
# define ROAD_W		0x8
struct			MapRoadType
{
  unsigned int		flag;
  SpriteID		spr_id;
};

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
  int			_case[MAP_SIZE];	/* cases types		*/
  int			_case_price[MAP_SIZE];	/* cases prices		*/
  int			_case_owner[MAP_SIZE];	/* cases owners		*/
  int			_draw_pos[2];		/* drawing position	*/
  bool			_show_prices;		/* */

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
  void			ShowPrices(void);
  void			setCase(int type, int x, int y);
  void			setCasePrice(int price, int x, int y);
  void			setCaseOwner(int owner, int x, int y);
  void			setDrawPos(int x, int y);
  int			GetRealWidth(void) const;
  int			GetRealHeight(void) const;
};

#endif /* !DISPLAYMAP_H_ */
