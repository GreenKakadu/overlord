/***************************************************************************
                          ResourceCompetitiveRequest.h 
                             -------------------
    begin                : Fri Apr 18 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef RESOURCE_COMPETITIVE_REQUEST_H
#define RESOURCE_COMPETITIVE_REQUEST_H

/** Request for some action requiring limited resourse of items
  *@author Alex Dribin
  */
class AbstractData;
class ItemRule;
class Rational;
  
#include "BasicCompetitiveRequest.h"
#include "Rational.h"

class ResourceCompetitiveRequest : public BasicCompetitiveRequest  {
public: 
	ResourceCompetitiveRequest(UnitEntity * unit, Order * orderId, ItemRule * resource,  Rational& amount);
	~ResourceCompetitiveRequest();
//  virtual bool isEqualTo (BasicCompetitiveRequest * request);
   Rational getValue() const;
   AbstractData * getType() const;
  Rational getTotalAvailableValue() const;
  void answerRequest(Rational& answer);
protected:
  ItemRule * resourceType_;
  Rational amount_;
};

#endif
