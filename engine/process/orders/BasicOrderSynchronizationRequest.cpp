/***************************************************************************
                          BasicOrderSynchronizationRequest.cpp 
                             -------------------
    begin                : Sun Dec 28 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "BasicOrderSynchronizationRequest.h"
bool BasicOrderSynchronizationRequest::isEqual(BasicOrderSynchronizationRequest  * request)
{
  if (request->getType() != this->getType())
      return false;
  return isMatch(request);    
}                  

