/***************************************************************************
                          ObservationCondition.h  
                             -------------------
    begin                : Sun Jan 26 2003
 *
 * Observation condition defines condition of observation
 * of some Entity (target) by some TokenEntity (agent);
 * Observation Condition may contain bonus or malus for such observation
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef OBSERVATION_CONDITION_H
#define OBSERVATION_CONDITION_H
#include "BasicCondition.h"
class UnitEntity;


class ObservationCondition : public BasicCondition  {
public: 
  ObservationCondition(const string & keyword, GameData * parent ): BasicCondition(keyword, parent){}
  ObservationCondition(const ObservationCondition * prototype); 
	~ObservationCondition(){}
  static ObservationCondition * createObservationCondition(int observationLevel);
  STATUS      initialize ( Parser *parser );
  virtual void save(ostream &out);
  GameData * createInstanceOfSelf();
  bool isSatisfied(TokenEntity * unit, Entity * target =0);
  inline void setObservationCondition(int observation) {observationRequired_ = observation;}
  inline int getObservationCondition() const {return observationRequired_;}
protected:
  int observationRequired_;
};
extern ObservationCondition  sampleObservationCondition;

#endif
