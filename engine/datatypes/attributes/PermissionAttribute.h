/***************************************************************************
                          PermissionAttribute.h  -  description
                             -------------------
    begin                : Tue Jun 8 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/


#ifndef PERMISSION_ATTRIBUTE_H
#define PERMISSION_ATTRIBUTE_H

#include "BasicAttribute.h"

/**
  *@author Alex Dribin
  * may be used for adding multiple PermissionElement to OwnershipPolicy
  * currently not used
  */

class PermissionAttribute : public BasicAttribute  {
public: 
	PermissionAttribute();
	~PermissionAttribute();
};

#endif
