/***************************************************************************
                          ConstructionWorksVariety.h
                             -------------------
    begin                : Tue Sep 2 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef CONSTRUCTION_WORKS_VARIETY_H
#define CONSTRUCTION_WORKS_VARIETY_H

#include "Variety.h"
#include "VarietiesCollection.h"

/**Represents Type of construction work s that some building or ship may demand. 
  *@author Alex Dribin
  */

class ConstructionWorksVariety : public Variety  {
public:
      ConstructionWorksVariety ( const string & keyword, GameData * parent) : Variety(keyword, parent){}
      ConstructionWorksVariety ( const ConstructionWorksVariety * prototype );
      virtual STATUS     initialize      ( Parser *parser);
      GameData * createInstanceOfSelf();
	~ConstructionWorksVariety();
  inline string getPluralName() const{return pluralName_;}
	protected:
	  string pluralName_;
};
extern ConstructionWorksVariety   sampleConstructionWork;
//extern VarietiesCollection <ConstructionWorksVariety>      construction_works;


#endif
