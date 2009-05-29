/***************************************************************************
                          BattleEntity.h
                             -------------------
    begin                : May 22 2006
    copyright            : (C) 2006 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef BATTLE_ENTITY_H
#define BATTLE_ENTITY_H

#include "TokenEntity.h"


class RaceRule;
/**subclass of TokenEntities that are summoned duiring battle and dissapear when battle is over
  *@author Alex Dribin
  */
class BattleEntity : public TokenEntity  {
public:
      BattleEntity (const string & keyword, GameData * parent );
      BattleEntity ( const BattleEntity * prototype );
	~BattleEntity(){}
	inline void setBattleInstantiation(BattleInstance * instance)
	 																									{battleInstance_ = instance;}
  STATUS  initialize      ( Parser *parser );
      BattleEntity * makeCopy();
			void recalculateStats();
	inline RaceRule * getRace() const {return race_;}
	inline void setRace(RaceRule * race)  {race_ = race;}
	inline int getFigures() const {return figures_;}
	inline void setFigures(int figures)  { figures_ = figures;}
   protected:
		RaceRule *  race_;
		int figures_;
};
extern BattleEntity         sampleBattleEntity;
typedef vector <BattleEntity *>::iterator  BattleEntityIterator;
#endif
