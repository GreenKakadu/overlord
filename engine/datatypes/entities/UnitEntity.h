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
#include "InventoryElement.h"
#include "EntityStatistics.h"
#include "MovementMode.h"
class FactionEntity;
class LocationEntity;
class RaceRule;
class ItemRule;
class Reporter;
class BasicExit;
class TravelElement;
class RaceElement;

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
  void    print();
  void    dailyUpdate();
  bool    defaultAction();
  
// Reporting ==============================================
  void    report(FactionEntity * faction, ostream &out);
  void    publicReport(int observation, ostream &out);
  void    reportAppearence(FactionEntity * faction, ostream &out, int indent);
  void    reportInventory(FactionEntity * faction, ostream &out, int indent);
// Data access methods ==============================================
 
         FactionEntity *  getFaction() const;
         int              getObservation() const;
         int              getStealth() const;
         /** Return pointer to  Entity which keeps reports from this  */
         Entity *         getReportDestination();
         int              getWeight();
         int              getCapacity(int modeIndex);
         RaceRule *       getRace() const;
         void setRace(RaceRule * race, int number);
//  inline LocationEntity * getLocation() const{return location_;}
         LocationEntity * getGlobalLocation() const;
  inline UnitEntity *     getLeader() const {return stackFollowingTo_;}
         int              getFiguresNumber() const;
  inline bool             isGuarding() const { return guarding_;}
  inline bool             isTraced() const {return traced_;}  /** used for debugging */
  inline void             setFaction( FactionEntity * faction) {faction_ = faction;}
  inline bool             isMoving() const {return moving_;} 
  inline void             setUnitMoving(TravelElement * moving) {moving_ = moving;}
         bool             isUnaccessible() const;

// Inventory methods ==============================================
     
         void    addToInventory(ItemRule * item, Rational& num);
         void    addToInventory(ItemRule * item, int num);
         int     takeFromInventory(ItemRule * item, int num);
         bool    takeFromInventoryExactly(ItemRule * item, int num);
         int     hasItem(ItemRule * item) ;
         int     hasEquiped(ItemRule * item) ;
         Rational     getItemAmount(ItemRule * item) ;
        /** Updates number of equiped items after decreasing of number of figures in unit */
         vector < InventoryElement >    updateEquipement();
         int     equipItem(ItemRule * item, int num);
         int     hasMoney();                                  // These two items play special
  inline int     hasMana()  {return hasItem(items["mana"]);} // role. Demand special care?
  vector < InventoryElement > & getAllInventory();
  void giveAllInventory(UnitEntity * unit);

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
          int             calculateStackWeight (int & weight);
                          /** recursively calculates total capacity of all units in stack */
          void            calculateStackCapacity(int & capacity, int modeIndex);
          bool            isAccepting(UnitEntity * unit);
          bool            isLeading(UnitEntity * unit);
          void            clearAccept(UnitEntity * unit);
          void            accept(UnitEntity * unit);
          void            moveStackToLocation();
          void            movingStackArrived();
          bool            addStackLandwalkExperience(bool newLevel);
          bool            stayStack();

// Skills ========================================================

         vector < SkillElement>& getAllSkills(); 
         bool hasSkill(SkillRule  * skill, int level) ;
  inline bool hasSkill(SkillLevelElement * skill) 
        {return hasSkill(skill->getSkill(), skill->getSkill()->getLevelExperience(skill->getLevel()));}

  inline bool hasSkill(SkillElement  * skill)
        {return hasSkill(skill->getSkill(), skill->getExpPoints());}

         int  getSkillLevel(SkillRule  * const skill);
         int  getSkillPoints(SkillRule  * const skill);
         int  addSkill(SkillElement  skill);
         int  addSkill(SkillRule  * skill, int expPoints);
         void addSkillExperience(SkillRule  * skill, int expPoints);
         void gainNewLevel(SkillRule * skill, int newLevel);
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
// Other ========================================================

  void disband();  
  void recalculateStats();
  bool retreat();
  bool pay(int price);
  bool mayPay(int price);
  bool mayInterract(UnitEntity * unit);
  bool isDisbanded();
  void moveArrive();
  bool moveAdvance();
  void stay();
  void clearStay(); // may be these are not needed
  bool isStaying();
  void moveUnitToLocation();
  void movingUnitArrived();
  bool addLandwalkExperience();
  int calculateTravelTime(int time, int weight, int capacity);
  bool work();
  void setFigures(int number);
  bool isExposed() const;
  void expose(bool value);

// Reporters ========================================================

  
    protected:
         FactionEntity  * faction_;
//         LocationEntity * location_;
         TravelElement   * moving_;
         UnitEntity     * stackFollowingTo_;
         RaceElement    * raceComposition_;
         EntityStatistics stats;
				 bool             traced_;
         bool             guarding_;
         bool             staying_;
         bool             patroling_;
         bool             exposeFlag_;
  vector < UnitEntity *>      stackFollowers_;
  vector < InventoryElement > inventory_; 
  vector < SkillElement>      skills_;
	vector < UnitEntity *>      accepting_;
    private:

};
typedef vector <UnitEntity *>::iterator StackIterator;
typedef vector <InventoryElement >::iterator InventoryElementIterator;
typedef vector <SkillElement>::iterator SkillIterator;

#endif

