/***************************************************************************
                          SkillsAttribute.cpp
                             -------------------
    begin                : Apr 04 2005
    copyright            : (C) 2005 by Alex Dribin
     email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "SkillsAttribute.h"
#include "TokenEntity.h"
#include "FactionEntity.h"
#include "EntityStatistics.h"
#include "BasicCondition.h"
#include "reporting.h"
#include "UnitEntity.h"
#include "RaceRule.h"
extern ReportPattern * forgetReport;
extern ReportPattern * skillLossReporter;


//SkillsAttribute::SkillsAttribute(TokenEntity * entity)
//{
//	entity_ = entity;
//}
SkillsAttribute::SkillsAttribute(SkillsAttribute &p)
{
  skills_ = p.skills_;
}



SkillsAttribute::SkillsAttribute(vector <SkillElement > &skills)
{
	skills_ = skills;
}



SkillsAttribute::~SkillsAttribute(){
}

STATUS
SkillsAttribute::initialize(Parser *parser)
{

    if (parser->matchKeyword("SKILL"))
    {
        SkillElement * skill = SkillElement::readElement(parser);
        if (skill != 0)
        {
            for (SkillIterator iter = skills_.begin(); iter != skills_.end(); iter++)
            {
                if ((*iter).getSkill() == skill->getSkill())
                {
                    return OK;
                }
            }
            skills_.push_back(*skill);
            return OK;
        }
        return OK;
    }
       return OK;
}

void SkillsAttribute::save(ostream &out)
{
  for (SkillIterator iter = skills_.begin(); iter != skills_.end(); iter++)
    {
           out << "SKILL ";
	  (*iter).save(out);
    }
}

void SkillsAttribute::save(ostream &out, string prefix)
{
  for (SkillIterator iter = skills_.begin(); iter != skills_.end(); iter++)
    {
           out <<prefix<< "SKILL ";
	  (*iter).save(out);
    }
}



ostream&  SkillsAttribute::report(ReportPrinter &out)
{
   bool isFirst = true;
  if(skills_.empty())
         return out;
  SkillIterator iter;
  out << "Skills: ";
  isFirst = true;

  for (iter = skills_.begin(); iter != skills_.end(); iter++)
		{
      if( iter != skills_.begin())
        {
          out << ", ";
        }
       out << (*iter);
    }
  out <<". ";
 return out;
}


void SkillsAttribute::reportAll(ReportPrinter &out)
{
  bool isFirst = true;
  if(skills_.empty())
         return;
  SkillIterator iter;
  out << "Skills: ";
  isFirst = true;

  for (iter = skills_.begin(); iter != skills_.end(); iter++)
		{
      if( iter != skills_.begin())
        {
          out << ", ";
        }
       out << (*iter);
    }
  out <<". ";
	return;
}



void SkillsAttribute::reportAllShort(ReportPrinter &out)
{
  bool isFirst = true;
  if(skills_.empty())
         return;
  SkillIterator iter;
  out << "Skills: ";
  isFirst = true;

  for (iter = skills_.begin(); iter != skills_.end(); iter++)
		{
      if( iter != skills_.begin())
        {
          out << ", ";
        }
      (*iter).reportExperience(out);
    }
  out <<". ";
	return;
}

vector <AbstractArray>   SkillsAttribute::aPrint()
{
    vector <AbstractArray> out;
    vector <AbstractData *> v;
    for(SkillIterator iter = skills_.begin();
            iter != skills_.end(); ++iter)
    {
        vector <AbstractData *> v = (*iter).aPrintSkill();
        out.push_back(v);

      }
    return out;
}



int SkillsAttribute::getProductionBonus(SkillRule * skill)
{
  int bonus = 0;
	for (SkillIterator iter = skills_.begin();
        iter != skills_.end(); ++iter)
        {
         bonus += (*iter).getSkill()->getProductionBonusValue(skill);
        }
  return bonus;
}



int SkillsAttribute::getMovementBonus(MovementVariety * mode)
{
  int bonus = 0;
	for (SkillIterator iter = skills_.begin();
        iter != skills_.end(); ++iter)
        {
         bonus += (*iter).getSkill()->getMovementBonus(mode);
        }
  return bonus;
}


int SkillsAttribute::getStudyBonus(SkillRule * skill)
{
  int bonus = 0;
	for (SkillIterator iter = skills_.begin();
        iter != skills_.end(); ++iter)
        {
         bonus += (*iter).getSkill()->getStudyBonus(skill);
        }
  return bonus;
}


int SkillsAttribute::getLearningBonus(SkillRule * skill)
{
  int bonus = 0;
	int currentBonus = 0;
	for (SkillIterator iter = skills_.begin();
        iter != skills_.end(); ++iter)
        {
         currentBonus = (*iter).getSkill()->getLearningBonus(skill);
				 if(currentBonus >bonus)
					bonus = currentBonus;
        }
  return bonus;
}








int SkillsAttribute::getSkillLevel(SkillRule  * const skill)
{
	SkillIterator iter;
	for (iter = skills_.begin(); iter != skills_.end(); iter++)
		{
			if (skill == (*iter).getSkill())
				{
          return skill->getLevel((*iter).getExpPoints() );
          }
		}
  return 0;

}

bool SkillsAttribute::hasSkill(SkillRule  * skill, int experience)
{
  if(( skill == 0) || ( experience == 0) )
    return true;
  SkillIterator iter;
	for (iter = skills_.begin(); iter != skills_.end(); iter++)
		{
			if ((skill == (*iter).getSkill()) && (experience <= (*iter).getExpPoints()))
				return true;
		}

	return false;

}



SkillElement *  SkillsAttribute::getSkillElement(SkillRule  * const skill)
{
  SkillIterator iter;
	for (iter = skills_.begin(); iter != skills_.end(); iter++)
		{
			if (skill == (*iter).getSkill())
				{
          return &(*iter);
        }
		}
  return 0;
}


int SkillsAttribute::getSkillPoints(SkillRule  * const skill)
{
	SkillIterator iter;
	for (iter = skills_.begin(); iter != skills_.end(); iter++)
		{
			if (skill == (*iter).getSkill())
				{
          return (*iter).getExpPoints();
          }
		}
  return 0;

}

int SkillsAttribute::addSkill(SkillRule * skill, int expPoints)
{
    SkillIterator iter;
    int newLevel = 0;

    for (iter = skills_.begin(); iter != skills_.end(); iter++)
    {
        if ((*iter).getSkill() == skill)
        {
            (*iter).setExpPoints((*iter).getExpPoints() + expPoints);
            newLevel = skill->getLevel((*iter).getExpPoints());
            //    cout <<skill->printTag()<< " Exp " << (*iter).getExpPoints()<<endl;
            break;
        }
    }
    if (iter == skills_.end())
        skills_.push_back(SkillElement(skill, expPoints));
    //    cout <<skill->printTag()<< " Old level " << oldLevel <<" new " << newLevel<<endl;

    return newLevel;
}


/*
 * Adding skill experience without checking for new level
 */
