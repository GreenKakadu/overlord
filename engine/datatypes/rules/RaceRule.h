/***************************************************************************
                          RaceRule.h  -  description
                             -------------------
    begin                : Tue Dec 10 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef RACE_H
#define RACE_H

#include "Rule.h"
#include "ItemRule.h"
#include "UnitEntity.h"
#include "EntityStatistics.h"
#include "BonusElement.h"

class EquipmentSlotVariety;

typedef struct equip_slot {
  equip_slot(EquipmentSlotVariety * slotRule, int  cap = 1);
  ~equip_slot();
  EquipmentSlotVariety * type;
	int    capacity;
} EquipSlot;


typedef vector<EquipSlot *>::iterator EquipSlotIterator;
class RaceRule : public Rule  {
public:
      RaceRule ( const string & keyword, GameData * parent) : Rule(keyword, parent){}
      RaceRule ( const RaceRule * prototype );
      virtual STATUS     initialize      ( Parser *parser);
      GameData * createInstanceOfSelf();
 //     void     print();
  /** Returns number of items that may be equiped in this category. */
  int getEquipCapacity( EquipmentSlotVariety * type);
//  friend void UnitEntity::updateEquipement();
  inline string getPluralName() const{return pluralName_;}
         void   setPluralName(const string name);
  inline int getWeight() const {return weight_;}
	inline void setWeight(int weight) {weight_ = weight;}
  virtual LEARNING_RESULT mayLearn(SkillRule * skill, UnitEntity * unit);
  virtual bool teacherRequired(SkillRule * skill, UnitEntity * unit);
  virtual bool mayMove();
  /** No descriptions */
  int getBonus(SkillRule * skill);
  inline  EntityStatistics * getStats()  {return &stats;}
  inline int getCapacity(int modeIndex){return capacity_[modeIndex];}
  /** No descriptions */
  virtual bool mayWork();
  virtual bool mayRectuit();
  virtual bool mayTrade();
  virtual bool mayTransferFigures();
   vector< EquipSlot *>& getAllEquipmentSlots();
  /** Amounts of efforts teacher should spend in order to teach this student. */
  virtual int getLearningCapacity();
  virtual void extractKnowledge (Entity * recipient, int parameter = 0);
	protected:
    vector< EquipSlot *> equipmentSlots_;
    vector< SkillElement> intristicSkills_;
    vector< BonusElement> intristicSkillBonuses_;
		MovementMode<int> capacity_;
	  string pluralName_;
		int weight_;
    int hiringCost_;
    int hiringProbability_;
    int hiringMax_;
    EntityStatistics stats;
    private:
	
};

#endif
