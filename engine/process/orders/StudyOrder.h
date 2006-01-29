/***************************************************************************
                          StudyOrder.h
                             -------------------
    begin                : Thu Feb 13 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef STUDY_ORDER_H
#define STUDY_ORDER_H

#include "OrderPrototype.h"
class ReportPattern;
class SkillRule;
class UnitEntity;
class TeachingOffer;
/**
  *@author Alex Dribin
  */

class StudyOrder : public OrderPrototype  {
public:
	StudyOrder();
	~StudyOrder(){}
  STATUS loadParameters(Parser * parser, ParameterList &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, ParameterList &parameters);
    protected:
   ORDER_STATUS preProcess_(UnitEntity * unit, SkillRule * skill, int level);
   ORDER_STATUS doProcess_(UnitEntity * unit, SkillRule * skill, int level, TeachingOffer *teacher);
   bool teacherRequired_;
   static const  unsigned TEACHER_REQUIRED_REPORT_FLAG;
};

#endif