void  SkillsAttribute::addSkillExperience(SkillRule  * skill, int expPoints)
{
	SkillIterator iter;
  for ( iter = skills_.begin(); iter != skills_.end(); iter++)
		{
					if( (*iter).getSkill() == skill)
								{
									 (*iter).setExpPoints((*iter).getExpPoints() + expPoints);
								  break;
								}
		}
    if (iter == skills_.end())
	      skills_.push_back(SkillElement(skill,expPoints));
}



vector < SkillElement>& SkillsAttribute::getAll()
{
  return skills_;
}


// set all experience to 0 but not delete
void SkillsAttribute::clear()
{
	 for(SkillIterator iter = skills_.begin(); iter != skills_.end(); ++iter)
      {
        (*iter).setExpPoints(0);
      }

}



int SkillsAttribute::getCapacity(int modeIndex)
{
	int capacity = 0;
	for(SkillIterator skillIter = skills_.begin();
                    skillIter != skills_.end(); ++skillIter)
    {
        capacity += (*skillIter).getCapacity(modeIndex);
    }
	return capacity;
}



int SkillsAttribute::getCapacity(MovementVariety * mode)
{
	int capacity = 0;
	for(SkillIterator skillIter = skills_.begin();
                    skillIter != skills_.end(); ++skillIter)
    {
        capacity += (*skillIter).getCapacity(mode);
    }
	return capacity;
}



// Removes all experience in given skill and all it's derivatives
void SkillsAttribute::forgetSkill(SkillRule * skill, TokenEntity * entity)
{
 for(SkillIterator iter = skills_.begin(); iter != skills_.end();)
		{
			if ((skill == (*iter).getSkill()) || ((*iter).getSkill()->isDescendFrom(skill,1)))
			{
      entity->addReport(new UnaryMessage(forgetReport,(*iter).getSkill()));
			   // Skill loss may cause some items to be unequiped
			   // or titles lost
            //(*iter).getSkill()->checkConditions(entity);
			    skills_.erase(iter);
			}
			else
			   ++iter;
		}
 entity->checkEquipmentConditions();
}



void SkillsAttribute::proportionallyDiluteAll(int oldNumber, int newNumber,
			 UnitEntity * unit)
{

  int oldLevel;

  // recalculate skills
  // should take into account intristic skills
  RaceRule * race = unit->getRace();
  assert(race);

  SkillIterator iter;
	for (iter = skills_.begin(); iter != skills_.end(); iter++)
		{
      oldLevel = (*iter).getLevel();

      (*iter).setExpPoints( ( (*iter).getExpPoints() * oldNumber + race->hasIntisticSkill((*iter).getSkill()) * newNumber)
																										 / (newNumber));
      if( (*iter).getLevel() < oldLevel)
        {
  	        unit->addReport( new BinaryMessage(skillLossReporter, (*iter).getSkill(), new IntegerData((*iter).getLevel()) ));
           //(*iter).getSkill()->checkConditions(unit);
           unit->checkEquipmentConditions();
        }
		}
}



void SkillsAttribute::addStats(EntityStatistics * stats)
{
	for (SkillIterator iter = skills_.begin();
                  iter != skills_.end(); ++iter)
		{
      stats->addStats((*iter).getStats());
    }
}

void SkillsAttribute::extractAndAddKnowledge(Entity * recipient, int parameter)
{
    	for (SkillIterator iter = skills_.begin();
                  iter != skills_.end(); ++iter)
		{
            recipient->addSkillKnowledge((*iter).getSkill(), (*iter).getLevel());
        }


}
