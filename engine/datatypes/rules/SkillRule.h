/***************************************************************************
                          SkillRule.h  -  description
                             -------------------
    begin                : Tue Dec 10 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef SKILL_H
#define SKILL_H

#include "Rule.h"
#include "EntityStatistics.h"
#include "MovementMode.h"
#include "BasicUsingStrategy.h"
class SkillElement;
class SkillLevelElement;
class Entity;
class TokenEntity;
class BasicLearningStrategy;
class InventoryElement;
class ReportPattern;
class TeachingOffer;

class SkillRule : public Rule  {
public:

			SkillRule ( const string & keyword, GameData * parent) : Rule(keyword, parent) {}
      SkillRule ( const SkillRule * prototype );
      virtual STATUS     initialize      ( Parser *parser);
      GameData * createInstanceOfSelf();
      STATUS dataConsistencyCheck();
      void postInit();
      inline   string   getDescription(int level)  const
						{ return description_[currentLevel_];}
      inline   void   setDescription(const string  &description, int level)
						{ description_[currentLevel_] = description;}
      void printSkillDescription(int level, ostream & out);
      SkillElement * getMax();
      LEARNING_RESULT mayBeStudied(TokenEntity * unit);
      USING_RESULT     mayBeUsedBy(TokenEntity * unit);
      bool teacherRequired(TokenEntity * unit);
      void addLearningExperience(TokenEntity * unit, int exp);
      void addUsingExperience(TokenEntity * unit, int exp);
//      void addRecursiveLearningExperience(UnitEntity * unit, SkillElement & skill);
      int getLevel(int expPoints);
      int getMaxLevel();
      int getStudyCost(TokenEntity * const unit);
  /** Determines if current skill is in the tree growing from the given skill  */
          bool isDescendFrom(SkillRule * skill, int level);
          SkillLevelElement * getRequirement( int level) const;
          void addDerivative(SkillLevelElement * skill, int level);
          int  calculateLearningExperience(TokenEntity * unit, TeachingOffer * teacher);
          int  calculateUsingExperience(TokenEntity * unit);
  /** No descriptions */
          int  getLevelExperience(int level) const;
   inline EntityStatistics * getStats(int level)  {return &(stats_[level]);}
          void       getDerivatives (vector <SkillLevelElement *> & derivatives, int level);
          void printLevel(int level, ostream & out);
          string printLevel(int level);
   inline bool isCombatSkill(){return isCombat_;}
   inline bool isMagicSkill(){return isMagic_;}
   inline int  getCapacity(int modeIndex, int level){return (capacity_[level])[modeIndex];}
          USING_RESULT use(TokenEntity * unit, int & useCounter);
          void reportUse(USING_RESULT result, TokenEntity * unit);
   SkillRule * getBasicSkill();
          void extractKnowledge (Entity * recipient, int parameter = 0);
          int getUseDuration(TokenEntity * unit);
          InventoryElement * getItemRequired(TokenEntity * tokenEntity);
    inline static int getMaxSkillLevel()  {return maxSkillLevel;}
    protected:
	  static const int maxSkillLevel = 7;
    int currentLevel_;
    vector <SkillLevelElement *> requirement_;
    vector <vector <SkillLevelElement *> > derivatives_;
    vector <EntityStatistics> stats_;
    vector <BasicLearningStrategy *> learningParadigm_;
    vector <BasicUsingStrategy *> usingParadigm_;
    vector <string>   description_;
		vector < MovementMode<int> > capacity_;
	  vector <int> expPoints_;
	  vector <int> studyCost_;
    void initLevel_(int level);
    SkillElement * max_;
    bool isCombat_;
    bool isMagic_;
    GameData * targetType_;
//    int targetDistance
    private:
};
extern SkillRule      sampleSkill;
#include "RulesCollection.h"
extern RulesCollection <SkillRule>     skills;

#endif
