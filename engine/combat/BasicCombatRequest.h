/***************************************************************************
                            BasicCombatRequest.h
                          ------------------
    begin                : Mon May 24 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef BASIC_COMBAT_REQUEST_H
#define BASIC_COMBAT_REQUEST_H
#include "CombatEngine.h"
class TokenEntity;
class OrderLine;

class BasicCombatRequest {
public:
BasicCombatRequest(TokenEntity * attacker, TokenEntity * defender,
	OrderLine * orderId,
	void (*funPtr)(TokenEntity * attacker, TokenEntity * defender,
	const  BATTLE_RESULT result));

  virtual ~BasicCombatRequest(){}
  virtual void answerRequest(const BATTLE_RESULT result);
  virtual  string print();
//  inline UnitEntity * getRequestingUnit() const {return unit_;}
  virtual  bool isEqualTo (BasicCombatRequest * request);
//  virtual inline AbstractData * getType() const;
  virtual inline void free(){delete this;}
  virtual   bool isValid();
  virtual inline TokenEntity *  getAttacker() const {return attacker_;}
  virtual inline TokenEntity *  getDefender() const {return defender_;}
          inline BasicCombatEngine * getCombatEngine()const {return combatType_;}
					inline LocationEntity * getLocation() const {return location_;}
protected:
  void (*postCombatProcessing_)(TokenEntity * attacker, TokenEntity * defender, const BATTLE_RESULT  result);
  TokenEntity * attacker_;
  TokenEntity * defender_;
	LocationEntity * location_;
  OrderLine * orderId_;
  static BasicCombatEngine * combatType_;
private:

};
typedef  vector  <BasicCombatRequest *>::iterator BasicCombatRequestIterator;
#endif
