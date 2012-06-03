/***************************************************************************
                          UnitEntity.h
                             -------------------
    begin                : Tue Nov 13 18:21:12 IST 2001
    copyright            : (C) 2001 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

#ifndef UNIT_H
#define UNIT_H
#include "TokenEntity.h"
#include "SkillElement.h"
#include "SkillLevelElement.h"
#include "ReportPrinter.h"
#include "TitlesAttribute.h"

class FactionEntity;
class LocationEntity;
class RaceRule;
class ItemRule;
class ReportPattern;
class BasicExit;
class TravelElement;
class RaceElement;
class SkillUseElement;
class ConstructionEntity;
class ConstructionRule;
class TitleElement;
class InventoryElement;

class UnitEntity : public TokenEntity
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
  void    dailyPreProcess();
  STATUS  dataConsistencyCheck();
  void    save (ostream &out);
  void    save(ostream &out, string prefix);
  void    dailyUpdate();
  bool    defaultAction();
  void    postProcessData();
  void    payUpkeep();
//  BattleInstance * createBattleInstantiation();
// Reporting ==============================================
  void    produceFactionReport(FactionEntity * faction, ReportPrinter &out);
  void    publicReport(int observation, ReportPrinter &out);
  void    privateReport(ReportPrinter &out);
  void    reportAppearence(FactionEntity * faction, ReportPrinter &out);
  void    reportInventory(FactionEntity * faction, ReportPrinter &out);
  void    reportEnchantments(FactionEntity * faction, ReportPrinter &out);
  void    reportSkills(FactionEntity * faction, ReportPrinter &out);
  void    reportFlags(ReportPrinter &out);
// Data access methods ==============================================

	 int              getObservation() const;
	 int              getStealth() const;
	 int              getWeight();
	 int              getCapacity(int modeIndex);
	 int              getCapacity(MovementVariety * mode);
	 RaceRule *       getRace() const;
   inline RaceElement    * getRaceComposition(){return raceComposition_;}
	 void 		setRace(RaceRule * race, int number);
	 void 		changeRace(RaceRule * race);
	 int        getFiguresNumber() const;
	 int 		getControlPoints() const;
	 int 		getUpkeep();
   inline UnitEntity *     getLeader() const {return stackFollowingTo_;}
   inline ConstructionEntity * getContainingConstruction() const {return containingConstruction_ ;}
   inline void setContainingConstruction(ConstructionEntity * containingConstruction)  
					{ containingConstruction_ = containingConstruction;}
    bool recursiveSetContainingConstruction(ConstructionEntity* containingConstruction);
	  void enterConstruction(ConstructionEntity * containingConstruction) ;
	  void exitConstruction() ;
	 inline int getDamage() const {return stats.getDamage();}
	 inline int getRangedDamage() const {return stats.getRangedDamage();}
	 inline DAMAGE_TYPE getDamageType() const { return stats.getDamageType();}
//	 DAMAGE_TYPE modifyDamageType(DAMAGE_TYPE value){}
	 inline int getLife() const {return stats.getLife();}
         inline int getInitiative() const {return stats.getInitiative();}
	 inline int getMelee() const {return stats.getMelee();}
	 inline int getMissile() const {return stats.getMissile();}
	 inline int getDefence() const {return stats.getDefence();}
	 inline int getHits() const {return stats.getHits();}
	      string printComposition();
	      Rule * getComposition();

// Inventory methods ==============================================

/** Updates number of equiped items after decreasing of number of figures in unit */
         vector < InventoryElement>    updateEquipement();
         int     hasMoney();                    // These two items play special
  inline int     hasMana()  {return mana_;} // role.
  inline void    setMana(int mana)  {mana_ = mana;} 
         int     equipItem(ItemRule * item, int num);
         int     mayBorrow(ItemRule * item, int amount);
         int     borrow(ItemRule * item, int amount);
	void 	 dropUnequippedItems();
