/***************************************************************************
                          EffectEntity.cpp 
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
#include "EffectRule.h"
#include "EffectEntity.h"
#include "LocationEntity.h"
#include "EntitiesCollection.h"

EffectEntity         sampleEffectEntity  ("EFFECT",  &sampleTokenEntity);
EntitiesCollection <EffectEntity>   effects(new DataStorageHandler(gameConfig.getEffectsFile()));

EffectEntity::EffectEntity( const  EffectEntity* prototype) : TokenEntity(prototype)
{
    effectType_ = 0;
    expiration_=0;
    strength_ =0;
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
  if (parser->matchKeyword("TYPE"))
	{
	  effectType_ = effectRules[ parser->getWord()];
          if(effectType_ == 0)
          {
              cerr<<"Effect of undefined type"<<endl;
          }
	  return OK;
	}
        if (parser->matchKeyword("STRENGTH"))
 	{
          strength_= parser->getInteger();
 	  return OK;
 	}
        if (parser->matchKeyword("EXPIRATION"))
 	{
          expiration_= parser->getInteger();
 	  return OK;
 	}
	return TokenEntity::initialize(parser );
}


// Here Expiration and
void     EffectEntity::dailyPreProcess()
{
   effectType_->dailyPreProcessEffect(this);
}


void    EffectEntity::preprocessData()
{
    effectType_->preProcessEffect(this);
}



/*
 * Monthly post-processing
 */
void EffectEntity::postProcessData()
{
        effectType_->postProcessEffect(this);
        if(expiration_ ==0)
        {
            terminate();
        }
        expiration_--;
}

void EffectEntity::terminate()
{
   effectType_->terminateEffect(this);
   this->getLocation()->removeEffect(this);
}

string EffectEntity::publicReport()
{
    return effectType_->reportPublicAppearance(this);
}



void      EffectEntity::save (ostream &out)
{
        if(expiration_ ==0)
        {
            return;
        }

  out << endl;
  TokenEntity::save(out);
        out<<"EXPIRATION "<< getEffectExpiration() <<endl;

    if(this->getLocation())
    {
        out<<"LOCATION "<< getLocation()->getTag()<<endl;
    }
   if(effectType_)
    {
        out<<"TYPE "<< effectType_->getTag()<<endl;
        effectType_->saveData(this,out);
    }

    if(getEffectStrength())
    {
        out<<"STRENGTH "<< getEffectStrength() <<endl;
    }




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






