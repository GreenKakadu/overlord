/***************************************************************************
                          TitleElement.cpp 
                             -------------------
    begin                : Wed Sep 3 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "TitleElement.h"
#include "TitleRule.h"
#include "BasicCondition.h"
#include "LocationEntity.h"
#include "UnitEntity.h"
#include "SkillCondition.h"

TitleElement::TitleElement(TitleRule * title, LocationEntity * location,
                           UnitEntity * unit = 0):AbstractData()
{
  title_ = title;
  location_ = location;
  unit_ = unit;
}



void TitleElement::save(ostream & out)
{

  out << title_->getTag() << " " <<  location_->getTag();
  if(unit_)
      out << " " << unit_->getTag();
  out <<endl;
}



string TitleElement::print()
{
  return ( title_->getName() + " of " + location_->print());
}




void TitleElement::produceReport(ostream & out)
{
  out << "Available title of " <<title_->getName() <<" of "<<location_->print();
  if(unit_)  out<< " held by "<< unit_->print();
  if(title_->getClaimingCondition())
    {
        out<< " requires " << *(title_->getClaimingCondition()) << ". ";
    }
  out<<endl;
}



TitleElement  * TitleElement::readElement (Parser * parser)
{
        TitleRule * title = titles[parser->getWord()];
        LocationEntity * location   = locations[parser->getWord()];
        UnitEntity * unit   = units[parser->getWord()];
        if(( title == 0 ) || (location == 0) )
          return 0;
        else
        return new TitleElement(title, location,unit);
}



BasicCondition *    TitleElement::getClaimingCondition()      const
{
    return title_->getClaimingCondition();
}



bool TitleElement::contest(UnitEntity * contender)
{
 if (unit_ == 0)
  return true;
 else 
  return title_->contest(unit_,contender,location_);
}




void  TitleElement::activateClaimingEffects()
{
  title_->activateClaimingEffects(unit_,location_);
}



void  TitleElement::desactivateClaimingEffects()
{
  title_->desactivateClaimingEffects(unit_,location_);
}



bool TitleElement::mayCancelTitle(TitleElement * title)
{
// Title cancellation: define LandTitle Class
// Some land titles allows cancellation of  other land titles 
	return false;
}
