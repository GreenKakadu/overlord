/***************************************************************************
                          NewEntityPlaceholder.h
                             -------------------
    begin                : Tue Jun 3 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef NEW_ENTITY_PLACEHOLDER_H
#define NEW_ENTITY_PLACEHOLDER_H

#include "AbstractData.h"
class Entity;

/**
  *@author Alex Dribin
  */

class NewEntityPlaceholder : public AbstractData  {
public: 
	NewEntityPlaceholder(const string& name);
	~NewEntityPlaceholder(){cout << "newPlaceholder destroyed "<<endl;}
  inline Entity * getRealEntity() {return entity_;}
  inline void     setRealEntity(Entity * entity) {entity_ = entity;}
         void saveAsParameter (ostream &out);
  inline string getName() const {return temporaryName_;}       
private:
  Entity * entity_;
  string temporaryName_; 
};

#endif
