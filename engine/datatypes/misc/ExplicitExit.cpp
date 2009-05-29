/***************************************************************************
                          ExplicitExit.cpp  -  description
                             -------------------
    begin                : Mon Jan 20 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "ExplicitExit.h"
#include "LocationEntity.h"
#include "DirectionVariety.h"


ExplicitExit::ExplicitExit(LocationEntity * origin, DirectionVariety * dir,
        LocationEntity * dest, MovementMode <int> standardTravelTime) :
        BasicExit(origin,dir, dest)
{
 standardTravelTime_ = MovementMode<int>(standardTravelTime);
}



/*
 *  prints description of exit for debugging purposes.
 */
string ExplicitExit::print()
{
	return string("explicit exit  ") + dir_->print() + " to " + destination_->print();
}



/*
 * Saves exit data. Used for generation location data
 */
void ExplicitExit::save(ostream & out)
{
  UINT i;
	out << "EXIT_EXPLICIT " << dir_->getTag() << " " << destination_->getTag();
  for (i = 0; i < standardTravelTime_.size(); i++)
		{
     int time = standardTravelTime_[i];
		 if( time != 0)
			 out << " " << (movementModes[i])->getTag()<< " " << time ;
		}
		
		out << endl;
}



/*
 * 
 */
int ExplicitExit::getTravelTime(MovementVariety * mode)
{
 return standardTravelTime_[mode];
}
