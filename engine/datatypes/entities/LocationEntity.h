/***************************************************************************
                          LocationEntity.h  -  description
                             -------------------
    begin                : Sun Dec 8 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef LOCATION_ENTITY_H
#define LOCATION_ENTITY_H

#include "PhysicalEntity.h"
#include "TerrainRule.h"
//#include "BonusElement.h"
#include "Rational.h"
#include "ItemElement.h"

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

class LocationEntity : public Entity  {
    public:
      LocationEntity (const string & keyword, GameData * parent ) : Entity(keyword, parent){}
      LocationEntity ( const LocationEntity * prototype );
      ~LocationEntity (){}
      STATUS  initialize      ( Parser *parser );
       void      save (ostream &out);
      GameData * createInstanceOfSelf();
      void    preprocessData();
  inline void setTerrain(TerrainRule * terrain) {terrain_ = terrain;}

  /** Adds Unit to location */
  void addUnit(UnitEntity * unit);
  /** Removes Unit from location */
  void removeUnit(UnitEntity * unit);
  void addConstruction(ConstructionEntity * construction);
  void removeConstruction(ConstructionEntity * construction);
  /** prints  report for Entity (stats, posessions, private events) */
  void report(FactionEntity * faction, ReportPrinter & out);
  /** returns skill learning bonus that location gives */
  int getBonus(SkillRule * skill);
  void setResource(ItemElement);
  void setResource(ItemRule * item, int num);
  int  getResource(ItemRule * item);
  bool mayInterract(UnitEntity * unit);
  /** Some resources may be unavailable */
  Rational  getAvailableResource(ItemRule * item);
  void addDailyConflictRequest(BasicCompetitiveRequest * request);
  void addMonthlyConflictRequest(BasicCompetitiveRequest * request);
  void addMarketRequest(MarketRequest * request);
  void processDailyConflict();
  void processMonthlyConflict();
  void processMarket();
  void updateTotalMarketValue(const int value);
  void setMarketPrince(UnitEntity * prince);
  UnitEntity * getMarketPrince();
  int  getStudentCounter() const;
  void setStudentCounter(bool flag);
  int  getTeacherCounter() const;
  void setTeacherCounter(bool flag);
  bool ordersToBeRepeated() const;
  STATUS        prepareData();
  void dailyPreProcess();
  int getFactionalObservation(FactionEntity * faction);
  inline vector <UnitEntity *> & unitsPresent() {return units_;}
  inline vector <ConstructionEntity *> & constructionsPresent() {return constructions_;}
  inline TerrainRule * getTerrain() const {return terrain_;}
  inline int getWages()      const {return wages_;}
  inline int getPopulation() const {return population_;}
  inline int getOptima()     const {return optima_;}
         BasicExit *  findExit(LocationEntity * dest);
         BasicExit *  findExit(DirectionVariety * dir);
         void harvestResource(ItemRule * item, Rational& num);
  Rational  takeAvailableResource(ItemRule * item, Rational amount);
  //         BasicExit *  findExit(TerrainRule * dest);
         void setOwner(FactionEntity * owner);
  inline FactionEntity *  getOwner() const {return owner_;}
  inline int getTotalLand() const {return landTotal_;}
  inline int getFreeLand() const {return landFree_;}
          bool useLand(int landSize);
          void freeLand(int landSize);
  inline int getLandPrice() const {return landPrice_;}
  bool promoteUnit(UnitEntity * unit1,UnitEntity * unit2); 
  void addLocalItem(ItemRule * item, int number);
  void removeLocalItem(ItemRule * item, int number);
  int hasLocalItem(ItemRule * item);
// Titles ========================================================
          void addTitle(TitleElement * title);
          void removeTitle(TitleElement * title);
          void deleteTitle(TitleRule * titleType);
          TitleElement * findTitle(TitleRule * titleType);
    protected:
    vector <UnitEntity *> units_;
    vector <ConstructionEntity *> constructions_;
    vector < TitleElement *>      titles_;
    FactionEntity * owner_;
    TerrainRule * terrain_;
    int optima_;
    int population_;
    RaceRule * race_;
    int wages_;
    int entertainment_;
    int taxes_;
    BasicConflict * dailyConflict_;
    BasicConflict * monthlyConflict_;
    MarketStrategy * market_;
    int studentCounter_;
    int teacherCounter_;
	vector <BasicExit *>     exits_;
  vector <BonusElement *>      skillBonuses_;
  vector <SkillLevelElement *>  skillTeaching_;
  vector <ResourceElement *>   resources_;
  vector <ItemElement>   localItems_;
    int totalMarketValue_;
      int climate_;
      int economy_;
      int landPrice_;
      int landTotal_;
      int landFree_;
      int x_;
      int y_;
//      int z_;
//	 vector <ReportRecord *> reports_;
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
typedef vector <UnitEntity *>::iterator  UnitIterator;
typedef vector <ConstructionEntity *>::iterator  ConstructionIterator;
extern LocationEntity     sampleLocation;
#include "EntitiesCollection.h"
extern EntitiesCollection <LocationEntity>  locations;
#endif
