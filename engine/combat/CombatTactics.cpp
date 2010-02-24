/***************************************************************************
                          CombatTactics.cpp  -  description
                             -------------------
    begin                : Sun Jun 20 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

#include "LineParser.h"
#include "CombatTactics.h"
#include "CombatMoveVariety.h"
#include "CombatStanceVariety.h"
#include "CombatRankVariety.h"
#include "CombatFileVariety.h"

CombatTactics::CombatTactics()
{
}

void     CombatTactics::defaultInitialization()
{
	move_ =   defaultCombatMove;
  stance_ = defaultCombatStance;
  rank_ =   defaultCombatRank;
  file_ =   defaultCombatFile;
}

STATUS
CombatTactics::initialize        ( Parser *parser )
{

  if (parser->matchKeyword ("COMBAT_TACTICS") )
    {
     CombatStanceVariety * stance = dynamic_cast<CombatStanceVariety *>
                        (combatStances.findByTag( parser->getWord(),false ));

     if(stance == 0)stance = avoidStance;
     stance_ = stance;

    CombatRankVariety * rank = dynamic_cast<CombatRankVariety *>
    			(combatRanks.findByTag( parser->getWord(),false ));
    if(rank == 0)
      rank = combatRanks["rear"];
     rank_ = rank;

    CombatFileVariety * file = dynamic_cast<CombatFileVariety *>
    			(combatFiles.findByTag( parser->getWord(),false ));
    if(file == 0)
      file = combatFiles["center"];
    file_ = file;

    CombatMoveVariety * move = dynamic_cast<CombatMoveVariety *>
    			(combatMoves.findByTag( parser->getWord(),false ));
    if(move == 0)
      move = combatMoves["flee"];// default
    move_ = move;
      
    
   }
   else
   {
     if(move_ ==0)
        move_ =   defaultCombatMove;
     if(stance_ ==0)
        stance_ = defaultCombatStance;
     if(rank_ ==0)
        rank_ =   defaultCombatRank;
     if(file_ ==0)
        file_ =   defaultCombatFile;
   }
      return OK;
}



void       CombatTactics::save (ostream &out)
{
  out<<"COMBAT_TACTICS "<< (stance_)->getTag() <<" "<< (rank_)->getTag() <<" "<< 
(file_)->getTag() <<" "<< (move_)->getTag() <<endl;}



string CombatTactics::print()
{
  return (stance_)->getName() + " "+(rank_)->getName() + " "
          + (file_)->getName() + " " + (move_)->getName();
}



void      CombatTactics::report (ostream &out)
{
if((stance_)&& (rank_) && (file_) && (move_))
 out << "Tactics settings: "<< (stance_)->getName() << " "<< (rank_)->getName()
 		 << " " <<(file_)->getName()<< " " <<(move_)->getName()<< ".";
else
 out << "Tactics settings undefined. ";
}

