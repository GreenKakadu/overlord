/***************************************************************************
                          SkillLevelelEmentData.cpp 
                             -------------------
    begin                : Fri May 23 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "SkillLevelElementData.h"

string SkillLevelElementData::printName()
{
  return data_->getSkill()->printLevel(data_->getLevel());
}
