/***************************************************************************
                          ExplicitExit.h  -  description
                             -------------------
    begin                : Mon Jan 20 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef EXPLICIT_EXIT_H
#define EXPLICIT_EXIT_H

#include "BasicExit.h"

class ExplicitExit : public BasicExit  {
public: 
	ExplicitExit(){}
	ExplicitExit(LocationEntity * origin, DirectionVariety * dir,
              LocationEntity * dest, MovementMode <int> standardTravelTime);
	~ExplicitExit(){}
   void save(ostream & out);
   void print();
  int getTravelTime(MovementVariety * mode);
    protected:
    MovementMode <int> standardTravelTime_;
};

#endif
