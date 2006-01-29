/***************************************************************************
                          CombatOrderLine.h
                             Combat Order  deals with initiative
                             -------------------
    begin                : Fri Oct  29 10:00:00 IST 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef COMBAT_ORDER_LINE_H
#define COMBAT_ORDER_LINE_H

#include "OrderLine.h"


using namespace std;
class TokenEntity;


class CombatOrderLine : public OrderLine
{
    public:
        CombatOrderLine(const string & order, TokenEntity * entity);
//        CombatOrderLine(CombatOrderLine & combatOrderLine);
        ~CombatOrderLine(){}
	bool parse(Parser * parser, Entity * entity );
	bool parse(const string & orderText, Entity * entity );
	void initialize(TokenEntity * entity);
	inline void setInitiative(int initiative){initiative_ = initiative;}
	inline  int getInitiative() const {return initiative_;}
  ORDER_STATUS process( ProcessingMode * processingMode, TokenEntity * entity,
	 int currentRound);
	void save( ostream & out );
  int reCalculateInitiative(TokenEntity * entity, int sideBonus);
	ORDER_TYPE	getOrderType() const {return orderPrototype_->getOrderType();}
	inline void setPermanent(bool value){isPermanent_ = value;}
	bool isSequentive();
  bool evaluate(TokenEntity * entity);
	inline bool isPlanned() const {return isPlanned_;}
	inline void setPlanned(bool value)  { isPlanned_ = value;}

    protected:
	int initiative_;
	int executedOnRound_;
	bool isPlanned_;


     private:

};
typedef vector <CombatOrderLine *>::iterator CombatOrderIterator;
#endif







