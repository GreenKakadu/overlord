/***************************************************************************
                          CombatOrderPrototype.cpp
                    .
                             -------------------
    begin                : Tue Nov  5 11:46:00 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "CombatOrderPrototype.h"
#include "Entity.h"





CombatOrderPrototype::CombatOrderPrototype()
{
 initiative_ = 0;
 isSequentive_ = false;
}



/*
 * All orders know how to save themself into entity's data and orders template
 */

STATUS
CombatOrderPrototype::save(ostream &out)
{
  out << ' ' << keyword_ << ' ';
  return OK;
}






/*
 * Sometimes order may not be processed for some reasons.
 * Entity may be unable to follow orders (dead, demoralized, paralized)
 * Entity may be busy (moving)
 * Determines if order may be processed
 */
bool CombatOrderPrototype::mayBeProcessed(ProcessingMode * processingMode,
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



