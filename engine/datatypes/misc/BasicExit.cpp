/***************************************************************************
                          BasicExit.cpp  -  description
                             -------------------
    begin                : Sun Jan 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "BasicExit.h"
#include "Parser.h"
#include "LocationEntity.h"
#include "DirectionVariety.h"
//extern MovementVariety * walkingMode;


BasicExit::BasicExit(LocationEntity * origin, DirectionVariety * dir, LocationEntity * dest)
{
 origin_ = origin;
 dir_ = dir;
 destination_ = dest; 
}

/** No descriptions */
//void BasicExit::read(Parser * parser)
//{
//  int i;
//   for(i=0;i<5;i++) // anyway only 5 movement modes
//   {
//    MovementElement newMovement = MovementElement::read(parser);
//    if(newMovement.isValidElement())
//       movemens_.push_back(newMovement);
//    else
//      break;
//   }
//}



void BasicExit::print()
{
	cout << "    Exit" << dir_->printName() << " to" << destination_->printName();
  UINT i;
  int days;
  bool firstMode= true;
  for(i=0; i < movementModes.size(); i++)
  {
    days =  getTravelTime(movementModes[i]);
    if( days )
      {
        if(firstMode)
          cout << " "<< days<< " days of" << (movementModes[i])->getName();
        else
         cout <<", "<< days<< " of" << (movementModes[i])->getName();
         firstMode = false;
       }

    }

  cout<<"."<<endl;

}


void BasicExit::report(ostream & out)
{
	out << "    Exit" << dir_->printName() << " to" << destination_->printName();
  UINT i;
  int days;
  bool firstMode= true;
  for(i=0; i < movementModes.size(); i++)
  {
    days =  getTravelTime(movementModes[i]);
    if( days )
      {
        if(firstMode)
          out << " "<< days<< " days of" << (movementModes[i])->getName();
        else
         out <<", "<< days<< " of" << (movementModes[i])->getName();
         firstMode = false;
       }

    }

  out<<"."<<endl;

}



/** No descriptions */
void BasicExit::save(ostream & out)
{
	out << "EXIT " << dir_->getTag() << " " << destination_->getTag()  <<endl;
}



int BasicExit::getTravelTime(MovementVariety * mode)
{
//MovementVariety * walkingMode = movementModes["walk"];
// if(walkingMode == 0)
//  cout << "oooooooooooooooooooooooops!\n";
 int time1 = origin_->getTerrain()->getTravelTime(mode);
 int time2 = destination_->getTerrain()->getTravelTime(mode);

 if((!time1) && (!time2))
    return 0;

 if((time1 == 0) && (mode != walkingMode))
   time1 = origin_->getTerrain()->getTravelTime(walkingMode);

 if((time2 == 0) && (mode != walkingMode))
   time2 = destination_->getTerrain()->getTravelTime(walkingMode);

 if((time1) && (time2))
    return (time1 +3 * time2) / 4;
 else   
  return 0;

   // returns travel time for  mode
}
