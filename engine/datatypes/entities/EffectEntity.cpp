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
//EntitiesCollection <EffectEntity>   effects(new DataStorageHandler(gameConfig.getEffectsFile()),&sampleEffectEntity);

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
	  setLocation(gameFacade->locations[ parser->getWord()]);
	  return OK;
	}
  if (parser->matchKeyword("TYPE"))
	{
	  effectType_ = gameFacade->effectRules[ parser->getWord()];
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
// Create token object as it is seen by referent
// Take into account observation. (imageLevel = obse)
// for allied units effective observation may be higher
//EffectEntity *     EffectEntity::createEffectImage(FactionEntity * referent, int observation)
//{
//  TokenEntity  * token = createTokenImage(referent,observation);
//  if(token == 0)
//  {
//      return 0;
//  }
//  EffectEntity  * effect = dynamic_cast<EffectEntity *> (token) ;
//  effect->setLocation(this->getLocation());
//  effect->effectType_ = this->effectType_;
//  return effect;
//}


// Create token object as it is seen by referent
// Take into account observation. (imageLevel = obse)
// for allied units effective observation may be higher
EffectEntity *     EffectEntity::createEffectImage(FactionEntity * referent, int observation)
{
  TokenEntity  * token = createTokenImage(referent,observation);
  if(token == 0)
  {
      return 0;
  }
  EffectEntity  * effect = dynamic_cast<EffectEntity *> (token) ;
  effect->setLocation(this->getLocation());
  effect->effectType_ = this->effectType_;
  return effect;
}


// Update token image with the data from another image
void EffectEntity::updateImage(EffectEntity * effect)
{
 //    if(image->getTimeStamp() < this->getTimeStamp())
//    {
//        return;
//    }
	  this->TokenEntity::updateImage(effect);
	  this->setLocation(effect->getLocation());
	  this->effectType_ = effect->effectType_;

}

// Here Expiration and
void     EffectEntity::dailyPreProcess()
{
	if(isUnknownEntity())
	{
		return;
	}
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
	if(isUnknownEntity())
	{
		return;
	}
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

void EffectEntity::extractAndAddKnowledge(FactionEntity * recipient, int parameter)
{

}

//void EffectEntity::extractSkillKnowledge(Entity * recipient, int parameter)
//{
//
//}





