/***************************************************************************
                          BasicLearningStrategy.h 
                             -------------------
    begin                : Mon Feb 10 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef LEARNING_H
#define LEARNING_H

#include <vector>
#include "OverlordTypes.h"
#include "Strategy.h"
using namespace std;

class Entity;
class UnitEntity;
class RaceRule;
class SkillElement;
class InventoryElement;
class Parser;
class SkillRule;
class TeachingOffer;

/**Rules for learning skill
  *@author Alex Dribin
  */

class BasicLearningStrategy : public Strategy  {
public: 
      BasicLearningStrategy ( const string & keyword, GameData * parent): Strategy(keyword, parent){}
      BasicLearningStrategy ( const BasicLearningStrategy * prototype );
//      BasicLearningStrategy ( const BasicLearningStrategy  &learning );
		 ~BasicLearningStrategy(){};
      virtual STATUS     initialize      ( Parser *parser);
      GameData * createInstanceOfSelf();
  /** Calculate amount of experience gained by 1 day of study */
  virtual int  calculateLearningExperience(UnitEntity * unit, SkillRule * skill, TeachingOffer * teacher);
  /** Add learning experience to skill studied by unit*/
  virtual void addLearningExperience(UnitEntity * unit, SkillElement & skill);
//  virtual void addLearningExperience(UnitEntity * unit, SkillRule * skill, int experience);
  /**  Add learning experience to all parent skills of a skill studied by unit */
  virtual void addRecursiveLearningExperience(UnitEntity * unit, SkillElement & skill);
  /** No descriptions */
  virtual  LEARNING_RESULT mayLearn(UnitEntity * unit, SkillRule * skill) const;
  virtual  bool teacherRequired(Entity * unit, SkillRule * skill);
  // For debugging:====================
  void print() {cout << " Learning: ";}
  static int getPointsPerDay()  {return expBase_;}
  virtual  void extractKnowledge (Entity * recipient, int parameter = 0);
protected:
  static int leaderBonus_;
  static int expBase_;
   RaceRule * racial_;
   bool special_;
   InventoryElement * itemRequired_;
   InventoryElement * bonusItem_;
//   SkillElement * max_;
private:
};

#endif
