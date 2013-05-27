/***************************************************************************
                          LocationEntity.h  -  description
                             -------------------
    begin                : Sun Dec 8 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

#ifndef LOCATION_ENTITY_H
#define LOCATION_ENTITY_H

#include "TokenEntity.h"
#include "TerrainRule.h"
//#include "BonusElement.h"
#include "RationalNumber.h"
#include "ItemElement.h"
#include "OwnershipPolicy.h"
#include "SkillBonusAttribute.h"
#include "MovementBonusAttribute.h"
#include "TitlesAttribute.h"

class  UnitEntity;
class  FactionEntity;
class  BasicExit;
class  TitleRule;
class  ItemRule;
class  DirectionVariety;
class  BasicConflict;
class  BasicCompetitiveRequest;
class  ResourceElement;
class  MarketRequest;
class  MarketStrategy;
class  RaceRule;
class  ConstructionEntity;
//class  ItemElement;
class  TitleElement;
class  BonusElement;
class  BasicCombatManager;
class WeatherRule;
class SeasonRule;
class EffectEntity;
class EffectRule;
class TurnReport;

class LocationEntity : public Entity  {
    public:
      LocationEntity (const string & keyword, GameData * parent ) : Entity(keyword, parent){}
      LocationEntity ( const LocationEntity * prototype );
      ~LocationEntity (){}
      STATUS  initialize      ( Parser *parser );
       void   save (ostream &out);
      GameData * createInstanceOfSelf();
      void    preprocessData();
      void    postProcessData();
      void    postPostProcessData();
      void    dailyUpdate();
// Static configuration parameters for economic model
      static const int migrationFactor;
      static const int pillagingFactor;
      static const int recoveryFactor;
      static const int growthFactor;
      static const int battleFactor;

  /** Adds Unit to location */
  void addUnit(UnitEntity * unit);
  void addUnitImmediatelly(UnitEntity * unit);
  void addConstruction(ConstructionEntity * construction);
  void addConstructionImmediatelly(ConstructionEntity * construction);
  void addAllAddedConstructions();
  void addAllAddedUnits();

  /** Removes Unit from location */
  void removeUnit(UnitEntity * unit);
  void eraseRemovedUnit(UnitEntity * unit);
  void removeConstruction(ConstructionEntity * construction);
  void eraseRemovedConstruction(ConstructionEntity * construction);
  void eraseAllRemovedConstructions();
  void eraseAllRemovedUnits();
  // =========== Resources ======================
  void generateResourses();
  void setResource(ItemElement);
  void setResource(ItemRule * item, int num);
  int  getResource(ItemRule * item);
  void addResource(ItemRule * item, int num);
  vector <ResourceElement *> & getResources(){return resources_;}
  /** Some resources may be unavailable */
  RationalNumber  getAvailableResource(ItemRule * item);
  void setAvailableResource(ItemRule * item, int num);
  void harvestResource(ItemRule * item, RationalNumber& num);
RationalNumber  takeAvailableResource(ItemRule * item, RationalNumber amount);
  void cleanResourses();
  // ====== Conflicts =============
  void addDailyConflictRequest(BasicCompetitiveRequest * request);
  void addMonthlyConflictRequest(BasicCompetitiveRequest * request);
  void addMarketRequest(MarketRequest * request);
  void processDailyConflict();
  void processMonthlyConflict();
  void processMarket();
   int getLocalBuyPrice(ItemRule * item);
   int getLocalSellPrice(ItemRule * item);
   int getLocalRecruitPrice(RaceRule * race);
  void updateTotalMarketValue(const int value);
  void setMarketPrince(UnitEntity * prince);
  UnitEntity * getMarketPrince();
  int  getStudentCounter() const;
  void setStudentCounter(bool flag);
  int  getTeacherCounter() const;
  void setTeacherCounter(bool flag);
  vector <SkillLevelElement *> & getAllSkillTeaching(){return this->skillTeaching_;}
  //==================================
  /** prints  report for Entity (stats, posessions, private events) */
   void produceFactionReport(FactionEntity * faction, ReportPrinter & out);
   /** returns skill learning bonus that location gives */
   int getBonus(SkillRule * skill);
   bool mayInterract(UnitEntity * unit);
  SkillBonusAttribute & getAllSkillBonuses() { return skillBonuses_;}
  bool ordersToBeRepeated() const;
  STATUS        prepareData();
  void dailyPreProcess();
  int getFactionalObservation(FactionEntity * faction);
