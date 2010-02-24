/* 
 * File:   EffectRule.h
 * Author: alex
 *
 * Created on December 28, 2009, 7:31 PM
 */

#ifndef _EFFECTRULE_H
#define	_EFFECTRULE_H
#include "Rule.h"
#include "RulesCollection.h"
class Entity;
class EffectEntity;
class LocationEntity;
/**Action types
  *@author Alex Dribin
  */

class EffectRule : public Rule {
public:
    EffectRule( const string & keyword, GameData * parent) : Rule(keyword, parent){}
    EffectRule(const EffectRule * prototype);
    virtual ~EffectRule();

       virtual STATUS     initialize      ( Parser *parser);
      //GameData * createInstanceOfSelf();
       virtual bool isToBeCreated(LocationEntity * location);
       virtual EffectEntity * createEffect(LocationEntity * location);
       virtual void deleteEffect(LocationEntity * location);
       virtual void printDescription(ReportPrinter & out);
       vector <AbstractData *> aPrint();
       virtual void terminateEffect(EffectEntity * entity);
       virtual void saveData(EffectEntity * entity, ostream &out);
       virtual void preProcessEffect(EffectEntity * entity);
       virtual void postProcessEffect(EffectEntity * entity);
       virtual void dailyPreProcessEffect(EffectEntity * entity){}
       virtual void reportEffectStart(EffectEntity * entity);
       virtual string reportPublicAppearance(EffectEntity * entity);
protected:
       //GameData * targetType_; // pointer to prototype
        string entityKeyword_;
        int trashold_;
        string publicAppearance_;
private:

};
extern RulesCollection <EffectRule>    effectRules  ;
typedef   vector <EffectRule>::iterator EffectRulesIterator;
extern  EffectRule      sampleEffectRule;
#endif	/* _EFFECTRULE_H */

