/***************************************************************************
                          TitlesAttribute.cpp
                             -------------------
    begin                : Apr 04 2005
    copyright            : (C) 2005 by Alex Dribin
     email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "TitlesAttribute.h"
#include "TokenEntity.h"
#include "UnitEntity.h"
#include "FactionEntity.h"
#include "EntityStatistics.h"
#include "BasicCondition.h"
#include "TitleRule.h"


// TitlesAttribute::TitlesAttribute(UnitEntity * unit)
// {
// 	unit_ = unit;
// }
TitlesAttribute::TitlesAttribute(vector <TitleElement *> &titles)
{
	titles_ = titles;
}



TitlesAttribute::~TitlesAttribute(){
}



STATUS
TitlesAttribute::initialize        ( Parser *parser )
{
      if (parser->matchKeyword("TITLE"))
	{
    TitleElement * title = TitleElement::readElement(parser);
    if(title)
      addTitle(title);
	  return OK;
	}
	  return OK;
}



void TitlesAttribute::save(ostream &out)
{
  for( TitleIterator iter = titles_.begin(); iter != titles_.end(); ++iter)
     {
        out << "TITLE ";
        (*iter)->save(out);
        out << endl;
    }
}


ostream& TitlesAttribute::report(ostream &out)
{
  return out;
}



void TitlesAttribute::reportAll(FactionEntity * faction, ostream &out)
{
  for( TitleIterator iter = titles_.begin(); iter != titles_.end(); ++iter)
     {
        (*iter)->produceReport(out);
        out << " ";
        faction->addKnowledge((*iter)->getTitle());
    }
}


void TitlesAttribute::addTitle(TitleElement * title)
{
  titles_.push_back(title);
}



void TitlesAttribute::removeTitle(TitleElement * title)
{
  TitleIterator iter = find(titles_.begin(), titles_.end(), title);
  if(iter == titles_.end())
			return;

   titles_.erase( iter);

}



void TitlesAttribute::deleteTitle(TitleRule * titleType)
{
  TitleElement * title = findTitle(titleType);
  title->getTitleHolder()->removeTitle(title);
  removeTitle(title);
  delete title;
}




TitleElement * TitlesAttribute::findTitle(TitleRule * titleType)
{
  for(TitleIterator iter = titles_.begin(); iter != titles_.end(); ++iter)
  {
    if((*iter)->getTitle() == titleType)
      return (*iter);
  }
		return 0;
}



bool TitlesAttribute::mayCancelTitle(TitleElement * title)
{
 for(TitleIterator iter = titles_.begin(); iter != titles_.end(); ++iter)
 {
 	if((*iter)->mayCancelTitle(title))
 		return true;
 }
 return false;
}



int TitlesAttribute::getProductionBonus(SkillRule * skill)
{
  int bonus = 0;
	for (TitleIterator iter = titles_.begin();
        iter != titles_.end(); ++iter)
        {
         bonus += (*iter)->getTitle()->getProductionBonusValue(skill);
        }
  return bonus;
}



int TitlesAttribute::getMovementBonus(MovementVariety * mode)
{
  int bonus = 0;
  return bonus;
}


int TitlesAttribute::getLearningBonus(SkillRule * skill)
{
	int currentBonus = 0;
  int bonus = 0;
	for (TitleIterator iter = titles_.begin();
        iter != titles_.end(); ++iter)
        {
         currentBonus = (*iter)->getTitle()->getLearningBonus(skill);
				 if(currentBonus >bonus)
					bonus = currentBonus;
        }
  return bonus;
}


int TitlesAttribute::getStudyBonus(SkillRule * skill)
{
  int bonus = 0;
  return bonus;
}


int TitlesAttribute::getControlPointsBonus()
{
	int controlPoints  = 0;
	for (TitleIterator iter = titles_.begin();
                      iter != titles_.end(); ++iter)
		{
			controlPoints += (*iter)->getTitle()->getControl();
    }
		return controlPoints;
}


void TitlesAttribute::releaseAll(UnitEntity * unit)
{
  for(TitleIterator iter = titles_.begin(); iter != titles_.end(); ++iter)
  {
   (*iter)->setTitleHolder(0);
   unit->getFaction()->removeMaxControlPoints((*iter)->getTitle()->getControl());
   (*iter)->desactivateClaimingEffects(); // will clean ownership info
  }
}

void TitlesAttribute::printAll(ostream &out)
{
  for(TitleIterator iter = titles_.begin(); iter != titles_.end(); ++iter)
  {
    if(iter != titles_.begin())
      out << ", "<< *(*iter);
    else
      out << " "<< *(*iter);
  }
}

