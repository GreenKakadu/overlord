/***************************************************************************
                          OwnershipPolicyAttribute.h  -  description
                             -------------------
    begin                : Tue Jun 8 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/


#ifndef OWNERSHIP_POLICY_ATTRIBUTE_H
#define OWNERSHIP_POLICY_ATTRIBUTE_H

#include "BasicAttribute.h"

/**
  *@author Alex Dribin
  * may be used for adding multiple OwnershipPolicy to location
  * currently not used
  */

class OwnershipPolicyAttribute : public BasicAttribute  {
public: 
	OwnershipPolicyAttribute();
	~OwnershipPolicyAttribute();
};

#endif
