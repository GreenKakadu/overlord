/***************************************************************************
                             StanceOrder.cpp 
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "StanceOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "FactionEntity.h"
#include "UnaryPattern.h"
#include "StanceVariety.h"
#include "BinaryPattern.h"
#include "TertiaryPattern.h"
#include "EntitiesCollection.h"
#include "VarietiesCollection.h"
extern EntitiesCollection <UnitEntity>      units;
extern EntitiesCollection <FactionEntity>      factions;
extern VarietiesCollection <StanceVariety>      stances;
extern Reporter *	missingParameterReporter;
extern Reporter *	invalidParameterReporter;
extern Reporter *	AtReporter;

StanceOrder * instantiateStanceOrder = new StanceOrder();

StanceOrder::StanceOrder(){
  keyword_ = "Stance";
  registerOrder_();
  description = string("STANCE  faction-id |unit-id stance \n") +
  "Immediate, one-shot.  Modifies your diplomatic stance toward a faction or unit.\n" +
  "The stance must be one of:\n" +
  "\"ALLY\", \"FRIENDLY\", \"NEUTRAL\", \"HOSTILE\", or \"ENNEMY\".  If the faction-id is\n" +
  "your own faction identifier, this modifies your default stance.  If not, it\n" +
  "will modify your stance toward the specified faction.\n" +
  "Stances are kept private.  You can only guess which stances are declared for\n" +
  "you by the behaviour of units within the game, and your report's friendly\n" +
  "marks.\n";
  orderType_   = IMMEDIATE_ORDER;
  mayInterrupt_ = true;
}

STATUS StanceOrder::loadParameters(Parser * parser,
                            vector <AbstractData *>  &parameters, Entity * entity )
{
//   if(!entityIsUnit(entity))
//            return IO_ERROR;


    parseOptionalGameDataParameter(entity,  parser, factions,parameters);
    parseOptionalGameDataParameter(entity,  parser, units, parameters);

    if(!parseGameDataParameter(entity,  parser,  stances, "stance tag", parameters) )
      {
       entity->addReport(new BinaryPattern(missingParameterReporter, 
 					new StringData(keyword_), new StringData("stance tag"))); 		
        return IO_ERROR;
      }     

  StanceVariety * stance = dynamic_cast<StanceVariety *>(parameters[parameters.size()-1]);
  if(stance == 0)
  {
        entity->addReport(new TertiaryPattern(invalidParameterReporter, new StringData(keyword_), new StringData(parameters[parameters.size()-1]->printName()), new StringData("stance")));
        return IO_ERROR;
  }
  return OK;

}



ORDER_STATUS StanceOrder::process (Entity * entity, vector <AbstractData *>  &parameters)
{
  FactionEntity * faction = dynamic_cast<FactionEntity *>(entity);
  if(faction == 0)
  	{
  		PhysicalEntity * unit = dynamic_cast<PhysicalEntity *>(entity);
  		if(unit)
  			faction = unit->getFaction();
  		else
  			return INVALID;
  	}

  StanceVariety * stance = dynamic_cast<StanceVariety *>(parameters[parameters.size()-1]);
  assert(stance);
  if(parameters.size() > 1)
  {	
  	Entity * target = dynamic_cast<Entity *>(parameters[0]);
  	assert(target);	
  	faction->setStance(target,stance);
  }
  else	
  	faction->setDefaultStance(stance);

  return SUCCESS;

}
