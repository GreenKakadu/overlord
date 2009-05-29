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

class EffectEntity : public TokenEntity  {
public: 
      EffectEntity (const string & keyword, Entity * parent ) : TokenEntity(keyword, parent){}
      EffectEntity ( const EffectEntity * prototype );
	   ~EffectEntity();
      STATUS  initialize      ( Parser *parser );
       void      save (ostream &out);
      GameData * createInstanceOfSelf();
       void     preprocessData();
       void     postProcessData();
       void      produceFactionReport (FactionEntity * faction, ReportPrinter &out);
       void      publicReport (int observation, ReportPrinter &out);
       void      privateReport (ReportPrinter &out);
       void     reportAppearence(FactionEntity * faction, ReportPrinter &out);
       void     reportInventory(FactionEntity * faction, ReportPrinter &out);
inline void     setTarget(Entity * target){ target_ = target;}
inline Entity * getTarget()const { return target_;}
	protected:
       Entity * target_;
};
#include "EntitiesCollection.h"
extern EntitiesCollection  <EffectEntity>   effects;

#endif
