/***************************************************************************
                          TitleElement.cpp 
                             -------------------
    begin                : Wed Sep 3 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
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

vector <AbstractData *> TitleElement::aPrint()
{
    vector <AbstractData *> v;
    if(!unit_)
    {
        v.push_back(new StringData("Available "));
    }
    else
    {
        v.push_back(new StringData("The "));
    }
    v.push_back(new StringData("title of "));
    v.push_back(title_);
    v.push_back(new StringData(" of "));
    v.push_back(location_);
    if(unit_)
    {
        v.push_back(new StringData(" held by "));
        v.push_back(unit_);
    }
    if(title_->getClaimingCondition())
    {
        vector <AbstractData *> v1 = title_->getClaimingCondition()->aPrint();
        v.push_back(new StringData(" requires "));
        for(vector <AbstractData *>::iterator iter = v1.begin(); iter != v1.end(); ++iter)
        {
        v.push_back(*iter);
        }
    }
    return v;
}

vector <AbstractData *> TitleElement::unitPrint()
{
    vector <AbstractData *> v;
    v.push_back(new StringData(", the "));
    v.push_back(title_);
    v.push_back(new StringData(" of "));
    v.push_back(location_);
    return v;
}


void TitleElement::produceReport(ostream & out)
{
  if(!unit_)
  {
	out << "Available ";
  }
  else
  {
   out << "The ";
  }
  out << "title of " <<title_->getName() <<" of "<<location_->print();
  if(unit_)  out<< " held by "<< unit_->print();
  if(title_->getClaimingCondition())
    {
        out<< " requires " << *(title_->getClaimingCondition()) << ". ";
    }
  out<<endl;
}

TitleElement  * TitleElement::readPrintedElement (Parser * parser)
{
    string str =parser->getText();
    TitleRule * title =0;
    LocationEntity * location=0;
 // get substring before "of"
    unsigned pos0 = str.find(" of ");
    string titleName = string(str,0,pos0);
// get location
     pos0 = str.find_first_of('[');
    unsigned pos1 = str.find(']');
    string locationTag = string(str,pos0,pos1);


     GameData * data = gameFacade->getDataManipulator()->findGameDataByName(titleName);
     if(data)
     {
        title = dynamic_cast<TitleRule *>(data);
     }
     location   = gameFacade->locations[locationTag];

        if(( title == 0 ) || (location == 0) )
          return 0;
        else
        return new TitleElement(title, location,0);
}


TitleElement  * TitleElement::readElement (Parser * parser)
{
	UnitEntity * unit =0;
        TitleRule * title = gameFacade->titles[parser->getWord()];
        LocationEntity * location   = gameFacade->locations[parser->getWord()];
	string tag = parser->getWord();
	if(!tag.empty())
	{
	  unit   = gameFacade->units[tag];
	}
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
