/***************************************************************************
                          ExplicitExit.cpp  -  description
                             -------------------
    begin                : Mon Jan 20 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
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
void ExplicitExit::print()
{
	cout << "exit  " << dir_->printName() << " to " << destination_->printName() << endl;
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
     if( standardTravelTime_[i] != 0)
			 out << standardTravelTime_[i] << " " << movementModes[i];
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
