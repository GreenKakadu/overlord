/***************************************************************************
                          TitleElement.h
                             -------------------
    begin                : Wed Sep 3 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#ifndef TITLE_ELEMENT_H
#define TITLE_ELEMENT_H
#include "RulesCollection.h"
#include "EntitiesCollection.h"
class UnitEntity;
class LocationEntity;
class TitleRule;

using namespace std;
extern RulesCollection <TitleRule>     titles;
extern EntitiesCollection <LocationEntity>     locations;
extern EntitiesCollection <UnitEntity>     units;


/**
  *@author Alex Dribin
  */

class TitleElement: public AbstractData  {
public: 
	TitleElement(TitleRule * title, LocationEntity * location,
               UnitEntity * unit);
	~TitleElement(){}
   inline TitleRule *    getTitle()      const     {return title_;}
   inline LocationEntity * getTitleLocation()    const     {return location_;}
   inline UnitEntity *    getTitleHolder()      const     {return unit_;}
          BasicCondition *    getClaimingCondition()      const;
   inline void         setLocation(LocationEntity * location)     { location_ = location;}
   inline void   setTitleHolder(UnitEntity * unit)     {unit_ = unit;}
   inline bool isValidElement() const {return (( title_ != 0 ) && (location_ != 0));}
   void save(ostream & out);
   string print();
   void produceReport(ostream & out);
   static TitleElement  * readElement (Parser * parser);
   bool contest(UnitEntity * contender);
   void activateClaimingEffects();
   void desactivateClaimingEffects();
   bool mayCancelTitle(TitleElement * title);

	protected:
    TitleRule * title_;
    LocationEntity * location_;
    UnitEntity * unit_;
};
typedef vector <TitleElement *>::iterator TitleIterator;

#endif
