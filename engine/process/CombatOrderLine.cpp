/***************************************************************************
                          CombatOrderLine.h
                      Combat Order  deals with initiative
                             -------------------
    begin                : Fri Oct  29 10:00:00 IST 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "CombatOrderLine.h"
#include "TokenEntity.h"
#include "BattleInstance.h"
#include "OrderPrototypesCollection.h"
//#include "LineParser.h"
extern bool testMode;
extern ofstream combatReportFile;


CombatOrderLine::CombatOrderLine(const string & order, TokenEntity * entity) :
				 OrderLine(order,entity)
{
	initiative_ = 0;
	executedOnRound_ = 0;
	isPlanned_ =true;
//	cout << "CREATING ORDER "<< order << " for " << entity<< endl;
}

//CombatOrderLine::CombatOrderLine(CombatOrderLine & combatOrderLine):OrderLine(combatOrderLine)
//{
//	executedOnRound_ = 0;
//	cerr << "Invoked copy constructor for "<< orderPrototype_->getKeyword()<<endl;
//}



void CombatOrderLine::initialize(TokenEntity * entity)
{
	executedOnRound_ = 0;
}



int CombatOrderLine::reCalculateInitiative(int initiative, int sideBonus)
{
	if(orderPrototype_ == 0)
	{
			cout << "Can't determine initiative: No prototype for ";
			printOrderLine(cout);
			return 0;
	}
	initiative_ = initiative + orderPrototype_->getInitiative() + sideBonus;
	return initiative_;
}



void CombatOrderLine::save( ostream & out )
{
  out << "COMBAT_ORDER ";
  printOrderLine( out );
}



bool CombatOrderLine::parse( Parser * parser, Entity * entity )
{
	string tempKeyword = parser->matchWord();// this may be keyword or parameter
//  orderPrototype_ = combatOrderPrototypesCollection->find( tempKeyword );
//  use of specific combatOrderPrototypesCollection for combat orders requires
// CombatOrderPrototype class with definition of combatOrderPrototypesCollection
  orderPrototype_ = orderPrototypesCollection->find( tempKeyword );
  if ( orderPrototype_ == 0 )
  {
		if(elseStatement_ || endifStatement_)
         return true;
		else
		{
			// Try this keyword  as a skill
			SkillRule * skill = skills[tempKeyword];
   		if ( skill != 0)
    	{
				orderPrototype_ = orderPrototypesCollection->find("Apply");
				if(orderPrototype_)
				{
					cerr << "Combat order Apply (skill) is not implemented"<<endl;
					return false;
				}
    	}
			else			// Try this keyword  as an item
			{
				ItemRule * item = items[tempKeyword];
   			if ( item != 0)
				{
					orderPrototype_ = orderPrototypesCollection->find("CombatUse");
					if(orderPrototype_)
					{
						cerr << "Combat order Use (item) is not implemented"<<endl;
						return false;
					}
				}
				else // Not a skill and not an item
				{
					return false;
				}
			}

		}

  }
	else // keyword is valid order. now we may scip it
	{
		parser->getWord();
	}

	if ( orderPrototype_->
       loadParameters( parser, parameters_, entity ) == OK )
         return true;
  else
  {
    orderPrototype_ = 0;
    return false;
  }

}






ORDER_STATUS CombatOrderLine::process( ProcessingMode * processingMode,
     TokenEntity * entity, int currentRound)
     {
       ORDER_STATUS result;

//        if (entity->isTraced())
//        {
//          cout << "==== Trying to process ";
//          save( cout );
//        }


//     combatReportFile << "==== Trying to process "; save( combatReportFile );
				if(!isPlanned()) // This order was not planned for this day. Skip
						return FAILURE;

       if ( orderPrototype_ == 0 )
			 	{
					if(elseStatement_ || endifStatement_)
         		return FAILURE;
					else
         		return INVALID;
				}

 	    BattleInstance * instance = entity->getBattleInstantiation();

			if(orderPrototype_->getOrderType() == COMBAT_MOVEMENT_ORDER
											&& isSequentive())
				{
					if (instance->getMovementInitiative()< processingMode->getInitiative())
						return FAILURE;
				}

				else if (initiative_ < processingMode->getInitiative())
			 {
//        	combatReportFile<< "- - - To low Initiative. Has "<<initiative_
//						<<" needs " <<processingMode->getInitiative()<<" Order: ";
//					printOrderLine(combatReportFile);
				if(isSequentive())
				{
					instance->setWaiting(true);
//				 	return WAITING; // not enough initiative to process
				 }
//				else
					return FAILURE;
			 }
//        	combatReportFile<< "+ + + Initiative OK. Has "<<initiative_
//						<<" needs " <<processingMode->getInitiative()<<" Order: ";
//					printOrderLine(combatReportFile);

       if ( executedOnRound_ == currentRound )
       {
/*         if ( entity->isTraced() )
           cout << "==== Was already executed on this round" << endl;
 */        return FAILURE;
       }

       if ( !orderPrototype_->mayBeProcessed( processingMode, entity ) )
       {
//          if ( entity->isTraced() )
//            cout << "==== Order can't be processed duiring this mode" << endl;

         return FAILURE;
       }

			 if ( !instance->mayActOnRound(orderPrototype_,currentRound) )
       {
         return FAILURE;
       }

       if ( ( dayRestricted_ == 0 ) || ( dayRestricted_ == currentRound ) )
       {
         instance->setCurrentOrder( this );
         result = orderPrototype_->process( entity, parameters_ );
         instance->setCurrentOrder( 0 );
       }
       else
         return FAILURE;

			if (instance->isProcessingRepeatingOrder()) // To be Repeated (Multi-hit)
			{
        return result; // do not set executedOnRound_ flag
			}


			 if ( ( result == SUCCESS ) || ( result == IN_PROGRESS ) )
         executedOnRound_ = currentRound;

       if ( ( result == FAILURE ) && isFullDayOrder() ) // This order was already checked
              executedOnRound_ = currentRound;

       return result;
}



