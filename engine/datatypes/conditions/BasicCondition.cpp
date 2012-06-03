/***************************************************************************
                          BasicCondition.cpp  -  description
                             -------------------
    begin                : Sun Jan 26 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "BasicCondition.h"
BasicCondition  sampleBasicCondition ("CONDITION",      &sampleGameData);

GameData * BasicCondition::createInstanceOfSelf()
{
   return CREATE_INSTANCE<BasicCondition> (this);
}
/** adds knowledge about data mentioned in condition */
void    BasicCondition::extractKnowledge (Entity * recipient, int parameter)
{
}



void BasicCondition::save(ostream &out)
{

}
/*
 * Creates actual type of condition (according to keyword) but returns it as BasicCondition
 */
BasicCondition * BasicCondition::readCondition(Parser * parser)
{
return dynamic_cast<BasicCondition *> (createByKeyword(parser->getWord()));
}
