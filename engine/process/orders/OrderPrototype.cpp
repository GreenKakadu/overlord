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
#include "GameInfo.h"
#include "UnaryPattern.h"
#include "BinaryPattern.h"
#include "TertiaryPattern.h"
extern Reporter *	invalidOrderReporter;
extern Reporter *	invalidParameterReporter;
extern Reporter *	missingParameterReporter;
extern Reporter *	unknownParameterReporter;

extern GameInfo game;

OrderPrototype::OrderPrototype()
{
 orderType_ =DEFAULT;
}

OrderPrototype::~OrderPrototype()
{

}

STATUS
OrderPrototype::save(ostream &out)
{
  out << ' ' << keyword_ << ' ';
  return OK;
}

STATUS
OrderPrototype::loadParameters(Parser * , vector <AbstractData *>  &, Entity * )
{
  /* check parameters: wrong parameter type */
  /* check parameters: missing parameter */
  /* check parameters: Rule/Entity doesn't exist <- option */
  /* check parameters: wrong Rule/Entity type */
  return OK;

}
string
OrderPrototype::getKeyword() const
{
  return keyword_;
}

ORDER_STATUS
OrderPrototype::process(Entity * , vector < AbstractData*>  & , Order * )
{
  return SUCCESS;

}



ORDER_STATUS
OrderPrototype::completeProcessing (Entity * entity, vector <AbstractData *>  &parameters, Order * orderId, int result)
{
  return FAILURE;
}




bool OrderPrototype::isFullDayOrder()
{
  if((orderType_ == STACK_ORDER) ||(orderType_ == DAY_LONG_ORDER))
    return true;
    else
    return false;
}


bool OrderPrototype::mayBeProcessed(ProcessingMode * processingMode,Entity * entity)
{
  	if ( !processingMode-> mayExecute(orderType_))
		return false;

    if(entity->isUnaccessible())
		return false;

    return true;
}



bool OrderPrototype::entityIsUnit(Entity *entity)
{
   UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  if(unit==0)  // Wrong Entity type
				{
         entity->addReport(new BinaryPattern(invalidOrderReporter, new StringData(keyword_), new StringData("units")));
         return false;
				}
   return true;
}
bool OrderPrototype::parseGameDataParameter(Entity *entity, Parser * parser, BasicCollection & collection, const string & parameterTypeName, vector <AbstractData *>  &parameters)
{
   return parseGameDataParameter(entity, parser->getWord(),collection,parameterTypeName,parameters);
}



bool OrderPrototype::parseGameDataParameter(Entity *entity,const string & tag, BasicCollection & collection, const string & parameterTypeName, vector <AbstractData *>  &parameters)
{
   if (tag.size() == 0)  // Missing parameter
        {
        entity->addReport(new BinaryPattern(missingParameterReporter, new StringData(keyword_), new StringData(parameterTypeName)));
         return false;
        }

   if (!collection.checkDataType(tag)) // this doesn't look like a tag  but it still may be new tag
       {
           if(!game.isNewEntityName(tag))
 				  {
            entity->addReport(new TertiaryPattern(invalidParameterReporter, new StringData(keyword_), new StringData(tag), new StringData(parameterTypeName)));
            return false;
				  }

       }   
  return (checkParameterTag(entity, tag,  collection, parameters));
}
bool OrderPrototype::checkParameterTag(Entity *entity,const string & tag, BasicCollection & collection, vector <AbstractData *>  &parameters)
{
//   cout << "[][]== checking tag "<<tag << endl;
   GameData* item = collection.findByTag(tag);
  if( item == 0) // Entity doesn't exist but it may be new entity placeholder
		{
//      UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
//      if(unit!=0)
//        {
          if(game.isNewEntityName(tag/*,unit->getFaction()*/))
            {
              NewEntityPlaceholder * placeholder = collection.findPlaceholder(tag);
 //              cout <<"placeholder=  "<< (int) placeholder <<endl;
              if(placeholder != 0)  // this is  placeholder.
              {
                GameData* entity = placeholder->getRealEntity();
 //              cout <<"entity=  "<< (int) entity <<endl;
                if(entity) // We can get real entity id from placeholder
   		            parameters.push_back(entity);
                 else   // placeholder is still empty
   		              parameters.push_back(placeholder);
                return true;
              }
//             }
				}
    // this is not placeholder. Entity doesn't exist but we don't want to let player to know that

    // Give warning if this is unknown rule
    
    Rule * rule = dynamic_cast<Rule*>(item);
    if(rule)
          {
            UnitEntity * unit = dynamic_cast<UnitEntity*>(entity);
            if(unit)
              {
                if(!unit->getFaction()->hasKnowledge(rule))
                {
                  entity->addReport(new UnaryPattern(unknownParameterReporter,
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



bool OrderPrototype::parseIntegerParameter(Parser * parser, vector <AbstractData *>  &parameters)
{
  if(parser -> matchInteger())
		{
      parameters.push_back( new IntegerData (parser->getInteger()));
      return true;
		}
  return false;
}

bool OrderPrototype::parseOptionalGameDataParameter(Entity *entity, Parser * parser, BasicCollection & collection, const string & parameterTypeName, vector <AbstractData *>  &parameters)
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

   parser->getWord();

  return checkParameterTag(entity, tag,  collection, parameters);
//   GameData* item = collection.findByTag(tag);
//  if( item == 0) // Entity doesn't exist but it may be new entity placeholder
//				{
//          NewEntityPlaceholder * placeholder = collection.findPlaceholder(tag);
//          if(placeholder == 0)  // this is not placeholder. Entity doesn't exist but we don't want to let player to know that
//            {
//            StringData * dummy = new StringData(tag);
//   		      parameters.push_back(dummy);
//           }
//           else
//           {
//            GameData* entity = placeholder->getRealEntity();
//             if(entity) // We can get real entity id from placeholder
//   		          parameters.push_back(entity);
//             else   // placeholder is still empty
//   		          parameters.push_back(placeholder);
//           }
//				}
//   else
//				{
//   		     parameters.push_back(item);
//				}
//   return true;
}



int OrderPrototype::getIntegerParameter(vector <AbstractData *>  &parameters, int parIndex)
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

