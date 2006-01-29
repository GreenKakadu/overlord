/***************************************************************************
                          SkillBonusAttribute.cpp  -  description
                             -------------------
    begin                : Feb 16 2005
    copyright            : (C) 2005 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "SkillBonusAttribute.h"
#include "BonusElement.h"
#include "MovementVariety.h"
#include "Entity.h"

SkillBonusAttribute::SkillBonusAttribute()
{
   keyword_ = "STUDY_BONUS";
	 reportTitle_ = "Skill bonuses";
}



SkillBonusAttribute::SkillBonusAttribute(const char * keyword, const char * reportTitle )
{
 keyword_ = keyword;
 reportTitle_ = reportTitle;

}



SkillBonusAttribute::~SkillBonusAttribute(){
}



STATUS
SkillBonusAttribute::initialize        ( Parser *parser )
{
	if ( parser->matchKeyword ("STUDY_BONUS") )
    {
        SkillRule * skill = skills[parser->getWord()];
        int bonusPoints = parser->getInteger();
        if(( skill == 0 ) || (bonusPoints == 0) )
          return OK;
        else
        	skillBonuses_.push_back(BonusElement(skill, bonusPoints));
    }
   return OK;
}



// int SkillBonusAttribute::getSkillBonus(SkillRule * skill)
// {
// 	for(BonusIterator2 iter = skillBonuses_.begin();
// 	iter != skillBonuses_.end(); ++iter)
// 	{
// 		if ( (*iter).getSkill() == skill)
// 		return (*iter).getBonusPoints();
// 	}
// 	return 0;
// }



int SkillBonusAttribute::getSkillBonus(SkillRule * skill)
{
  int bonus = 0;
  int currentBonus;
  for( vector <BonusElement>::iterator iter = skillBonuses_.begin();
       iter != skillBonuses_.end(); ++iter)
       {
          if(skill->isDescendFrom((*iter).getSkill(),1))
          {
            currentBonus = (*iter).getBonusPoints();
            if   (bonus < currentBonus)
                  bonus = currentBonus;
          }
       }
return bonus;
}



void SkillBonusAttribute::add( BonusElement * data)
{
  for (BonusIterator2  iter = skillBonuses_.begin();
    iter != skillBonuses_.end();  ++iter)
    {
      if ( (*iter).getSkill() == data->getSkill() )
        {

          (*iter).setBonusPoints((*iter).getBonusPoints() +
									 data->getBonusPoints()) ;
            return;
        }
    }
    skillBonuses_.push_back(*data);

}



void SkillBonusAttribute::remove(BonusElement * data)
{
  for (BonusIterator2  iter = skillBonuses_.begin();
    iter != skillBonuses_.end();  ++iter)
    {
      if ( (*iter).getSkill() == data->getSkill() )
        {

          (*iter).setBonusPoints((*iter).getBonusPoints() -
									 data->getBonusPoints()) ;

					if((*iter).getBonusPoints() == 0)
          {
            skillBonuses_.erase(iter);
          }
         return;
        }
    }
}



void    SkillBonusAttribute::extractKnowledge (Entity * recipient, int parameter)
{
  for(vector <BonusElement>::iterator iter = skillBonuses_.begin();
                                 iter != skillBonuses_.end(); ++iter)
    {
      if((*iter).getSkill())
      {
        if(recipient->addSkillKnowledge((*iter).getSkill(),1))
          (*iter).getSkill()->extractKnowledge(recipient, 1);

      }
  }
}



void SkillBonusAttribute::save(ostream &out)
{
  for (BonusIterator2 bonusIter = skillBonuses_.begin(); bonusIter != skillBonuses_.end(); bonusIter++)
    {
        out << keyword_ ;
        (*bonusIter).save(out);
    }
}



ostream& SkillBonusAttribute::report(ostream &out)
{
 if(!skillBonuses_.empty())
    {
      out << reportTitle_<<": ";
      for ( vector< BonusElement>::iterator iter = skillBonuses_.begin();
            iter != skillBonuses_.end(); iter++)
		    {
          if( iter != skillBonuses_.begin())
            {
              out << ", ";
            }
          out << (*iter).print();
        }
      out <<". ";
    }
		return out;
}

