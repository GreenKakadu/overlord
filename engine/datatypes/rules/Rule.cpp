/***************************************************************************
                          Rule.cpp  -  description
                             -------------------
    begin                : Thu Jan 23 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "Rule.h"
#include "Entity.h"
#include "BasicCondition.h"
#include "DataManipulator.h"

extern DataManipulator * dataManipulatorPtr;
Rule::Rule ( const Rule * prototype ): GameData(prototype)
{
}


STATUS
Rule::initialize        ( Parser *parser )
{
  if (parser->matchKeyword("KNOWLEDGE"))
  {
    Rule * data = dynamic_cast<Rule *>(dataManipulatorPtr->findGameData(parser->getWord()));
    //Rule * data = dynamic_cast<Rule *>(createByKeyword(parser->getWord()));
    if(data)
    {
        explicitKnowledge_.push_back(data);
    }
    else
    {
        cerr << "Unknown knowledge ignored"<<endl;
    }
    return OK;
  }
  return OK;
}



void Rule::bindCondition(BasicCondition * condition)
{
  conditions_.push_back(condition);
}



void Rule::checkConditions(Entity * entity)
{
  vector <BasicCondition *>::iterator iter;
  for(iter = conditions_.begin();iter != conditions_.end(); ++iter)
  {
     (*iter)->conditionHandler(entity);
    }
}

void Rule::extractKnowledge(Entity * recipient, int parameter)
{
    for (vector<Rule *>::iterator iter = explicitKnowledge_.begin();
            iter != explicitKnowledge_.end(); ++iter)
    {
        // Skill knowledge should be added separatelly
        SkillRule * skill = dynamic_cast<SkillRule *> (*iter);
        if (skill)
        {
            if (recipient->addSkillKnowledge(skill,1))
                (*iter)->extractKnowledge(recipient, 1);

        } else
        {
            if (recipient->addKnowledge(*iter))
                (*iter)->extractKnowledge(recipient, parameter);
        }
    }
}



void Rule::printDescription(ReportPrinter & out)
{
    out << print()<< ": "<< getDescription()<<".";
}





int Rule::getMovementBonus(MovementVariety * mode)
{
  return movementBonuses_.getMovementBonus(mode);
}

  vector <AbstractData *> Rule::aPrint()
{
  vector <AbstractData *> v;
  return v;
}