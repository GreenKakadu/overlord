/***************************************************************************
                          ItemRule.h  -  description
                             -------------------
    begin                : Tue Dec 10 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef ITEM_H
#define ITEM_H

#include "Rule.h"
#include "EquipmentSlotVariety.h"
#include "MovementMode.h"
#include "EntityStatistics.h"
#include "SkillBonusComboAttribute.h"
class EquipmentSlotVariety;
class SkillLevelElement;
class BasicCondition;
class UnitEntity;
class CombatActionStrategy;

class ItemRule : public Rule  {
public:
      ItemRule ( const string & keyword, GameData * parent);
      ItemRule ( const ItemRule * prototype );
      virtual STATUS     initialize      ( Parser *parser);
      void save(ostream &out);
      GameData * createInstanceOfSelf();
  inline string getPluralName() const{return pluralName_;}
    void printDescription(ReportPrinter & out);
    vector <AbstractData *> aPrint();
  inline EquipmentSlotVariety * getEquipSlot() const {return equipSlot_ ;}
  /** gets nimber of equipement slots item demands to be equiped */
  inline int getNumEquipSlotsRequired() const {return numEquipSlotsRequired_;}
  inline int getWeight() const {return weight_;}
  inline int getFormalPrice() const {return price_;}
  inline  EntityStatistics * getStats()  {return &stats;}
  inline  BasicCondition * demandsEquipCondition(){return equipCondition_;}
  inline int getCapacity(MovementVariety * mode){return capacity_[mode];}
  inline int getCapacity(int modeIndex){return capacity_[modeIndex];}
  inline int getEquipCapacity(int modeIndex){return equipCapacity_[modeIndex];}
  inline int getEquipCapacity(MovementVariety * mode){return equipCapacity_[mode];}
        void extractKnowledge (Entity * recipient, int parameter = 0);
        void applyEquipementEffects(UnitEntity * unit, int number);
  inline CombatActionStrategy * getCombatAction() {return combatAction_;}
         int getProductionBonusValue(SkillRule * skill);
         int getStudyBonus(SkillRule * skill);
         int getLearningBonus(SkillRule * skill);
  inline bool isUnique() {return unique_;}
  inline SkillLevelElement * getProspectSkill(){return prospectSkill_;}
  inline bool isRegeneratingResource(){return isRegeneratingResource_;}
    protected:
	  string pluralName_;
	  EquipmentSlotVariety * equipSlot_;
		int numEquipSlotsRequired_ ;
		int weight_ ;
		int price_ ;
		BasicCondition * equipCondition_;
		SkillLevelElement *useSkill_;
		SkillLevelElement *prospectSkill_;
		bool unique_;
		bool special_;
		bool magic_;
		bool live_;
        bool isRegeneratingResource_;
	MovementMode<int> capacity_;
    MovementMode<int> equipCapacity_;
    EntityStatistics stats;
	CombatActionStrategy * combatAction_;
	SkillBonusComboAttribute skillBonuses_;
    private:
};
extern ItemRule * cash; // special value
extern ItemRule * food; // special value
extern ItemRule       sampleItem;
//extern RulesCollection <ItemRule>      items;
#endif
