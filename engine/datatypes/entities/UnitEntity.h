/***************************************************************************
                          UnitEntity.h  
                             -------------------
    begin                : Tue Nov 13 18:21:12 IST 2001
    copyright            : (C) 2001 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef UNIT_H
#define UNIT_H
#include "PhysicalEntity.h"
#include "SkillElement.h"
#include "SkillLevelElement.h"
#include "EntityStatistics.h"
#include "MovementMode.h"
#include "ReportPrinter.h"
class FactionEntity;
class LocationEntity;
class RaceRule;
class ItemRule;
class Reporter;
class BasicExit;
class TravelElement;
class RaceElement;
class SkillUseElement;
class ConstructionEntity;
class ConstructionRule;
class TitleElement;
class InventoryElement;

class UnitEntity : public PhysicalEntity
{
    public:
      UnitEntity (const string & keyword, GameData * parent );
      UnitEntity ( const UnitEntity * prototype );
      ~UnitEntity (){}
  GameData * createInstanceOfSelf();
  UnitEntity * createUnit (FactionEntity * faction, RaceRule * race,
                  int number, LocationEntity * location);
  STATUS  initialize      ( Parser *parser );
  void    preprocessData();
  STATUS  dataConsistencyCheck();
  void    save (ostream &out); 
  void    dailyUpdate();
  bool    defaultAction();
  void      postProcessData();
// Reporting ==============================================
  void    report(FactionEntity * faction, ReportPrinter &out);
  void    publicReport(int observation, ReportPrinter &out);
  void    privateReport(ReportPrinter &out);
  void    reportAppearence(FactionEntity * faction, ReportPrinter &out);
  void    reportInventory(FactionEntity * faction, ReportPrinter &out);
  void    reportSkills(FactionEntity * faction, ReportPrinter &out);
// Data access methods ==============================================
 
         int              getObservation() const;
         int              getStealth() const;
         int              getWeight();
         int              getCapacity(int modeIndex);
         RaceRule *       getRace() const;
         void setRace(RaceRule * race, int number);
  inline UnitEntity *     getLeader() const {return stackFollowingTo_;}
         int              getFiguresNumber() const;
   inline ConstructionEntity * getContainingConstruction() const {return containingConstruction_ ;}
   inline void setContainingConstruction(ConstructionEntity * containingConstruction)  { containingConstruction_ = containingConstruction;}
          void enterConstruction(ConstructionEntity * containingConstruction) ;
          void exitConstruction() ;
   inline  int              getControlPoints();
// Inventory methods ==============================================
     
        /** Updates number of equiped items after decreasing of number of figures in unit */
         vector < InventoryElement *>    updateEquipement();
         int     hasMoney();                                  // These two items play special
  inline int     hasMana()  {return hasItem(items["mana"]);} // role. Demand special care?
         int     equipItem(ItemRule * item, int num);
// Stacking ========================================================

                          /** Stacks under new leader */
  friend  void            stack(UnitEntity * unit, UnitEntity * newLeader);
                          /** Returns the leader of current unit if any or 0 */
  inline  UnitEntity *    getPrevStack() const {return stackFollowingTo_;} //for traversing up
          bool            unstack();
                          /** returns list of all units following current unit */
  vector< UnitEntity *> & createStackMembersList(vector< UnitEntity *> & followers) ;
                          /** Applies all movement effects to all units in stack */
          void            setStackMoving(TravelElement * moving) ;
                          /** recursively calculates weight of all units in stack */
          int             calculateTotalWeight (int & weight);
                          /** recursively calculates total capacity of all units in stack */
          void            calculateTotalCapacity(int & capacity, int modeIndex);
          bool            isLeading(UnitEntity * unit);
          bool            isFollowingInStackTo(UnitEntity * unit);
          void            moveGroupToLocation();
          void            movingGroupArrived();
          void            movingGroupReport(ReportRecord * report);
          bool            addStackLandwalkExperience(bool newLevel);
          bool            leaveStaying();
          void            enterStack(ConstructionEntity * containingConstruction);
          void            exitStack();
          bool            promoteUnit(UnitEntity * unit1,UnitEntity * unit2); 
// Skills ========================================================


         void addSkillExperience(SkillRule  * skill, int expPoints);
//         bool hasTeacher(SkillRule  * skill);
         void mergeUnits(int number, UnitEntity * unit);
              /** Add new unskilled figures */
         void addNewFigures(int number);
              /** returns basic skill - the first skill unit learned */
         SkillRule  * getBasicSkill();
              /** returns second basic skill for units that have  */
         SkillRule  * getSecondaryBasicSkill();
          int getLearningCapacity();
         bool teacherRequired(SkillRule * skill) ;
         LEARNING_RESULT mayLearn(SkillRule * skill);
          int getLearningLevelBonus(SkillRule * skill);
// Titles ========================================================
          void addTitle(TitleElement * title);
          void removeTitle(TitleElement * title);
          bool claimTitle(TitleElement * title);
void addLearningLevelBonus(SkillLevelElement * bonus);
void removeLearningLevelBonus(SkillLevelElement * bonus);
int getTitleBonus(SkillRule * skill);
bool mayCancelTitle(TitleElement * title);
// Other ========================================================
  void  doOath();
  void  setEntityMoving(TravelElement * moving);
  Rule * getType();
  void disband();  
  void recalculateStats();
  bool retreat();
  bool pay(int price);
  bool mayPay(int price);
  bool maySee(PhysicalEntity * tokenEntity);
  bool mayInterract(UnitEntity * unit);
  bool mayInterract(ConstructionEntity * buildingOrShip);
  bool mayBuild (ConstructionRule * construction);
  bool mayHoldTitles();
  bool mayMove();
  void moveArrive();
  bool moveAdvance();
  void setStaying(); 
  void clearStay(); // may be these are not needed
  bool isStaying();
  void moveToLocation();
  void movingEntityArrived();
  bool addLandwalkExperience();
  bool work();
  void setFigures(int number);
  bool isExposed() const;
  void expose(bool value);
  bool isHidden();
  inline void setHidden(bool value)  {isAssignedToStaff_ = value;}
  inline bool isStaff() const {return  isAssignedToStaff_;}
         bool isOfType(Rule * type);
		// Removes all experience in given skill and all it's derivatives
		void forgetSkill(SkillRule * skill);
   inline bool isAccepted(UnitEntity * unit) {return isLeading(unit);}
// Reporters ========================================================

  
    protected:
         ConstructionEntity  * containingConstruction_;
         UnitEntity     * stackFollowingTo_;
         RaceElement    * raceComposition_;
         EntityStatistics stats;
         bool             staying_;
         bool             patroling_;
         bool             exposeFlag_;
         bool             isAssignedToStaff_;
  vector < UnitEntity *>      stackFollowers_;
  vector < TitleElement *>      titles_;
  vector < SkillLevelElement *>      learningLimitBonus_;
    private:

};
typedef vector <UnitEntity *>::iterator StackIterator;
extern UnitEntity     sampleUnit;
#include "EntitiesCollection.h"
extern EntitiesCollection <UnitEntity>      units;

#endif

