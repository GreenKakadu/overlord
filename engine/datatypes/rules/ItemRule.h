/***************************************************************************
                          ItemRule.h  -  description
                             -------------------
    begin                : Tue Dec 10 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef ITEM_H
#define ITEM_H

#include "Rule.h"
#include "EquipmentSlotVariety.h"
#include "MovementMode.h"
#include "EntityStatistics.h"
class EquipmentSlotVariety;
class SkillLevelElement;
class BasicCondition;
class UnitEntity;

class ItemRule : public Rule  {
public:
      ItemRule ( const string & keyword, GameData * parent);
      ItemRule ( const ItemRule * prototype );
      virtual STATUS     initialize      ( Parser *parser);
      GameData * createInstanceOfSelf();
  inline string getPluralName() const{return pluralName_;}
    void printDescription(ReportPrinter & out);
  inline EquipmentSlotVariety * getEquipSlot() const {return equipSlot_ ;}
  /** gets nimber of equipement slots item demands to be equiped */
  inline int getNumEquipSlotsRequired() const {return numEquipSlotsRequired_;}
  inline int getWeight() const {return weight_;}
  inline int getFormalPrice() const {return price_;}
  inline  EntityStatistics * getStats()  {return &stats;}
  inline  BasicCondition * demandsEquipCondition(){return equipCondition_;}
  inline int getCapacity(int modeIndex){return capacity_[modeIndex];}
  inline int getEquipCapacity(int modeIndex){return equipCapacity_[modeIndex];}
        void extractKnowledge (Entity * recipient, int parameter = 0);
        void applyEquipementEffects(UnitEntity * unit, int number);
    protected:
	  string pluralName_;
	  EquipmentSlotVariety * equipSlot_;
		int numEquipSlotsRequired_ ;
		int weight_ ;
		int price_ ;
		BasicCondition * equipCondition_;
		SkillLevelElement *useSkill_;
		SkillLevelElement *learningLevelBonus_;
		bool unique_;
		bool special_;
		bool magic_;
		bool live_;
		MovementMode<int> capacity_;
    MovementMode<int> equipCapacity_;
    EntityStatistics stats;
    private:
};
extern ItemRule * cash; // special value
extern ItemRule * food; // special value
extern ItemRule       sampleItem;
#include "RulesCollection.h"
extern RulesCollection <ItemRule>      items;
#endif
