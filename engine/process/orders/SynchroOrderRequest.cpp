/***************************************************************************
                          SynchroOrderRequest.cpp 
                             -------------------
    begin                : Mon Dec 29 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
 

#include "SynchroOrderRequest.h"

SynchroOrderRequest::SynchroOrderRequest(OrderPrototype * orderType,
                  PhysicalEntity * synchroTarget)
                    :BasicOrderSynchronizationRequest(orderType)
{
   synchroTarget_ = synchroTarget;
}


/*
 *
 */
bool SynchroOrderRequest::isMatch(BasicOrderSynchronizationRequest  * request)
{
  SynchroOrderRequest  * synchroRequest  = dynamic_cast<SynchroOrderRequest *>(request);
  if(synchroRequest == 0)
    return false;
  return(this->getSynchroTarget() ==  synchroRequest->getSynchroTarget() );
}

