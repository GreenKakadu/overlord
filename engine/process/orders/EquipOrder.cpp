/***************************************************************************
                          EquipOrder.cpp
                             -------------------
    begin                : Mon Mar 3 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "EquipOrder.h"
#include "SkillLevelElement.h"
#include "StringData.h"
#include "IntegerData.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "UnitEntity.h"
#include "ItemRule.h"
#include "ItemElement.h"
const unsigned EquipOrder::SKILL_REQUIRED_REPORT_FLAG = 0x01;
extern ReportPattern *	unequipReporter;
extern ReportPattern *	equipReporter;
extern ReportPattern *	unequipableReporter;
extern ReportPattern *	equipSkillReporter;

EquipOrder * instantiateEquipOrder = new EquipOrder();
//EquipOrder  instantiateEquipOrder;

EquipOrder::EquipOrder(){
   keyword_ = "equip";
  registerOrder_();
  description = string("EQUIP item-tag [number] \n") +
  "Immediate.  Executes if the unit has the required skill to equip the item,\n" +
  "and enough items.  The items are equipped, or unequipped if some items were\n" +
  "already equipped, and a lower number has been specified.  If other items in\n" +
  "the same category are already equipped, less than the requested number of\n" +
  "items might be equipped.\n" +
  "\n" +
  "If unspecified, as many items as possible will be equipped.  If 0 is\n" +
  "specified, all items will be unequipped. If more items are specified than\n" +
  "allowed, the maximum amount will be equipped.\n";

  orderType_   = IMMEDIATE_ORDER;
}



STATUS EquipOrder::loadParameters(Parser * parser,
                            ParameterList &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

    if(!parseGameDataParameter(entity,  parser, gameFacade->items, "item tag", parameters))
            return IO_ERROR;

//   string itemTag = parser->getWord();
//  if (itemTag.size() == 0)  // Missing parameter
//        {
//					cout  << "=<>= ACCEPT: missing Parameter (item tag expected) for "
//								<< entity->print() << endl;
//         entity->addReport(new BinaryMessage(missingParameterReporter, new StringData(keyword_), new StringData("unit id")));
//         return IO_ERROR;
//        }
//
//
//  ItemRule * item = items[itemTag];
//  if( item == 0) // item doesn't exist but we don't want to let player to know that
//				{
//          StringData * dummyItem = new StringData(itemTag);
//   		     parameters.push_back(dummyItem);
//				}
//   else
//				{
//   		     parameters.push_back(item);
//				}

    parseIntegerParameter(parser, parameters);

  return OK;
}



ORDER_STATUS EquipOrder::process (Entity * entity, ParameterList &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);

  ItemRule * item = dynamic_cast< ItemRule*>(parameters[0]);

  if ( item == 0) // item doesn't exist but we don't want to let player to know that
     {
 		  return FAILURE;
      }

  if(item->getEquipSlot() == 0)
    {
         unit->addReport(new UnaryMessage(unequipableReporter,item));
 		  return INVALID;
    }
  BasicCondition * equipCondition = item->demandsEquipCondition();
  if(equipCondition )
  {
    if(!equipCondition->isSatisfied(unit))
    {
     // Unit has no skill to equip item
     if(!unit->getCurrentOrder()->getReportingFlag( SKILL_REQUIRED_REPORT_FLAG))
      {
//        unit->addReport(new BinaryMessage(equipSkillReporter,new SkillLevelElement(skillRequired),item));
        unit->getCurrentOrder()->setReportingFlag( SKILL_REQUIRED_REPORT_FLAG);
      }
        return FAILURE;
     }
    unit->getCurrentOrder()->clearReportingFlag(SKILL_REQUIRED_REPORT_FLAG);
    }
   int number;
   int result;
   OrderLine * orderId = entity->getCurrentOrder();
   if(parameters.size() > 1)
      {
        IntegerData * numToEquip  = dynamic_cast<IntegerData *>(parameters[1]);
        assert(numToEquip);

        number = numToEquip->getValue();
        if (number == 0)  // unequip all
              {
                 unit->equipItem (item, number);
		             unit->addReport(new BinaryMessage(unequipReporter, unit, item),orderId,0 );
                 return SUCCESS;
                }
      result = unit->equipItem (item, number);
        if( result == 0)
		            return FAILURE;
        if( result < 0)    // unequiped
        {
		      unit->addReport(new BinaryMessage(unequipReporter, unit, item),orderId,0  );
          return SUCCESS;
          }
        if (result < number) // only some part of items equiped
              {
                //numToEquip->setValue(number - result);
//QQQ
		            unit->addReport( new BinaryMessage(equipReporter, unit, new ItemElement(item,result)),orderId,0 );
// 		            return FAILURE;
                  return SUCCESS;
              }
//QQQ
		     unit->addReport( new BinaryMessage(equipReporter, unit, new ItemElement(item,result)),orderId,0  );
         return SUCCESS;
      }
    else  // equip all available
      {
// 	if(unit->isTraced())
// 	{
// 	//cout << unit->print()<< " is equipping all "<< item->print()<<endl;
// 	}
        number = unit->hasItem(item);
        result = unit->equipItem (item, number);
        if (result == 0)
              {
 		            return FAILURE;
              }
//QQQ
		     unit->addReport(new BinaryMessage(equipReporter, unit, new ItemElement(item,result)),orderId,0  );
         return SUCCESS;
       }

// Equipping items may cause change in combat action
}
