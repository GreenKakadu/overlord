/***************************************************************************
                          RulesCollectionExplicit.cpp
  Template Classes for storing collection of Overlord data objects.
It includes access methods and methods for performing some generic
operations on all objects stored in the collection.

                             -------------------
    begin                : Mon Jun 10 12:24:42 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "RulesCollection.h"

// #include "Title.h"
// #include "Skill.h"
// #include "Item.h"
// #include "Race.h"
// #include "MovementMode.h"
#include "TerrainRule.h"



// this explicitly instantiates code for all collection types in Overlord

// template class RulesCollection<Title>;
// template class RulesCollection<Skill>;
// template class RulesCollection<Item>;
// template class RulesCollection<Race>;
// template class RulesCollection<MovementMode>;
template class RulesCollection<TerrainRule>;




