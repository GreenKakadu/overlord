/***************************************************************************
                          UnitEntity.cpp 
                             -------------------
    begin                : Tue Nov 13 18:07:56 IST 2001
    copyright            : (C) 2001 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#include "UnitEntity.h"

UnitEntity::UnitEntity()
{
}


UnitEntity::UnitEntity(const string & keyword, GameData * parent)
{
    keyword_   = keyword;               
    parent_   = parent;               
    prototypeManager->addToRegistry(this);
    UnitEntity1 * sampleUnitEntity1 =  new  UnitEntity1("TYPE1", this);
    UnitEntity2 * sampleUnitEntity2 =  new  UnitEntity2("TYPE2", this);
}
UnitEntity::UnitEntity(const UnitEntity * prototype)
{
      parent_ = prototype->getParent();
      keyword_ = prototype->getKeyword();
}


GameData * UnitEntity::createInstanceOfSelf()
{
   return CREATE_INSTANCE<UnitEntity> (this);
}

STATUS 
UnitEntity::initialize        ( Parser *parser )
{

	
      if (parser->matchKeyword ("NAME") )
        {
	  setName(parser->getText());
	  return OK;
	}
      if (parser->matchKeyword("DESCRIPTION"))
	{
	  setDescription(parser->getText());
	  return OK;
	}
//       if (parser->matchKeyword("ORDER"))
// 	{
// 	  addOrder(parser->getText());
// 	}

	  return OK;

}


// void UnitEntity::addOrder(const string & order)
// {
// orders.push_back (order);
// }

void UnitEntity::print() // For debugging
{
    cout  << getName();
    cout << " [" << getTag()  << "] "<< endl;  

//  list<string>::const_iterator iter;
//  for ( iter = orders.begin(); iter != orders.end(); iter++)
//    {
//     cout  << (*iter)<< endl;
//    }
      
}



// ==============================================================
#define CONSTRUCTOR1(class) class::class(const class  * prototype   ) \
{      parent_ = prototype->getParent();      keyword_ = prototype->getKeyword();}
#define CONSTRUCTOR2(class) class::class(  ) { }
#define CONSTRUCTOR3(class) class::class(const string & keyword, GameData * parent  ) \
{ keyword_   = keyword; parent_   = parent; prototypeManager->addToRegistry(this);}
#define CLONE(class) GameData * class::createInstanceOfSelf() \
{  return CREATE_INSTANCE<class> (this);}

CONSTRUCTOR1(UnitEntity1)
CONSTRUCTOR2(UnitEntity1)
CONSTRUCTOR3(UnitEntity1)
CLONE(UnitEntity1)

CONSTRUCTOR1(UnitEntity2)
CONSTRUCTOR2(UnitEntity2)
CONSTRUCTOR3(UnitEntity2)
CLONE(UnitEntity2)


  
