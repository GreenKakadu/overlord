/***************************************************************************
                          NewEntityPlaceholder.cpp 
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
#include "NewEntityPlaceholder.h"
#include "Entity.h"

NewEntityPlaceholder::NewEntityPlaceholder(const string& name)
{
//   cout << "newPlaceholder with tag " << name<< " created"<<endl;
  temporaryName_ = name;
  entity_ = 0;
}

void NewEntityPlaceholder::saveAsParameter (ostream &out)
{
 if(entity_)
  entity_->saveAsParameter(out);
  else
    out << " " << temporaryName_;
}
