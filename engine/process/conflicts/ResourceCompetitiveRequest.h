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
class RationalNumber;
  
#include "BasicCompetitiveRequest.h"
#include "RationalNumber.h"

class ResourceCompetitiveRequest : public BasicCompetitiveRequest  {
public: 
	ResourceCompetitiveRequest(UnitEntity * unit, OrderLine * orderId, ItemRule * resource,  RationalNumber& amount);
	~ResourceCompetitiveRequest();
//  virtual bool isEqualTo (BasicCompetitiveRequest * request);
   RationalNumber getValue() const;
   AbstractData * getType() const;
  RationalNumber getTotalAvailableValue() const;
  void answerRequest(RationalNumber& answer);
protected:
  ItemRule * resourceType_;
  RationalNumber amount_;
};

#endif
