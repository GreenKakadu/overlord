/***************************************************************************
                          BasicExit.cpp  -  description
                             -------------------
    begin                : Sun Jan 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "BasicExit.h"
#include "LineParser.h"
#include "LocationEntity.h"
#include "DirectionVariety.h"
#include "WeatherRule.h"
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



/*
 *
 */
string BasicExit::print()
{
	return string("exit  ") + dir_->print() + " to " + destination_->print();
}


/*
 * Outputs exit information for use in reports
 */
void BasicExit::produceReport(ReportPrinter & out)
{
  out.incr_indent();
  out << "Exit "; out.incr_indent(); out.incr_indent(); out<< dir_->print() << " to " << destination_->print() <<" ("<<destination_->getTerrain()->print() <<")";
  int i;
  int days;
  bool firstMode= true;
  for(i=0; i < movementModes.size(); i++)
  {
    days =  getTravelTime(movementModes[i]);
    if( days )
      {
        if(firstMode)
          out << " "<< days<< " days of " << (movementModes[i])->getName();
        else
         out <<", "<< days<< " of " << (movementModes[i])->getName();
         firstMode = false;
       }

    }

  out<<"."<<endl;
  out.decr_indent();
  out.decr_indent();
  out.decr_indent();

}



/*
 *
 */
void BasicExit::save(ostream & out)
{
	out << "EXIT " << dir_->getTag() << " " << destination_->getTag()  <<endl;
}



/*
 *  returns travel time for  mode
 */
int BasicExit::getTravelTime(MovementVariety * mode)
{
 int weatherFactor;
 int time1 = origin_->getTerrain()->getTravelTime(mode);
     weatherFactor = origin_->getWeather()->getMovementBonus(mode);
		 time1 = time1 * (100 + weatherFactor) / 100;
 int time2 = destination_->getTerrain()->getTravelTime(mode);
     weatherFactor = destination_->getWeather()->getMovementBonus(mode);
		 time2 = time2 * (100 + weatherFactor) / 100;

//cout << "---> DEBUG--> from "<< origin_->printTag()<<" to "<<destination_->printTag()<<":  "<<time1<<","<<time2<<endl;

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
}
