/***************************************************************************
                          OrderPrototype.cpp
                    .
                             -------------------
    begin                : Tue Nov  5 11:46:00 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "OrderPrototype.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "FactionEntity.h"
#include "StringData.h"
#include "IntegerData.h"
#include "GameConfig.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "OrderPrototypesCollection.h"
ReportPattern *	invalidOrderReporter = new ReportPattern ("invalidOrderReporter");
extern ReportPattern *	invalidParameterReporter;
extern ReportPattern *	missingParameterReporter;
extern ReportPattern *	unknownParameterReporter;
extern OrderPrototypesCollection  * orderPrototypesCollection;


OrderPrototype::OrderPrototype()
{
 orderType_ =DEFAULT;
 mayInterrupt_ = false;
 fullDayOrder_= false;
 initiative_ = 0;//
 isSequentive_ = false;//
}



/*
 * All orders should register themself in OrderPrototypesCollection
 * if OrderPrototypesCollection doesn't exist yet it should be created.
 */

bool OrderPrototype::registerOrder_()
{
  if(orderPrototypesCollection == 0)
    orderPrototypesCollection = new  OrderPrototypesCollection();
 if(orderPrototypesCollection->find(keyword_) ==0)
 {
    orderPrototypesCollection->add(this);
//    cout << "Order  [" << keyword_  << "]  registered \n";
    return true;
 }
 else
//  cout << "Order  [" << keyword_  << "] was already registered once\n";
 return false;
}



/*
 * All orders know how to save themself into entity's data and orders template
 */

STATUS
OrderPrototype::save(ostream &out)
{
  out << ' ' << keyword_ << ' ';
  return OK;
}

/*
 * Interface: All orders know how to parse, interpret and check their parameters
 */

STATUS
OrderPrototype::loadParameters(Parser * , ParameterList &, Entity * )
{
  return OK;
}




/*
 * Interface: All orders know how to process themlelf
 */

ORDER_STATUS
OrderPrototype::process(Entity * , vector < AbstractData*>  & )
{
  return SUCCESS;
}



/*
 * Interface: Some orders may require two-stage processing. Here is second stage.
 */

ORDER_STATUS
OrderPrototype::completeOrderProcessing (Entity * entity,
							ParameterList &parameters, int result)
{
  return FAILURE;
}



/*
 * Some orders may be executed only once per day.
 * Determines if this order is of sich kind
 */
bool OrderPrototype::isFullDayOrder()
{
  return fullDayOrder_;
//  if((orderType_ == STACK_ORDER) ||(orderType_ == DAY_LONG_ORDER))
//    return true;
//    else
//    return false;
}


/*
 * Sometimes order may not be processed for some reasons.
 * Entity may be unable to follow orders (dead, demoralized, paralized)
 * Entity may be busy (moving)
 * Determines if order may be processed
 */
bool OrderPrototype::mayBeProcessed(ProcessingMode * processingMode,
									Entity * entity)
{
  	if ( !processingMode-> mayExecute(orderType_))
		return false;

    if(entity->isUnaccessible())
		return false;

    if(entity->isBusy() && !mayInterrupt())
		  return false;

    return true;
}


/*
 * Some orders may be processed even while entity is busy
 * Determines if this order may be executed while entity is busy
 */
bool OrderPrototype::mayInterrupt()
{
  return mayInterrupt_;
}


/*
 * Checks that entity is unit. Used for unit-only orders
 */
bool OrderPrototype::entityIsUnit(Entity *entity, PARSING_MODE mode)
{
   UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  if(unit==0)  // Wrong Entity type
				{
         if(mode == NORMAL_PARSING)
          entity->addReport(new BinaryMessage(invalidOrderReporter, new StringData(keyword_), new StringData("units")));
         return false;
				}
   return true;
}


/*
 * Checks that entity is unit or construction.
 */
bool OrderPrototype::entityIsTokenEntity(Entity *entity, PARSING_MODE mode)
{
   TokenEntity * unit = dynamic_cast<TokenEntity *>(entity);
  if(unit==0)  // Wrong Entity type
				{
         if(mode == NORMAL_PARSING)
          entity->addReport(new BinaryMessage(invalidOrderReporter, new StringData(keyword_), new StringData("units or constructions")));
         return false;
				}
   return true;
}


/*
 * Checks that entity is faction. Used for faction-only orders
 */
bool OrderPrototype::entityIsFaction(Entity *entity, PARSING_MODE mode )
{
   FactionEntity * faction = dynamic_cast<FactionEntity *>(entity);
  if(faction==0)  // Wrong Entity type
				{
         if(mode == NORMAL_PARSING)
          entity->addReport(new BinaryMessage(invalidOrderReporter, new StringData(keyword_), new StringData("factions")));
         return false;
				}
   return true;
}


/*
 * Tries to get a word from the parser and to interpret it as a tag
 * of GameData object stored in collection.
 * parameterTypeName is provided for error reporting.
 */
bool OrderPrototype::parseGameDataParameter(Entity *entity, Parser * parser,
				BasicCollection & collection, const string & parameterTypeName,
				ParameterList &parameters)
{
   return parseGameDataParameter(entity, parser->getWord(),collection,parameterTypeName,parameters);
}



