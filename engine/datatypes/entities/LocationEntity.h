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
#include "BonusElement.h"
#include "Rational.h"

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

class LocationEntity : public PhysicalEntity  {
    public:
      LocationEntity (const string & keyword, GameData * parent ) : PhysicalEntity(keyword, parent){}
      LocationEntity ( const LocationEntity * prototype );
      ~LocationEntity (){}
      STATUS  initialize      ( Parser *parser );
       void      save (ostream &out);
      GameData * createInstanceOfSelf();
  /** No descriptions */
  void setTerrain(const string &tag);
      void  print();

  /** Adds Unit to location */
  void addUnit(UnitEntity * unit);
  /** Removes Unit from location */
  void removeUnit(UnitEntity * unit);
  /** prints  report for Entity (stats, posessions, private events) */
  void report(FactionEntity * faction, ostream & out);
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
  int  getStudentCounter() const;
  void setStudentCounter(bool flag);
  int  getTeacherCounter() const;
  void setTeacherCounter(bool flag);
  bool ordersToBeRepeated() const;
  STATUS        prepareData();
  void dailyPreProcess();
  int getFactionalObservation(FactionEntity * faction);
  inline vector <UnitEntity *> & unitsPresent() {return units_;}   
  inline TerrainRule * getTerrain() const {return terrain_;}
  inline int getWages()      const {return wages_;}
  inline int getPopulation() const {return population_;}
  inline int getOptima()     const {return optima_;}
         BasicExit *  findExit(LocationEntity * dest);
         BasicExit *  findExit(DirectionVariety * dir);
         void harvestResource(ItemRule * item, Rational& num);
//         BasicExit *  findExit(TerrainRule * dest);


    protected:
    vector <UnitEntity *> units_;   
	  TerrainRule * terrain_;
    int optima_;
    int population_;
    RaceRule * race_;
    int wages_;
    int entertainment_;
    int taxes_;
    TitleRule * title_;
    UnitEntity * titleOwner_;
    BasicConflict * dailyConflict_;
    BasicConflict * monthlyConflict_;
    MarketStrategy * market_;
    int studentCounter_;
    int teacherCounter_;
	vector <BasicExit *>     exits_;
  vector <BonusElement *>      skillBonuses_;
  vector <SkillLevelElement *>  skillTeaching_;
  vector <ResourceElement *>   resources_;
    int totalMarketValue_;
      int climate_;
      int economy_;
      int x_;
      int y_;
//      int z_;
//	 vector <ReportRecord *> reports_;
    private:
};
typedef vector <ResourceElement *>::iterator ResourceIterator;
typedef vector <BonusElement *>::iterator BonusIterator;
typedef vector <SkillLevelElement *>::iterator TeachingIterator;
typedef vector <BasicExit*>::iterator ExitIterator;
typedef vector <UnitEntity *>::iterator  UnitIterator;
#endif
