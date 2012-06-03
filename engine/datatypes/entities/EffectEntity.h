/***************************************************************************
                          EffectEntity.h
                             -------------------
    begin                : Sun Jan 18 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef EFFECT_ENTITY_H
#define EFFECT_ENTITY_H

#include "TokenEntity.h"

/**Effect is temporary entity that may exist for some time and affect other entities.
  *@author Alex Dribin
  */
class FactionEntity;
class LocationEntity;
class EffectRule;

class EffectEntity : public TokenEntity  {
public: 
      EffectEntity (const string & keyword, Entity * parent ) : TokenEntity(keyword, parent){}
      EffectEntity ( const EffectEntity * prototype);
	   ~EffectEntity();
      virtual STATUS  initialize      ( Parser *parser );
       virtual void      save (ostream &out);
      virtual GameData * createInstanceOfSelf();
  //EffectEntity * createEffectImage(TokenEntity * referent);
  EffectEntity * createEffectImage(FactionEntity * referent, int observation);
  void updateImage(EffectEntity *effect);
       virtual void     preprocessData();
       virtual void     postProcessData();
       virtual void     dailyPreProcess();
       virtual void      produceFactionReport (FactionEntity * faction, ReportPrinter &out);
       virtual void      publicReport (int observation, ReportPrinter &out);
       virtual void      privateReport (ReportPrinter &out);
       virtual void     reportAppearence(FactionEntity * faction, ReportPrinter &out);
       virtual void     reportInventory(FactionEntity * faction, ReportPrinter &out);
       virtual void terminate();
       virtual string publicReport();
inline virtual void     setTarget(Entity * target){ target_ = target;}
inline virtual Entity * getTarget()const { return target_;}
inline void setEffectType(EffectRule * effectType){effectType_ = effectType;}
inline EffectRule * getEffectType(){return effectType_;}
inline int  getEffectStrength(){return strength_;}
inline void setEffectStrength(int strength){strength_ = strength;}
inline int  getEffectExpiration(){return expiration_;}
inline void setEffectExpiration(int expiration){expiration_ = expiration;}
  virtual void extractAndAddKnowledge(FactionEntity * recipient, int parameter = 0);
//  virtual void extractSkillKnowledge(Entity * recipient, int parameter = 0);

	protected:
       Entity * target_;
       EffectRule * effectType_;
        int expiration_;
        int strength_;
};

//#include "EntitiesCollection.h"
//extern EntitiesCollection  <EffectEntity>   effects;
extern EffectEntity         sampleEffectEntity;
typedef vector <EffectEntity *>::iterator EffectIterator;
#endif
