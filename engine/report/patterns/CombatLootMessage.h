/***************************************************************************
                          CombatLootMessage.h
                             -------------------
    begin                : Apr 14 2005
    copyright            : (C) 2005 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef COMBAT_PARTICIPANT_PATTERN_H
#define COMBAT_PARTICIPANT_PATTERN_H

#include "ReportMessage.h"
#include "ItemElement.h"

class TokenEntity;



/**
  *@author Alex Dribin
  */

class CombatLootMessage : public ReportMessage  {
public:
 CombatLootMessage(){}
 CombatLootMessage(vector <ItemElement> loot);
  void printReport(ReportPrinter &out) const;
  void clean(){}
protected:
	vector <ItemElement> loot_;
	~CombatLootMessage(){}


};

#endif
