/***************************************************************************
                          BasicCondition.cpp  -  description
                             -------------------
    begin                : Sun Jan 26 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "BasicCondition.h"
GameData * BasicCondition::createInstanceOfSelf()
{
   return CREATE_INSTANCE<BasicCondition> (this);
}
/** adds knowledge about data mentioned in condition */
void    BasicCondition::extractKnowledge (Entity * recipient, int parameter)
{
}

