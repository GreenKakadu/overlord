/***************************************************************************
                          PhysicalEntity.cpp 
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
#include "PhysicalEntity.h"
#include "TeachingOffer.h"


void PhysicalEntity::cancelTeachingOffer()
{
  vector <TeachingOffer  *>::iterator iter;
  for(iter = teachingOffers_.begin(); iter != teachingOffers_.end(); ++iter)
  {
    if((*iter)->getTeacher() == this)
      {
//         cout<< *this<<" deletes " << *(*iter) <<endl;
         delete (*iter);
         teachingOffers_.erase(iter);
         return;
      }
    }
  cout << "ERROR. on cancelling "<< printName() <<" Can't find his own teachingOffers\n";

}


