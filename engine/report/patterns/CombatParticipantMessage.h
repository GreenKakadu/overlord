/***************************************************************************
                          CombatParticipantMessage.h
                             -------------------
    begin                : Mar 22 2005
    copyright            : (C) 2005 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef COMBAT_PARTICIPANT_PATTERN_H
#define COMBAT_PARTICIPANT_PATTERN_H

#include "ReportMessage.h"
class TokenEntity;



/**
  *@author Alex Dribin
  */

class CombatParticipantMessage : public ReportMessage  {
public:
 CombatParticipantMessage(){}
 CombatParticipantMessage(TokenEntity * participant);
  void printReport(ReportPrinter &out) const;
  void clean(){}
protected:
	~CombatParticipantMessage(){}
	  TokenEntity * participant_;
		string report_;


};

#endif
