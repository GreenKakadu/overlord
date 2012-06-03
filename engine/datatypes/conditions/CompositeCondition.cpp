/* 
 * File:   CompositeCondition.cpp
 * Author: alex
 * 
 * Created on April 6, 2011, 11:08 AM
 */
#include "CompositeCondition.h"
#include "BasicCondition.h"



BasicCondition  sampleCompositeCondition ("COMPOSITE_CONDITION",      &sampleBasicCondition);

GameData * CompositeCondition::createInstanceOfSelf()
{
   return CREATE_INSTANCE<CompositeCondition> (this);
}

void CompositeCondition::save(ostream &out)
{

}



bool CompositeCondition::isSatisfied(TokenEntity * , Entity * )
{
    return true;
}



//void CompositeCondition::conditionHandler(Entity * )
//{
//
//}



void CompositeCondition::setSubject(Rule * )
{
}


Rule * CompositeCondition::getSubject() const
{
    return 0;
}



void CompositeCondition::extractKnowledge (Entity * recipient, int parameter )
{

}
