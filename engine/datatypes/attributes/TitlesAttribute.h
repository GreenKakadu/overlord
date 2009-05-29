/***************************************************************************
                          TitlesAttribute.h
                             -------------------
    begin                : Apr 04 2005
    copyright            : (C) 2005 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef TITLES_ATTRIBUTE_H
#define TITLES_ATTRIBUTE_H

#include "BasicAttribute.h"
#include "TitleElement.h"
class EntityStatistics;
class SkillRule;
//class UnitEntity;
class FactionEntity;
class TokenEntity;
/**
  *@author Alex Dribin
  */

class TitlesAttribute : public BasicAttribute  {
public: 
	TitlesAttribute(){}
	TitlesAttribute(vector <TitleElement *> &titles);
//	TitlesAttribute(UnitEntity * unit);
	~TitlesAttribute();
  STATUS     initialize      ( Parser *);
  void save(ostream &out);
  ostream& report(ostream &out);

// Titles ========================================================
          void reportAll(FactionEntity * faction, ostream &out);
	        void addTitle(TitleElement * title);
          void removeTitle(TitleElement * title);
					bool mayCancelTitle(TitleElement * title);
          void deleteTitle(TitleRule * titleType);
          TitleElement * findTitle(TitleRule * titleType);
          void activateAll();



	virtual int getProductionBonus(SkillRule * skill);
 	virtual int getMovementBonus(MovementVariety * mode);
  virtual int getLearningBonus(SkillRule * skill);
  virtual int getStudyBonus(SkillRule * skill);
//  ==============================================
          int getControlPointsBonus();
					void releaseAll(UnitEntity * unit);
          void printAll(ostream &out);
					vector < TitleElement *> * getAll() {return &titles_;}
		protected:
  vector < TitleElement *>      titles_;
//	UnitEntity * unit_;
};
typedef   vector <TitleElement *>::iterator  TitleIterator;
#endif
