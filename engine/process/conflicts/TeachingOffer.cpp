/***************************************************************************
                          TeachingOffer.cpp 
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
#include "TeachingOffer.h"
#include "TokenEntity.h"
#include "SkillRule.h"

TeachingOffer::TeachingOffer(TokenEntity * teacher, SkillRule * skill, vector <Entity *> &students)
{
  teacher_ =  teacher;
  skill_   =  skill;
  level_   =  teacher_->getSkillLevel(skill_);
  students_ =  students;
  vector <Entity *>::iterator iter;
  for(iter = students_.begin(); iter != students_.end(); ++iter)
  {
    (*iter)->addTeachingOffer(this);
    }

}



TeachingOffer::~TeachingOffer()
{

}



ostream&  TeachingOffer::reportOffer(ostream& out) const
{
 out << teacher_<< "Offers to teach "<< *skill_<< " on level "<< level_<<" to ";
 for(vector <Entity *>::const_iterator  iter =students_.begin();
                              iter != students_.end(); ++iter)
 {
   if(*iter)
   out<< (*iter) <<" ";
 }
  out<<endl;
  return out;
}



int TeachingOffer::getTeachingBonus()
{
 // Calculate total learning capacity.
 // If too many students taking a course teaching will be slowed down.
 int totalNum = 0;
 for(vector <Entity *>::const_iterator  iter = confirmedStudents_.begin();
                              iter != confirmedStudents_.end(); ++iter)
 {
   if(*iter) // extra check
//    if(skill_->teacherRequired(*iter) )
//      {
//        totalNum += 5 * (*iter)->getLearningCapacity();
//      }
//    else
//    {
        totalNum += (*iter)->getLearningCapacity();
//    }
 }

  if (totalNum < 100)
          totalNum = 100;
          
 return 100 * 100/totalNum;
}



void TeachingOffer::confirmTeachingOffer(Entity * unit)
{
  if(unit)
  {
    confirmedStudents_.push_back(unit);
  }
}



bool TeachingOffer::isConfirmed() const
{
 return  !confirmedStudents_.empty();
}