UnitEntity * getFactionalObserver(FactionEntity * faction);
  inline vector <UnitEntity *> & unitsPresent() {return units_;}
  inline vector <ConstructionEntity *> & constructionsPresent() {return constructions_;}
  // Economy
  inline int getEconomy() const {return economy_;}
  inline void setEconomy(int value)  { economy_= value;}
  inline int getWages()      const {return wages_;}
  inline void setWages(int value)       {wages_=value;}
  inline int getTaxes()      const {return taxes_;}
  inline int getPopulation() const {return population_;}
  inline void setPopulation(int value)  {population_ =value;}
  inline int getPopulationExcess() const {return populationExcess_;}
  inline int getOptima()     const {return optima_;}
  inline void setOptima(int value)    { optima_ = value;}
  inline int getMigration()     const {return migration_;}
  inline RaceRule * getRace(){return race_;}
  inline vector <ItemElement> & getLocalItems(){return   localItems_;}
  inline MarketStrategy * getMarket(){return market_;}
  inline void setMigration(int value) { migration_ =value;}
 // ===  Geography
  inline TerrainRule * getTerrain() const {return terrain_;}
  inline void setTerrain(TerrainRule * terrain) {terrain_ = terrain;}
  LocationEntity * findAdjacientLocation( int x, int y);
         BasicExit *  findExit(LocationEntity * dest);
         BasicExit *  findExit(DirectionVariety * dir);
         void addExit(BasicExit * exit);
  inline int getX() const {return x_;}
  inline void setX(int value)  { x_=value;}
  inline int getY() const {return y_;}
  inline void setY(int value)  { y_=value;}
  inline vector <BasicExit *>&  getAllExits() {return   exits_;}
  //         BasicExit *  findExit(TerrainRule * dest);

  inline LocationEntity * getOuterLocation(){ return outerLocation_;}
  inline map <string, LocationEntity *> & getInnerLocation(){ return innerLocations_;}
  void checkInnerLocations();
  // Ownership
         void setLegalOwner(FactionEntity * owner, LocationEntity * titleLocation);
         void setGuard(TokenEntity * guard);
         bool tokenAllowedToEnter(TokenEntity * traveler, 
                    MovementVariety * movingMode, TokenEntity * patrol = 0);
         bool unitMayPillage(UnitEntity * unit, bool enableReport);
         TokenEntity * getGuard() const;
         TokenEntity * getBlockingPatrol(TokenEntity * traveler,
                    MovementVariety *    movingMode, StanceVariety * stance);
         TokenEntity *  selectNewGuard();
         void checkNewOwnerConflicts(TokenEntity * newOwner);
  inline FactionEntity *  getLegalOwner() const {return owner_;}
         FactionEntity *  getRealOwner() const;
  inline int getTotalLand() const {return landTotal_;}
  inline void setTotalLand(int value)  { landTotal_= value;}
  inline bool isPillaged() const {return isPillaged_;}
  inline void setPillaged(bool status)  { isPillaged_ = status;}
  void pillage();
  void addBattle();
  inline int getFreeLand() const {return landFree_;}
          bool useLand(int landSize);
          void freeLand(int landSize);
  inline int getLandPrice() const {return landPrice_;}
  inline void setLandPrice(int value) { landPrice_ = value;}
  OwnershipPolicy & getOwnershipPolicy(){return ownershipPolicy_;}

  bool promoteUnit(UnitEntity * unit1,UnitEntity * unit2); 
  void addLocalItem(ItemRule * item, int number);
  void removeLocalItem(ItemRule * item, int number);
  int hasLocalItem(ItemRule * item);
   BasicCombatManager * getCombatManager() const {return combatManager_;}
  virtual void extractAndAddKnowledge(FactionEntity * recipient, int parameter = 0);
//  virtual void extractSkillKnowledge(Entity * recipient, int parameter = 0);
// Teaching
//    void addLocationTeachingOffer(TeachingOffer *offer);
//    TeachingOffer * findLocationTeachingOffer(SkillRule  * skill, int level);
//    void cleanLocationTeachingOfers();
// Weather and seasons ========================================================
	 WeatherRule * getWeather() const;
	 void setWeather(WeatherRule * weather);
         WeatherRule * getNextWeather() const {return nextWeather_;}
         void setNextWeather(WeatherRule * weather) {nextWeather_ = weather;}
	 SeasonRule * getSeason() const;
	 SeasonRule * determineSeason();
	 int getMovementBonus(MovementVariety * mode);
