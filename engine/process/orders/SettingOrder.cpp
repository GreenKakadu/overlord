/***************************************************************************
                             SettingOrder.cpp
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "SettingOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "FactionEntity.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "EntitiesCollection.h"
extern ReportPattern *	invalidParameterReporter;
extern ReportPattern *	missingParameterReporter;
extern ReportPattern *	AtReporter;

SettingOrder * instantiateSettingOrder = new SettingOrder();

SettingOrder::SettingOrder(){
  keyword_ = "Setting";
  registerOrder_();
  description = string("SETTING [setting-name] [ON|OFF] \n") +
  "Immediate, one-shot. Modifies a unit or faction setting.  Always executes.\n" +
  "\n" +
  "Setting-name must be:\n" +
  "\n" +
  "ADVERTISE: Causes the tokenEntity to show its faction affiliation to any unit\n" +
  "capable of seeing the tokenEntity.\n" +
  "\n" +
  "ANNOUNCE: Causes the tokenEntity to reveal its presence to any unit capable of\n" +
  "observing the location, as if its stealth factor was infinitely negative.\n" +
  "\n" +
  "CONSUME: Causes the unit to try to consume for the upkeep available food.\n" +
  "\n" +
  "SILENT: Causes the tokenEntity to be silent about its daily activities. All\n" +
  "normal activities, such as skill use, give and get items, go unreported,\n" +
  "making for a shorter turn report.\n" +
  "\n" +
  "SHARE: Causes the tokenEntity to share it's food and edible items with other\n" +
  "units at the same location.\n" +
  "\n" +
  "SUPPORT: Causes the tokenEntity to try to withdraw from the faction funds to pay\n" +
  "for the upkeep first, regardless of the amount of cash in the area.\n" +
  "\n" +
  "TERSE: Select terse battle reports.  The default is OFF, meaning you'll get\n" +
  "detailed reports.\n";

  orderType_   = IMMEDIATE_ORDER;
  mayInterrupt_ = true;
}

STATUS SettingOrder::loadParameters(Parser * parser,
                            vector <AbstractData *>  &parameters, Entity * entity )
{
	if(parseSetting(parser,parameters,"TERSE","ON"))
		return OK;

   if(!entityIsTokenEntity(entity))
            return IO_ERROR;

	if(parseSetting(parser,parameters,"ADVERTISE","ON"))
		return OK;

	if(parseSetting(parser,parameters,"ANNOUNCE","ON"))
		return OK;

	if(parseSetting(parser,parameters,"SILENT","ON"))
		return OK;

	if(parseSetting(parser,parameters,"SHARE","ON"))
		return OK;

	if(parseSetting(parser,parameters,"SUPPORT","ON"))
		return OK;


   if(!entityIsUnit(entity))
            return IO_ERROR;

	if(parseSetting(parser,parameters,"CONSUME","ON"))
		return OK;

  return OK;

}



ORDER_STATUS SettingOrder::process (Entity * entity, vector <AbstractData *>  &parameters)
{
	if (parameters.size() < 2)
    {
    	// Missing parameters
    }
    string par1 = parameters[0]->print();
    string par2 = parameters[1]->print();
    bool settingValue = false;
    if(par2 == string("ON"))
    	settingValue = true;

    if(par1 == string("TERSE"))
    {
    	FactionEntity * faction = dynamic_cast<FactionEntity *>(entity);
    	if(faction)
    		{
    			faction->setTerseBattleReport(settingValue);
				return SUCCESS;
       		}
		TokenEntity * tokenEntity = dynamic_cast<TokenEntity *>(entity);
    	if(tokenEntity)
    		{
    			tokenEntity->getFaction()->setTerseBattleReport(settingValue);
				return SUCCESS;
       		}
		return INVALID;
    }

  TokenEntity * tokenEntity = dynamic_cast<TokenEntity *>(entity);
  assert(tokenEntity);

   if(par1 == string("ADVERTISE"))
   	{
    	tokenEntity->setAdvertising(settingValue);
		return SUCCESS;
   	}
   if(par1 == string("ANNOUNCE"))
   	{
    	tokenEntity->setAnnouncing(settingValue);
		return SUCCESS;
   	}
   if(par1 == string("SILENT"))
   	{
    	tokenEntity->setSilent(settingValue);
		return SUCCESS;
   	}
   if(par1 == string("SHARE"))
   	{
    	tokenEntity->setSharing(settingValue);
		return SUCCESS;
   	}
   if(par1 == string("SUPPORT"))
   	{
    	tokenEntity->setWithdrawingSupport(settingValue);
		return SUCCESS;
   	}
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
   if(par1 == string("CONSUME"))
   	{
    	unit->setConsuming(settingValue);
		return SUCCESS;
   	}
		return INVALID;
}



bool SettingOrder::parseSetting(Parser * parser, vector <AbstractData *>  &parameters,
							const char *setting,  const char * defaultSetting)
{
  if(parser ->matchKeyword(setting))
	{
      	parameters.push_back( new StringData (setting));
		if(parser ->matchKeyword("ON"))
		{
      		parameters.push_back( new StringData ("ON"));
      		return true;
		}
		if(parser ->matchKeyword("OFF"))
		{
      		parameters.push_back( new StringData ("OFF"));
      		return true;
		}
      	parameters.push_back(new StringData (defaultSetting));
      	return true;
	}
	// missing parameter
	return false;
}



//bool SettingOrder::getSettingParameter(vector <AbstractData *>  &parameters,
//											const char *setting, bool &value)
//{
//	  if (parameters.size() > 2)
//    {
//      string par = parameters[1]->print();
//      if(par == string("FREE"))
//        {
//           claimFreeTitleOnly = true;
//           cout<< "Claiming free title only set \n";
//          }
//    }
//
//}
