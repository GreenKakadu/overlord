/***************************************************************************
                          OrderPrototype.cpp
                    .
                             -------------------
    begin                : Tue Nov  5 11:46:00 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "OrderPrototype.h"
#include "EntitiesCollection.h"
extern EntitiesCollection <UnitEntity>   units;


OrderPrototype::OrderPrototype()
{

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
OrderPrototype::load(Parser * parser, vector <ParameterHandler *>  &parameters)
{
  return OK;

}
string
OrderPrototype::getKeyword() const
{
  return keyword_;
}

//STATUS
//OrderPrototype::load()
//{

  //}

STATUS
OrderPrototype::check(Parser * parser, vector <ParameterHandler *>  &parameters, ostream &out)
{
  return OK;
}

ORDER_STATUS
OrderPrototype::process(GameData * entity, vector <ParameterHandler *>  &parameters,
					 ostream &out, list <OrderModifier *> reportModifiers_)
{
  return SUCCESS;

}




GiveOrderPrototype::GiveOrderPrototype()
{
  keyword_ = "give";
  description_ = "GIVE unit-id item-tag [number [kept]] \n
Immediate.  Attempts to hand the required amount of items to the designated unit.  If no number is specified, attempts to give as much as possible.  The order executes if the designated unit is there and the issuing unit has the items.  If a number of items to be kept is specified, the unit must have at least the number of items handed and the number kept in possession.  In that case, zero may be specified as the number, and the unit will give all but the specified amount, as long as one item can be given.";
}

STATUS
GiveOrderPrototype::check(Parser * parser, vector <ParameterHandler *>  &parameters, ostream &out)
{
  string parameter;
  parameter = parser -> getWord(); 
  if(parameter.size() == 0)
    {
      out << "GIVE: missing parameters - no recipient found" << endl;
      return ERROR;
    }
  if(! units.checkEntityType(parameter))
    {
      out << "GIVE: wrong tag for UNIT " << parameter << endl;
      return ERROR;
    }
  if(parser -> getWord().size() == 0)
    {
      out << "GIVE: missing parameters - ITEM to give was not found" << endl;
      return ERROR;
    }

  return OK;
}

STATUS
GiveOrderPrototype::load(Parser * parser, vector <ParameterHandler *>  &parameters )
{
  parameters.push_back( new StringParameterHandler (parser));
  parameters.push_back( new StringParameterHandler (parser));
  parameters.push_back( new IntegerParameterHandler (parser));
  parameters.push_back( new IntegerParameterHandler (parser));
  return OK;


}

//============================================================================
//
//      ORDER_STATUS process ()
//
//  Unit gives something to another unit. Both must be at the same location
// and the other must treat the first as friendly.
//
//============================================================================

ORDER_STATUS
GiveOrderPrototype::process (GameData * entity, vector <ParameterHandler *>  &parameters,
			     ostream &out, list <OrderModifier *> reportModifiers_)
{
# if 0 // <---------------------------  Demands reporting routines


// give all list of report modifiers us unefficient!
  

UnitEntity  *recipient;
int unitItemPossesion;
ItemEntity  *item;
int     given, kept;
// pointers to elements, containing item 
/*
 * Validate arguments
 */
string par0,par1;
int par2, par3;

  parameters[0].getParameter(par0);
  parameters[1].getParameter(par1);

  if ( !units.isValidTag(par0)  || ! items.isValidTag(par1) )
    {
      // Report:          unit_personal_event(unit, today_number, "Invalid GIVE order");
      out << entity.getName() << '[' << entity.getTag() << ']'<< current_day
	  << "Invalid GIVE order" << endl;

                return INVALID;
    }

        recipient = units[par0];
        if (!may_interact_with(unit, recipient))
                return FAILURE;
/*
 * Validate item amounts
 */
        item = items[par1];
	
        // get number of items in iventory unitItemPossesion
        if (!unitItemPossesion)
                return FAILURE;

         parameters[2].getParameter(given);       
	 parameters[3].getParameter(kept);

        if (given == 0)
                given = unitItemPossesion - kept;
        if (given <= 0 || given+kept > unitItemPossesion)
                return FAILURE;
        current->considered = 1;

        if (/* recipient is less than friendly */)
	  {
                sprintf(work, "%s [%s] does not accept gifts", recipient->name, recipient->id.text);
                unit_personal_event(unit, today_number, work);
                sprintf(work, "Refused gifts from %s [%s]", unit->name, unit->id.text);
                unit_personal_event(recipient, today_number, work);
                return SUCCESS;
        }
/*
 * Carrying
 */
	// subtract items from posessions   -= given

        if (item == items[mana])
                given = 0;
	
        if (!unit->setting_silent)
	  {
	    sprintf(work, "Giving %d %s [%s] to %s [%s]", given, given > 1 ? type->plural :
		    type->name, type->tag.text, recipient->name, recipient->id.text);
	    unit_personal_event(unit, today_number, work);
        }
	// add  items to posessions   += given

        if (given && !recipient->setting_silent)
	  {
	    sprintf(work, "Received %d %s [%s] from %s [%s]", given, given > 1 ? type->plural :
		    type->name, type->tag.text, unit->name, unit->id.text);
	    unit_personal_event(recipient, today_number, work);
	  }


	if (items->amount < items->equipped) {
                items->equipped = items->amount;
                compute_unit_stats(unit);
        }
#endif // <------------------------------ Commented untill reporting routines will be ready
        return SUCCESS;
	
}