// Titles ========================================================
	 void setTitleLocation(LocationEntity * titleCenter){titleCenter_ = titleCenter;}
	 LocationEntity * getTitleLocation() const {return titleCenter_;}
          void addTitle(TitleElement * title);
          void removeTitle(TitleElement * title);
          void deleteTitle(TitleRule * titleType);
          TitleElement * findTitle(TitleRule * titleType);
          void turnNpcGuards();
          inline TitlesAttribute &  getTitles(){return titles_;}
// Effects ========================================================
          void  addEffect(EffectEntity * effect);
          void removeEffect(EffectEntity * effect);
          EffectEntity * findEffect(EffectRule * rule);
    protected:
    vector <TokenEntity *> visitors_;
    vector <UnitEntity *> units_;
    vector <UnitEntity *> unitsToRemove_;
    vector <UnitEntity *> unitsToAdd_;
    vector <ConstructionEntity *> constructions_;
    vector <ConstructionEntity *> constructionsToRemove_;
    vector <ConstructionEntity *> constructionsToAdd_;
    vector <EffectEntity *> effects_;
    BasicCombatManager * combatManager_;
    TitlesAttribute      titles_;
    OwnershipPolicy ownershipPolicy_;
    FactionEntity * owner_;
    TokenEntity * guard_;
    TerrainRule * terrain_;
    WeatherRule * weather_;
    WeatherRule * nextWeather_;
    SeasonRule * season_;
    LocationEntity * titleCenter_;
    bool isPillaged_;
    bool isInitialized_;
    int optima_;
    int population_;
    RaceRule * race_;
    int wages_;
    int entertainment_;
    int taxes_;
    int populationExcess_;
    int migration_;
    BasicConflict * dailyConflict_;
    BasicConflict * monthlyConflict_;
    MarketStrategy * market_;
    int studentCounter_;
    int teacherCounter_;
  vector <BasicExit *>     exits_;
  SkillBonusAttribute  skillBonuses_;
  vector <SkillLevelElement *>  skillTeaching_;
  vector <ResourceElement *>   resources_;
  vector <ItemElement>   localItems_;
    int totalMarketValue_;
   MovementBonusAttribute movementBonuses_;
   LocationEntity * outerLocation_;
   map <string, LocationEntity *>  innerLocations_;
//      int climate_;
      int economy_;
      int landPrice_;
      int landTotal_;
      int landFree_;
      int x_;
      int y_;
//      int z_;
//	 vector <ReportRecord> reports_;
// LocatioImage create and maintain
public:
      LocationEntity * createIncompleteVisitedImage(FactionEntity * referent);
      LocationEntity * createVisitedLocationImage(FactionEntity * referent, int observation);
      LocationEntity * createBasicImage(FactionEntity * referent);
      void addNeighbours(FactionEntity * target);
      void addNeighbours(TurnReport * report);
      void updateImage(LocationEntity *image);
      LocationEntity * makeVisitedImage(LocationEntity * image, LocationEntity * source);
      void populateLocationImage(FactionEntity * referent, int observation);
      LocationEntity * produceDailyImages(FactionEntity * faction);
private: 
    bool isVisited_;
    int imageObservation_;
//=======================================================================
// For PathFinding
    public:
      static int findWalkingDistance(LocationEntity * start, LocationEntity * end, int maxDistance);
     void examineNeighboringLocations(int &currentDistance,                    
                    vector <LocationEntity *> &openList,
                    vector <LocationEntity *> & examinedLocations);
      inline void markClosed() { closed_ = true;}
      inline void clearClosed() { closed_ = false;}
      inline bool isClosed(){return closed_;}
      inline  int getDistance() {return totalDistance_;}
      inline void addDistance(int dist) { totalDistance_ += dist;}
      inline void setDistance(int dist) { totalDistance_ = dist;}
    protected:
      int totalDistance_;
      bool closed_;
//      vector <LocationEntity *> path_;
//=======================================================================
    private:
};
typedef LocationEntity * locPtr;
extern bool betterDistance( const locPtr & loc1, const locPtr & loc2);
typedef vector <BonusElement *>::iterator BonusIterator;
typedef vector <SkillLevelElement *>::iterator TeachingIterator;
typedef vector <BasicExit*>::iterator ExitIterator;
typedef vector <ConstructionEntity *>::iterator  ConstructionIterator;
extern LocationEntity     sampleLocation;
//#include "EntitiesCollection.h"
//extern EntitiesCollection <LocationEntity>  locations;
#endif
