/***************************************************************************
                          EffectEntity.cpp 
                             -------------------
    begin                : Sun Jan 18 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "EffectEntity.h"
#include "LocationEntity.h"
#include "EntitiesCollection.h"

EffectEntity         sampleEffectEntity  ("EFFECT",  &sampleTokenEntity);
EntitiesCollection <EffectEntity>   effects(new DataStorageHandler(gameConfig.getEffectsFile()));

EffectEntity::EffectEntity( const  EffectEntity* prototype ) : TokenEntity(prototype)
{
}


EffectEntity::~EffectEntity(){
}



GameData * EffectEntity::createInstanceOfSelf()
{
   return CREATE_INSTANCE<EffectEntity> (this);
}


STATUS
EffectEntity::initialize        ( Parser *parser )
{
  if (parser->matchKeyword("LOCATION"))
	{
	  setLocation(locations[ parser->getWord()]);
	  return OK;
	}
    
	return TokenEntity::initialize(parser );
}



void    EffectEntity::preprocessData()
{
}



/*
 * Monthly post-processing
 */
void EffectEntity::postProcessData()
{
}



void      EffectEntity::save (ostream &out)
{

}
void   EffectEntity::produceFactionReport (FactionEntity * faction, ReportPrinter &out)
{

}



void   EffectEntity::publicReport (int observation, ReportPrinter &out)
{

}



void   EffectEntity::privateReport (ReportPrinter &out)
{

}



void   EffectEntity::reportAppearence(FactionEntity * faction, ReportPrinter &out)
{

}



void   EffectEntity::reportInventory(FactionEntity * faction, ReportPrinter &out)
{

}






