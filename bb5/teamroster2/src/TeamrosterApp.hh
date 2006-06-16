#ifndef TEAMROSTERAPP_HH_
#define TEAMROSTERAPP_HH_

#include "pgapplication.h"
#include "pgnavigator.h"

class TeamrosterApp  : public PG_Application, public PG_Navigator 
{
public:
    
    // -----------------------------------------------------------------------
    //  Constructors
    // -----------------------------------------------------------------------
	TeamrosterApp();
	virtual ~TeamrosterApp();

protected:
    // -----------------------------------------------------------------------
    //  GUI Handlers
    // -----------------------------------------------------------------------
	bool eventKeyDown(const SDL_KeyboardEvent* key);
};

#endif /*TEAMROSTERAPP_HH_*/