// Stacking ========================================================

                       /** Stacks under new leader */
  friend  void            stack(UnitEntity * unit, UnitEntity * newLeader);
                       /** Returns the leader of current unit if any or 0 */
  inline  UnitEntity *    getPrevStack() const {return stackFollowingTo_;} //for traversing up
          UnitEntity *    getStackHead(); 
  bool            unstack();
  void          unstackAll();
                       /** returns list of all units following current unit */
 vector< UnitEntity *> &createStackMembersList(vector< UnitEntity *> &followers);
          void printStackMembersList(FactionEntity * faction, ReportPrinter &out, bool isOwnStackOnly = false);
                       /** Applies all movement effects to all units in stack */
          void            setStackMoving(TravelElement * moving) ;
                       /** recursively calculates weight of all units in stack */
          int             calculateTotalWeight (int & weight);
               /** recursively calculates total capacity of all units in stack */
          void            calculateTotalCapacity(int & capacity, int modeIndex);
          void            calculateTotalCapacity(int & capacity, MovementVariety * mode);
          bool            isLeading(UnitEntity * unit);
          bool            isFollowingInStackTo(UnitEntity * unit);
          void            moveGroupToLocation();
          void            movingGroupArrived();
          void            movingGroupReport(ReportRecord report);
          bool            addStackLandwalkExperience(bool newLevel);
          bool            leaveStaying();
          void           enterStack(ConstructionEntity * containingConstruction);
          void            exitStack();
          bool            promoteUnit(UnitEntity * unit1,UnitEntity * unit2);
          bool            isFollowingForeignUnit();
          bool              isStackLeader();
          bool              hasFollowers();
          vector< UnitEntity *> & getAllFollowers(){ return stackFollowers_;}
// Skills ========================================================


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
          vector < SkillLevelElement *> getMayLearnList();
// Titles ========================================================
          void addTitle(TitleElement * title);
          void removeTitle(TitleElement * title);
          bool claimTitle(TitleElement * title);
	  bool mayCancelTitle(TitleElement * title);
	 inline TitlesAttribute * getTitlesAttribute()  {return &titles_;}
// Bonuces ========================================================
 		int getProductionBonus(SkillRule * skill);
//		int getSkillLearningBonus(SkillRule * skill);
//		int getItemLearningBonus(SkillRule * skill);
//		int getTitleLearningBonus(SkillRule * skill);
//		int getEnchantmentLearningBonus(SkillRule * skill);
		int getSkillStudyBonus(SkillRule * skill);
		int getItemStudyBonus(SkillRule * skill);
		int getTitleStudyBonus(SkillRule * skill);
		int getEnchantmentStudyBonus(SkillRule * skill);
//		int getSkillProductionBonus(SkillRule * skill);
//		int getItemProductionBonus(SkillRule * skill);
//  		int getTitleProductionBonus(SkillRule * skill);
  int calculateMovementBonus(MovementVariety * mode);
// Flags ========================================================
  void setConsuming(bool value) {consuming_ = value;}
 // bool getConsuming() {return consuming_;}
  void setDiscontenting(bool value) {discontenting_ = value;}
 // bool getDiscontenting() {return discontenting_;}
  inline bool isDiscontent(){return discontenting_;}
  inline bool isConsuming(){return consuming_;}

// Other ========================================================
  void sufferDamage(int value);
  void  doOath();
  void  setEntityMoving(TravelElement * moving);
  Rule * getType();
  void disband();
  void recalculateStats();
  bool retreat();
  bool pay(int price);
  bool mayPay(int price);
  bool maySee(TokenEntity * tokenEntity);
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
  void moveToLocation(LocationEntity * destination);
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
   bool mayGuard(bool enableReport)  ;
   int getAttackRating() const;
   int getDefenceRating() const;
   int getObservationRating() const;
   int getStealthRating();
   EntityStatistics  getBasicStats();
    void drownUnit();
    void addEffect(EffectEntity * effect);
    EnchantmentElement * hasEnchantment(EnchantmentRule * rule);
  virtual void extractAndAddKnowledge(Entity * recipient, int parameter = 0);
//  virtual void extractSkillKnowledge(Entity * recipient, int parameter = 0);
// ReportPatterns ========================================================
   void copyPrivateImage(UnitEntity * image, UnitEntity  * source);
   UnitEntity *  makePrivateImage();
  virtual void makeAlliedImage(TokenEntity * source);
  virtual void makeObservedImage(TokenEntity * source);
  virtual UnitEntity * createUnitImage(FactionEntity * referent, int observation);
 // UnitEntity * createUnitImage(TokenEntity * referent);
  void updateImage(UnitEntity *unit);
    protected:
         ConstructionEntity  * containingConstruction_;
         UnitEntity     * stackFollowingTo_;
         RaceElement    * raceComposition_;
         bool             staying_;
//         bool             patroling_;
         bool             exposeFlag_;
         bool 	          consuming_;
         bool 	          discontenting_;
         bool             isAssignedToStaff_;
  vector < UnitEntity *>      stackFollowers_;
  TitlesAttribute      titles_;
	int mana_;
    private:

};
typedef vector <UnitEntity *>::iterator StackIterator;
typedef vector <UnitEntity *>::iterator  UnitIterator;
extern UnitEntity     sampleUnit;
//#include "EntitiesCollection.h"
//extern EntitiesCollection <UnitEntity>      units;

#endif

