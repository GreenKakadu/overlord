/* 
 * File:   EffectRule.cpp
 * Author: alex
 * 
 * Created on December 28, 2009, 7:31 PM
 */

#include "EffectRule.h"
#include "StringData.h"
#include "EffectEntity.h"
#include "LocationEntity.h"
#include "entities/LocationEntity.h"

RulesCollection <EffectRule>      effectRules(new DataStorageHandler("effects.rules"));
EffectRule    sampleEffectRule    ("EFFECT_RULE",&sampleGameData);

EffectRule::EffectRule(const EffectRule * prototype): Rule(prototype)
{
   //  cout << " ---:---> EffectRule created"<<endl;
   // init here
    entityKeyword_ = "EFFECT";
    trashold_ = 100;
    publicAppearance_ = "This area is affected by ";
}

EffectRule::~EffectRule()
{
}

//GameData * EffectRule::createInstanceOfSelf()
//{
//  return CREATE_INSTANCE<EffectRule> (this);
//}
bool EffectRule::isToBeCreated(LocationEntity * location)
{
    return false;
}
EffectEntity * EffectRule::createEffect(LocationEntity * location)
{
  EffectEntity  * newEffect   = dynamic_cast<EffectEntity *> (createByKeyword(entityKeyword_));
  if(newEffect)
  {
    if(effects.addNew(newEffect) != OK)
      {
        cout << "Failed to add new effect \n";
        return 0;
      }
     newEffect->setEffectType(this);
     newEffect->setName(name_);
     location->addEffect(newEffect);
     newEffect->explicitInitialization();
  }
      cout << " ---:---> EffectRule createEffect "<<print()<<endl;
      reportEffectStart(newEffect);
 return newEffect;

}
void EffectRule::deleteEffect(LocationEntity * location)
{

}


STATUS
EffectRule::initialize        ( Parser *parser )
{
  GameData::initialize(parser);
     return OK;
}

void EffectRule::terminateEffect(EffectEntity * entity)
{
    // Remove effect from lists

}

void EffectRule::printDescription(ReportPrinter & out)
{
    out << print()<< ": "<< getDescription()<<".";
}

vector <AbstractData *> EffectRule::aPrint()
{
  vector <AbstractData *> v;
  v.push_back(this);
  v.push_back(new StringData(getDescription()));
  return v;
}

 void EffectRule::saveData(EffectEntity * entity, ostream &out)
 {
     cout << " ---:---> EffectRule saved"<<endl;
 }

void EffectRule::preProcessEffect(EffectEntity * entity)
{

}
void EffectRule::postProcessEffect(EffectEntity * entity)
{

}
void EffectRule::reportEffectStart(EffectEntity * entity)
{
 
}
string EffectRule::reportPublicAppearance(EffectEntity * entity)
{
  return publicAppearance_ + print() + ". ";
}