/*
 * Tries to interpret a "tag" as a tag of GameData object stored in collection.
 * parameterTypeName is provided for error reporting.
 * Take into account that tag parameter may be also new unit placeholder.
 * This procedure performs also check of parameter validity.
 */
bool OrderPrototype::parseGameDataParameter(Entity *entity,const string & tag, BasicCollection & collection, const string & parameterTypeName, ParameterList &parameters)
{
   if (tag.size() == 0)  // Missing parameter
        {
        entity->addReport(new BinaryMessage(missingParameterReporter, new StringData(keyword_), new StringData(parameterTypeName)));
         return false;
        }

   if (!collection.checkDataType(tag)) // this doesn't look like a tag  but it still may be new tag
       {
           if(!gameConfig.isNewEntityName(tag))
 				  {
            entity->addReport(new TertiaryMessage(invalidParameterReporter, new StringData(keyword_), new StringData(tag), new StringData(parameterTypeName)));
            return false;
				  }

       }
  return (checkParameterTag(entity, tag,  collection, parameters));
}



/*
 * Performs  check of parameter validity.
 * Check includes:
 * - Checking parameter as possible new entity placeholder
 * We don't want to let player to detect valid unknown tags so
 * no error on unexisting tag will be returned but we'll give him
 * warning when he uses unknown tag
 */
bool OrderPrototype::checkParameterTag(Entity *entity, const string & tag,
			BasicCollection & collection, ParameterList &parameters)
{
//   cout << "[][]== checking tag "<<tag << endl;
   GameData* item = collection.findByTag(tag,false);
  if( item == 0) // Data doesn't exist but it may be new entity placeholder
		{
     if(gameConfig.isNewEntityName(tag))
      {
       NewEntityPlaceholder * placeholder = collection.findOrAddPlaceholder(tag);
       //    cout <<"placeholder=  "<< (int) placeholder <<endl;
       if(placeholder != 0)  // this is  placeholder.
         {
           GameData* realEntity = placeholder->getRealEntity();
           //   cout <<"realEntity=  "<< (int) entity <<endl;
           if(realEntity) // We can get real entity id from placeholder
   		          parameters.push_back(realEntity);
           else   // placeholder is still empty
   		          parameters.push_back(placeholder);
           return true;
          }
       }
     // this is not placeholder.
		 //Entity doesn't exist but we don't want to let player to know that

     // Give warning if this is unknown rule

   	 Rule * rule = dynamic_cast<Rule*>(item);
   	 if(rule)
          {
            UnitEntity * unit = dynamic_cast<UnitEntity*>(entity);
            if(unit)
              {
                if(!unit->getFaction()->hasKnowledge(rule))
                {
                  entity->addReport(new UnaryMessage(unknownParameterReporter,
                      new StringData(tag)));
                }
               }
            }
    	StringData * dummy = new StringData(tag);
   		parameters.push_back(dummy);
    	return true;
    }
   else
				{
   		     parameters.push_back(item);
				}
   return true;
}



/*
 * Tries to get a word from the parser and to interpret it as an integer
 */
bool OrderPrototype::parseIntegerParameter(Parser * parser,
										ParameterList &parameters)
{
  if(parser -> matchInteger())
		{
      parameters.push_back( new IntegerData (parser->getInteger()));
      return true;
		}
  return false;
}

/*
 * Tries to parse a word and to interpret is as a tag of GameData object
 * stored in collection. Return no error if this interpretation fails
 * This procedure performs also check of parameter validity.
 */
bool OrderPrototype::parseOptionalGameDataParameter(Entity *entity,
					Parser * parser, BasicCollection & collection,
					ParameterList &parameters)
{
   string tag = parser->matchWord();
   if (tag.size() == 0)
        {
         return false;
        }

  if (!collection.checkDataType(tag)) // this can't be a tag
				{
         return false;
				}
  if(!checkParameterTag(entity, tag,  collection, parameters))
				{
         return false;
				}
  parser->getWord();
  return true;
}


/*
 * Tries to interpret parIndex'th parameter in order parameters as integer
 * returns value or 0 on error.
 */
int OrderPrototype::getIntegerParameter(ParameterList &parameters,
										unsigned int parIndex)
{
  IntegerData * par;
  if(parameters.size() > parIndex)
    {
      par       =  dynamic_cast<IntegerData *>(parameters[parIndex]);
      assert(par);
      return (par->getValue());
    }
   return 0;
}



bool OrderPrototype::parseStringParameter(Entity *entity, Parser * parser,
						ParameterList &parameters)
{
 	string stringParameter = parser->getParameter();
 	if(stringParameter.empty())
 	{
        entity->addReport(new BinaryMessage(missingParameterReporter,
        					new StringData(keyword_), new StringData("string")));
        return false;
 	}
   parameters.push_back( new StringData (stringParameter));
  return true;
}



bool OrderPrototype::parseOptionalStringParameter(Entity *entity, Parser * parser,
						ParameterList &parameters, const char * stringParameter)
{
		if(parser ->matchKeyword(stringParameter))
		{
      		parameters.push_back( new StringData (stringParameter));
      		return true;
		}
  return false;
}
