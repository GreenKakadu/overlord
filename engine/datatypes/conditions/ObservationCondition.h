/***************************************************************************
                          ObservationCondition.h  
                             -------------------
    begin                : Sun Jan 26 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef OBSERVATION_CONDITION_H
#define OBSERVATION_CONDITION_H
#include "BasicCondition.h"
class UnitEntity;

class ObservationCondition : public BasicCondition  {
public: 
  ObservationCondition(const string & keyword, GameData * parent ): BasicCondition(keyword, parent){}
  ObservationCondition(const ObservationCondition * prototype): BasicCondition(prototype){}
	~ObservationCondition(){}
  static ObservationCondition * createObservationCondition(int observationLevel);
  STATUS      initialize ( Parser *parser );
  GameData * createInstanceOfSelf();
  bool isSatisfied(UnitEntity * unit);
  inline void setObservationCondition(int observation) {observationRequired_ = observation;}
  inline int getObservationCondition() const {return observationRequired_;}
protected:
  int observationRequired_;
};

#endif
