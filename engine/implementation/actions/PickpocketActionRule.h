/***************************************************************************
                          PickpocketActionRule.h
                             -------------------
    begin                : Tue Jan 20 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef PICKPOCKET_ACTION_RULE_H
#define PICKPOCKET_ACTION_RULE_H

#include "ActionRule.h"

/**
  *@author Alex Dribin
  */

class PickpocketActionRule : public ActionRule  {
public: 
      PickpocketActionRule ( const string & keyword, GameData * parent) : ActionRule(keyword, parent){}
      PickpocketActionRule ( const PickpocketActionRule * prototype );
      ~PickpocketActionRule (){}
//      virtual STATUS     initialize      ( Parser *parser);
      GameData * createInstanceOfSelf();
//      void printDescription(ReportPrinter & out);
      ACTION_RESULT carryOut(Entity * entity);
};

#endif
