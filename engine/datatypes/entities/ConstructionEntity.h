/***************************************************************************
                          ConstructionEntity.h
                             -------------------
    begin                : Sun Aug 31 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef CONSTRUCTION_ENTITY_H
#define CONSTRUCTION_ENTITY_H

#include "TokenEntity.h"

/**Ships and buildings entities
  *@author Alex Dribin
  */
class ConstructionRule;
class FactionEntity;
class LocationEntity;
class ConstructionWorksElement;
class ConstructionWorksVariety;
class UnitEntity;
class ResourceElement;
class SkillRule;
class ItemRule;

class ConstructionEntity : public TokenEntity  {
public: 
      ConstructionEntity (const string & keyword, Entity * parent ) : TokenEntity(keyword, parent){}
      ConstructionEntity ( const ConstructionEntity * prototype );
	   ~ConstructionEntity();
      STATUS  initialize      ( Parser *parser );
       void      save (ostream &out);
       void      produceFactionReport (FactionEntity * faction, ReportPrinter &out);
       void      publicReport (int observation, ReportPrinter &out);
       void      privateReport (ReportPrinter &out);
       void    reportAppearence(FactionEntity * faction, ReportPrinter &out);
       void    reportInventory(FactionEntity * faction, ReportPrinter &out);
      GameData * createInstanceOfSelf();
       void    preprocessData();
       void    postProcessData();
//       void      payUpkeep();
       inline LocationEntity * getLocation()const {return location_;}
       inline void setLocation(LocationEntity * location){location_ = location;}
       inline void initializeBuildingWorks(vector<ConstructionWorksElement *> buildingWorks) {buildingWorks_  = buildingWorks;}
      bool addBuildingWork(ConstructionWorksElement * buildingWorks);
      int  workToDo(ConstructionWorksVariety * buildingWorksType);
      inline void setConstructionType(ConstructionRule * construction) {construction_ = construction;}
      inline ConstructionRule * getConstructionType() /*const*/ {return construction_;}
      bool isCompleted() const;
      bool isUnaccessible() const;
      int addSkill(SkillElement  skill);
      int addSkill(SkillRule  * skill, int expPoints);
      void buildingCompleted();
      int getStealth() const;
      void addUnit(UnitEntity * unit);
      void removeUnit(UnitEntity * unit);
      void destroy();
      bool mayMove();
      bool mayInterract(UnitEntity * unit);
      int getLandUse();
      int getProductionBonus(ItemRule * item, SkillRule * skill,int level);
      void setProductionBonus(ItemRule * product, SkillRule * skill,int level);
      RationalNumber useProductionBonus(ItemRule * product, RationalNumber number);
//      void recalculateCapacity();
     int getCapacity(int modeIndex);
     bool leaveStaying();
     bool isOfType(Rule * type);
     int  calculateTotalWeight (int & weight);
     void calculateTotalCapacity(int & capacity, int modeIndex);
     void moveToLocation();
     void moveGroupToLocation();
     void movingGroupArrived();
     void movingEntityArrived();
     void movingGroupReport(ReportRecord * report );
     void addStaff(UnitEntity * unit);
     void removeStaff(UnitEntity * unit);
     inline int getEffectiveStaff() {return effectiveStaff_;}
     bool mayUseAsStaff(UnitEntity * unit);
     bool isObservableBy(FactionEntity * faction);
    int calculateSkill(SkillRule *  skill);
    bool recalculateSkills();
     LEARNING_RESULT mayLearn(SkillRule * skill);
     bool isOnBoard(UnitEntity * unit);
     inline bool isAccepted(UnitEntity * unit) {return isOnBoard(unit);}

    protected:
    vector <UnitEntity *> units_;
    vector <UnitEntity *> staff_;
    ConstructionRule * construction_;
    vector<ConstructionWorksElement *>    buildingWorks_;
    vector <ResourceElement *>   resourceQuotas_;
    int effectiveStaff_;
};
extern ConstructionEntity   sampleConstructionEntity;
#include "EntitiesCollection.h"
extern EntitiesCollection <ConstructionEntity>  buildingsAndShips;

#endif
