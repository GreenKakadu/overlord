/***************************************************************************
                          SkillLevelElementData.h
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

#ifndef SKILL_LEVEL_ELEMENT_DATA_H
#define SKILL_LEVEL_ELEMENT_DATA_H

#include "AbstractData.h"
#include "SkillLevelElement.h"
/**
  *@author Alex Dribin
  */

class SkillLevelElementData : public AbstractData  {
public: 
	SkillLevelElementData(SkillLevelElement * data){data_ = data;}
	~SkillLevelElementData(){}
    string printName();
    protected:
   SkillLevelElement * data_; 
};

#endif
