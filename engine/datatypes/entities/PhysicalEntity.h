/***************************************************************************
                          PhysicalEntity.h
                             -------------------
    begin                : Fri Jul 25 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef PHYSICAL_ENTITY_H
#define PHYSICAL_ENTITY_H

#include "Entity.h"
class LocationEntity;

/**subclass of Entities that have physical appearence and may be located in the world
  *@author Alex Dribin
  */

class PhysicalEntity : public Entity  {
public: 
	PhysicalEntity();
      PhysicalEntity (const string & keyword, GameData * parent ) : Entity(keyword, parent){}
      PhysicalEntity ( const PhysicalEntity * prototype ): Entity(prototype){}
	~PhysicalEntity(){}
          void cancelTeachingOffer();
   inline LocationEntity * getLocation() const{return location_;}
   inline void             setLocation(LocationEntity * location) {location_ = location;}
   protected:
   LocationEntity * location_ ;
};

#endif
