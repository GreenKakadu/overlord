/***************************************************************************
                          EntitiesCollectionExplicit.cpp
  Template Classes for storing collection of Overlord data objects.
It includes access methods and methods for performing some generic
operations on all objects stored in the collection.

                             -------------------
    begin                : Mon Jun 10 12:24:42 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "EntitiesCollection.h"
#include "UnitEntity.h"
//#include "LocationEntity.h"
//#include "FactionEntity.h"



// this explicitly instantiates code for all collection types in Overlord


template class EntitiesCollection<UnitEntity>;




