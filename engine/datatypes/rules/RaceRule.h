/***************************************************************************
                          RaceRule.h  -  description
                             -------------------
    begin                : Tue Dec 10 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef RACE_H
#define RACE_H

#include "Rule.h"
#include "ItemRule.h"
#include "UnitEntity.h"
#include "EntityStatistics.h"
#include "BonusElement.h"
#include "SkillBonusComboAttribute.h"

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
  int getEquipCapacity( EquipmentSlotVariety * type);
  inline string getPluralName() const{return pluralName_;}
          void   setPluralName(const string name);
  virtual void printDescription(ReportPrinter & out);
  virtual void printTypeSpecificDescription(ReportPrinter & out){}
  inline   int getWeight() const {return weight_;}
	inline  void setWeight(int weight) {weight_ = weight;}
  virtual LEARNING_RESULT mayLearn(SkillRule * skill, UnitEntity * unit);
  virtual bool teacherRequired(SkillRule * skill, UnitEntity * unit);
  virtual bool mayMove(UnitEntity * unit);
  inline  EntityStatistics * getStats()  {return &stats;}
  inline int getCapacity(int modeIndex){return capacity_[modeIndex];}
	inline int getCapacity(MovementVariety * mode){return capacity_[mode];}
  virtual bool mayWork();
  virtual bool mayRectuit();
  virtual bool mayTrade();
  virtual bool mayHoldTitles(){return false;}
  virtual bool mayTransferFigures();
   vector< EquipSlot *>& getAllEquipmentSlots();
  /** Amounts of efforts teacher should spend in order to teach this student. */
  virtual int getLearningCapacity();
  virtual void extractKnowledge (Entity * recipient, int parameter = 0);
         int getProductionBonusValue(SkillRule * skill);
         int getStudyBonus(SkillRule * skill);
         int getLearningBonus(SkillRule * skill);
	protected:
    vector< EquipSlot *> equipmentSlots_;
    vector< SkillElement> intristicSkills_;
		MovementMode<int> capacity_;
	  string pluralName_;
    int controlPointsFraction_;
		int weight_;
    int hiringCost_;
    int hiringProbability_;
    int hiringMax_;
    EntityStatistics stats;
    private:
		SkillBonusComboAttribute skillBonuses_;

};
extern RaceRule       sampleRace;
extern RulesCollection <RaceRule>      races;
#endif