bool CombatOrderLine::isSequentive()
{
	if(orderPrototype_)
		return orderPrototype_->isSequentive();
	else
		return false;
}



bool CombatOrderLine::evaluate(TokenEntity * entity)
{
	return orderPrototype_->evaluate(entity, parameters_);
}




bool CombatOrderLine::parse(const string & orderText, Entity * entity )
{
  Parser * parser = new Parser(orderText);
	string tempKeyword = parser->matchWord();// this may be keyword or parameter

	orderPrototype_ = orderPrototypesCollection->find( tempKeyword );
  if ( orderPrototype_ == 0 )
  {
		if(elseStatement_ || endifStatement_)
         return true;
		else
		{
			// Try this keyword  as a skill
			SkillRule * skill = skills[tempKeyword];
   		if ( skill != 0)
    	{
				orderPrototype_ = orderPrototypesCollection->find("Apply");
				if(orderPrototype_ == 0)
				{
					cerr << "Combat order Apply (skill) is not implemented"<<endl;
					return false;
				}
    	}
			else			// Try this keyword  as an item
			{
				ItemRule * item = items[tempKeyword];
   			if ( item != 0)
				{
					orderPrototype_ = orderPrototypesCollection->find("CombatUse");
					if(orderPrototype_ == 0)
					{
						cerr << "Combat order Use (item) is not implemented"<<endl;
						return false;
					}
				}
				else // Not a skill and not an item
				{
					return false;
				}
			}

		}

  }
	else // keyword is valid order. now we may scip it
	{
		parser->getWord();
	}

	if ( orderPrototype_->
       loadParameters( parser, parameters_, entity ) == OK )
         return true;
  else
  {
    orderPrototype_ = 0;
    return false;
  }

}