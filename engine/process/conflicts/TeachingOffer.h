/***************************************************************************
                          TeachingOffer.h
                             -------------------
    begin                : Fri Jul 25 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef TEACHING_OFFER_H
#define TEACHING_OFFER_H
#include  <vector>
#include <string>
#include <iostream>
#include "Entity.h"

/**Provides syncronozation between TEACH and STUDY orders
  *@author Alex Dribin
  */
class SkillRule;
class PhysicalEntity;

class TeachingOffer {
public: 
	TeachingOffer(PhysicalEntity * teacher, SkillRule * skill, vector <Entity *>& students);
	virtual ~TeachingOffer();
  ostream&  print(ostream& out) const;
  virtual int getTeachingBonus();
  virtual void confirmTeachingOffer(Entity * unit);
  bool isConfirmed() const;
  inline PhysicalEntity * getTeacher() const {return teacher_;}
  inline SkillRule * getSkill() const {return skill_;}
  inline int getLevel() const {return level_;}
    protected:
  PhysicalEntity * teacher_;
  SkillRule * skill_;
  int level_;
  vector <Entity *> confirmedStudents_;
  vector <Entity *> students_;

};
   inline ostream& operator << (ostream& out,  TeachingOffer& data)
                                { return data.print(out); }

#endif