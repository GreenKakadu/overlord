/***************************************************************************
                          TreeBonusElement.h 
                             -------------------
    begin                : Sun Feb 16 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

#ifndef TREE_BONUS_H
#define TREE_BONUS_H

#include "BonusElement.h"

/**Skill learning bonus, that spreads to all tree
  *@author Alex Dribin
  */

class TreeBonusElement : public BonusElement  {
public: 
	TreeBonusElement(SkillRule * rule, int bonusPoints) : BonusElement (rule,bonusPoints){}
	~TreeBonusElement(){}
};

#endif
