/***************************************************************************
                          Rule.cpp  -  description
                             -------------------
    begin                : Thu Jan 23 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "Rule.h"
#include "Entity.h"
#include "BasicCondition.h"
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


void    Rule::extractKnowledge (Entity * recipient, int parameter)
{
}



void Rule::printDescription(ReportPrinter & out)
{
    out << print()<< ": "<< getDescription()<<".";
}
