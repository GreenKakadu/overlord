/***************************************************************************
                          CombatTactics.cpp  -  description
                             -------------------
    begin                : Sun Jun 20 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

#include "LineParser.h"
#include "GameFacade.h"
#include "CombatTactics.h"
#include "CombatMoveVariety.h"
#include "CombatStanceVariety.h"
#include "CombatRankVariety.h"
#include "CombatFileVariety.h"

CombatTactics::CombatTactics()
{
    move_ = 0;
    stance_ = 0;
    rank_ = 0;
    file_ = 0;
    isUnknown_ = true;

}
// Copy constructor
CombatTactics::CombatTactics(CombatTactics & p)
{
    move_ = p.move_;
    stance_ = p.stance_;
    rank_ = p.rank_;
    file_ = p.file_;
    isUnknown_ = p.isUnknown_;
}

void     CombatTactics::defaultInitialization()
{
 move_ =   defaultCombatMove;
  stance_ = defaultCombatStance;
  rank_ =   defaultCombatRank;
  file_ =   defaultCombatFile;
  isUnknown_ = false;
}

STATUS
CombatTactics::initialize        ( Parser *parser )
{

  if (parser->matchKeyword ("COMBAT_TACTICS") )
    {
      if (parser->matchKeyword ("UNKNOWN") )
      {
        isUnknown_ = true;
        return OK;
      }
     CombatStanceVariety * stance = dynamic_cast<CombatStanceVariety *>
                        (gameFacade->combatStances.findByTag( parser->getWord(),false ));

     if(stance == 0)stance = avoidStance;
     stance_ = stance;

    CombatRankVariety * rank = dynamic_cast<CombatRankVariety *>
    			(gameFacade->combatRanks.findByTag( parser->getWord(),false ));
    if(rank == 0)
      rank = gameFacade->combatRanks["rear"];
     rank_ = rank;

    CombatFileVariety * file = dynamic_cast<CombatFileVariety *>
    			(gameFacade->combatFiles.findByTag( parser->getWord(),false ));
    if(file == 0)
      file = gameFacade->combatFiles["center"];
    file_ = file;

    CombatMoveVariety * move = dynamic_cast<CombatMoveVariety *>
    			(gameFacade->combatMoves.findByTag( parser->getWord(),false ));
    if(move == 0)
      move = gameFacade->combatMoves["flee"];// default
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

	if(isDefined() /*&& !isDefault()*/)
	{
  out<<"COMBAT_TACTICS "<< (stance_)->getTag() <<" "<< (rank_)->getTag() <<" "<< 
(file_)->getTag() <<" "<< (move_)->getTag() <<endl;
	}
        if (isUnknown())
        {
          out<<"COMBAT_TACTICS "<< "UNKNOWN"<<endl;
        }
}

void       CombatTactics::save (ostream &out, string prefix)
{
	if(isDefined() /*&& !isDefault()*/)
	{
  out<<prefix<<"COMBAT_TACTICS "<< (stance_)->getTag() <<" "<< (rank_)->getTag() <<" "<<
(file_)->getTag() <<" "<< (move_)->getTag() <<endl;
	}
        if (isUnknown())
        {
          out<<prefix<<"COMBAT_TACTICS "<< "UNKNOWN"<<endl;
        }
}



string CombatTactics::print()
{
	if(isDefined())
	{
  return (stance_)->getName() + " "+(rank_)->getName() + " "
          + (file_)->getName() + " " + (move_)->getName();
	}
else if (isUnknown())
{
  return string("unknown");
}
	else
		return string("undefined");
}



void      CombatTactics::report (ostream &out)
{
if(isDefined())
 out << "Tactics settings: "<< (stance_)->getName() << " "<< (rank_)->getName()
 		 << " " <<(file_)->getName()<< " " <<(move_)->getName()<< ".";
else if (isUnknown())
{
  out << "Tactics settings unknown. ";
}
else
 out << "Tactics settings undefined. ";
}



void CombatTactics::undefine()
{
    move_ = 0;
    stance_ = 0;
    rank_ = 0;
    file_ = 0;
    isUnknown_ = true;

}

bool CombatTactics::isDefined()
{
    if(isUnknown())
    {
        return false;
    }

	if((stance_)&& (rank_) && (file_) && (move_))
			return true;
	else
		return false;
}

bool CombatTactics::isDefault()
{
	if((stance_ == defaultCombatStance)&& (rank_ ==defaultCombatRank)
                && (file_ ==defaultCombatFile) && (move_ ==defaultCombatMove))
			return true;
	else
		return false;
}
