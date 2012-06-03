/***************************************************************************
                          EnchantmentAttribute.h
                             -------------------
    begin                : Thu Mar 11 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef ENCHANTMENT_ATTRIBUTE_H
#define ENCHANTMENT_ATTRIBUTE_H

#include "BasicAttribute.h"
#include "EnchantmentElement.h"
class EntityStatistics;
class SkillRule;
/**
  *@author Alex Dribin
  */

class EnchantmentAttribute : public BasicAttribute  {
public: 
	EnchantmentAttribute();
	~EnchantmentAttribute();
  STATUS     initialize      ( Parser *);
  void save(ostream &out);
  void save(ostream &out, string prefix);
  ostream& report(ostream &out);
      vector <AbstractArray>   aPrint();
  void add(EnchantmentElement * data);
  void remove(EnchantmentElement * data);
  void removeAll();
  void addStats(EntityStatistics * stats, int figuresNumber );
  void processExpiration(Entity * entity, int figuresNumber = 1 );
  void carryOutAllActions(Entity * entity, int value);
  int getCapacity(int modeIndex, int figuresNumber );
  int getCapacity(MovementVariety * mode, int figuresNumber );
         int getProductionBonus(SkillRule * skill);
         int getStudyBonus(SkillRule * skill);
         int getLearningBonus(SkillRule * skill);
         int getMovementBonus(MovementVariety *    movingMode);
   EnchantmentElement * has(EnchantmentElement * data);
  EnchantmentElement * findAndDo(EnchantmentElement * data, EnchantmentElement * (*toDo) ());
      inline bool empty()  {return dataCollection_.empty();}
    protected:
  vector <EnchantmentElement> dataCollection_;
};
typedef   vector <EnchantmentElement>::iterator  EnchantmentAttributesIterator;
#endif
