/** ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff
*************************************************************************
 TacticOrder.cpp ------------------- begin                : Thu Nov 19 2003
 copyright            : (C) 2003 by Alex Dribin
 email                : Alex.Dribin@gmail.com
fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff */
#include "TacticOrder.h"
#include "StringData.h"
#include "TokenEntity.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "EntitiesCollection.h"
#include "CombatMoveVariety.h"
#include "CombatStanceVariety.h"
#include "CombatRankVariety.h"
#include "CombatFileVariety.h"
extern EntitiesCollection < UnitEntity > units;
extern ReportPattern * invalidParameterReporter;
extern ReportPattern * missingParameterReporter;
extern ReportPattern * unknownTactics;
extern ReportPattern * tacticsSettings;

TacticOrder * instantiateTacticOrder = new TacticOrder();



TacticOrder::TacticOrder()
{
  keyword_ = "Tactic";
  registerOrder_();
  description = string( "TACTIC stance rank file move \n" )
       + "Immediate, one-shot.  This order executes immediately and sets your tactical\n"
       + "battle settings.\n"
       + "\n"
       + "Combat Stance is one of either: AVOID (do not enter battle unless attacked), DEFEND\n"
       +  "( do not enter battle unless you are on the defending side ) or FIGHT.\n"
       +  "\n"
       +  "Combat Rank is one of either: FRONT, MIDDLE or REAR.\n" + "\n"
              + "Combat File is one of either: LEFT, CENTER or RIGHT.\n"
              + "\n"
              + "Combat Move is one of either: FLEE, RETREAT, STAND, ADVANCE, SWEEP, FLANK, CHARGE,\n"
              +  "FIRE, or SUPPORT( see the description of the battle phase )\n";

         orderType_ = IMMEDIATE_ORDER;
         mayInterrupt_ = true;
}



STATUS TacticOrder::loadParameters( Parser * parser,
     vector < AbstractData * > & parameters, Entity * entity )
{
  if ( !entityIsUnit( entity ) )
         return IO_ERROR;
  bool parsingResult = false;
  parsingResult |= parseOptionalStringParameter( entity, parser, parameters, "avoid" );
  parsingResult |= parseOptionalStringParameter( entity, parser, parameters, "defend" );
  parsingResult |= parseOptionalStringParameter( entity, parser, parameters, "attack" );
  parsingResult |= parseOptionalStringParameter( entity, parser, parameters, "fight" );
  parsingResult |= parseOptionalStringParameter( entity, parser, parameters, "front" );
  parsingResult |= parseOptionalStringParameter( entity, parser, parameters, "middle" );
  parsingResult |= parseOptionalStringParameter( entity, parser, parameters, "rear" );
  parsingResult |= parseOptionalStringParameter( entity, parser, parameters, "left" );
  parsingResult |= parseOptionalStringParameter( entity, parser, parameters, "center" );
  parsingResult |= parseOptionalStringParameter( entity, parser, parameters, "right" );
  parsingResult |= parseOptionalStringParameter( entity, parser, parameters, "flee" );
  parsingResult |= parseOptionalStringParameter( entity, parser, parameters, "retreat" );
  parsingResult |= parseOptionalStringParameter( entity, parser, parameters, "stand" );
  parsingResult |= parseOptionalStringParameter( entity, parser, parameters, "advance" );
  parsingResult |= parseOptionalStringParameter( entity, parser, parameters, "sweep" );
  parsingResult |= parseOptionalStringParameter( entity, parser, parameters, "flank" );
  parsingResult |= parseOptionalStringParameter( entity, parser, parameters, "charge" );
  parsingResult |= parseOptionalStringParameter( entity, parser, parameters, "fire" );
  parsingResult |= parseOptionalStringParameter( entity, parser, parameters, "support" );
  if ( parsingResult ) return OK;
  else
     {
       entity->addReport(
        new TertiaryMessage( invalidParameterReporter, new StringData( keyword_),
        new StringData( parser->getWord() ),
				new StringData( "one of tactic keywords ")));
		  return IO_ERROR;
			}
}



ORDER_STATUS TacticOrder::process
            ( Entity * entity, vector < AbstractData * > & parameters )
{
              TokenEntity * token = dynamic_cast < TokenEntity * > ( entity );
              assert( token );
              CombatStanceVariety * stance = 0;
              CombatRankVariety * rank = 0;
              CombatFileVariety * file = 0;
              CombatMoveVariety * move = 0;

              for ( unsigned int i = 0; i < parameters.size(); ++i )
              {
                string par = ( parameters[i] )->print();
                stance =
                     dynamic_cast < CombatStanceVariety *
                     > ( combatStances.findByTag( par, false ) );
                if ( stance )
                {
                  token->setCombatStance( stance );
                  continue;
                }

                rank =
                     dynamic_cast < CombatRankVariety *
                     > ( combatRanks.findByTag( par, false ) );
                if ( rank )
                {
                  token->setCombatRank( rank );
                  continue;
                }

                file =
                     dynamic_cast < CombatFileVariety *
                     > ( combatFiles.findByTag( par, false ) );
                if ( file )
                {
                  token->setCombatFile( file );
                  continue;
                }

                move =
                     dynamic_cast < CombatMoveVariety *
                     > ( combatMoves.findByTag( par, false ) );
                if ( move )
                {
                  token->setCombatMove( move );
                  continue;
                }
                entity->addReport(
                     new UnaryMessage( unknownTactics, new StringData( par ) ),
                     entity->getCurrentOrder(), 0 );
              }
               token->addReport(
                     new UnaryMessage( tacticsSettings, new StringData(token->reportCombatTactics())));
              return SUCCESS;
}
