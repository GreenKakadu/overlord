/***************************************************************************
                          SettingOrder.h
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef SETTING_ORDER_H
#define SETTING_ORDER_H

#include "OrderPrototype.h"

/**
  *@author Alex Dribin
  */
class ReportPattern;
class AbstractData;

class SettingOrder : public OrderPrototype  {
public:
	SettingOrder();
	~SettingOrder(){}
  STATUS loadParameters(Parser * parser, ParameterList &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, ParameterList &parameters);
  bool parseSetting(Parser * parser, ParameterList &parameters,
							const char *setting,   const char *defaultSetting);

    protected:
};

#endif
