/***************************************************************************
                          CombatOrderPrototype.h
     Generic Combat order object               .
                             -------------------
    begin                : Wen Aug  7 13:28:00 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef COMBAT_ORDER_PROTOTYPE_H
#define COMBAT_ORDER_PROTOTYPE_H
#include <vector>
#include <string>
#include <iostream>

#include "OrderPrototype.h"


class CombatOrderPrototype : public OrderPrototype
{
    public:
        CombatOrderPrototype();
        virtual ~CombatOrderPrototype(){}


    bool mayBeProcessed(ProcessingMode * processingMode, Entity * entity);
//		inline int getInitiative()const {return initiative_;}
//	  inline bool isSequentive(){return isSequentive_;}
		STATUS save(ostream &out);
		virtual bool evaluate(){return true;}
    protected:
			int initiative_;
	    bool isSequentive_;
    private:
};
#endif



