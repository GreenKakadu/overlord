/***************************************************************************
                             ResignOrder.cpp
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "ResignOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "FactionEntity.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "EntitiesCollection.h"
extern EntitiesCollection <FactionEntity>      factions;
extern ReportPattern * resignReporter;
ResignOrder * instantiateResignOrder = new ResignOrder();

ResignOrder::ResignOrder(){
  keyword_ = "Resign";
  registerOrder_();
  description = string("RESIGN faction-id \n") +
  "This orders resigns your faction.  All your units will revert to the faction\n" +
  "designated, which must be one of The Imperials [np1], Citizens [np2], Free\n" +
  "People [np4] or Outlaws [np5].  Your creature units revert to being members\n" +
  "of the Creatures faction [np6].  The units are considered given to the\n" +
  "faction, which will automatically accept them, even if no units of that\n" +
  "faction are located there, or the faction has declared you neutral or worse.\n" +
  "\n" +
  "The faction-id is mandatory.\n";

  orderType_   = IMMEDIATE_ORDER;
  mayInterrupt_ = true;
}

STATUS ResignOrder::loadParameters(Parser * parser,
                            vector <AbstractData *>  &parameters, Entity * entity )
{
    if(!parseGameDataParameter(entity,  parser, factions, "npc faction id", parameters))
            return IO_ERROR;
         return OK;
}



ORDER_STATUS ResignOrder::process (Entity * entity, vector <AbstractData *>  &parameters)
{
  FactionEntity * faction = dynamic_cast<FactionEntity *>(entity);
  if(parameters.size() == 0)
    faction->resign(0);
  else
    {
        FactionEntity *  target = dynamic_cast<FactionEntity *>(parameters[0]);
        assert(target);
        faction->resign(target);
    }
  return SUCCESS;
}
// To destroy faction - mark it as resigned and not save a data
