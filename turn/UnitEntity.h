/***************************************************************************
                          UnitEntity.h  
                             -------------------
    begin                : Tue Nov 13 18:21:12 IST 2001
    copyright            : (C) 2001 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef UNIT_H
#define UNIT_H
#include "GameData.h"


class UnitEntity : public GameData
{
    public:
      UnitEntity (const string & keyword, GameData * parent );
      UnitEntity ( const UnitEntity * prototype );
      UnitEntity ();
      virtual STATUS  initialize      ( Parser *parser );
      GameData * createInstanceOfSelf();
      void  print();



    protected:
      /*   list <Order> orders; */
    private:
};
class UnitEntity1 : public UnitEntity
{
    public:
      UnitEntity1 (const string & keyword, GameData * parent );
      UnitEntity1 (const UnitEntity1 * prototype  );
      UnitEntity1 ( );
      GameData * createInstanceOfSelf();
    private:
};
#define CLASS_DEFINITION( type, base) class type : public base{ \
public: \
   type (const string & keyword, GameData * parent );\
   type (const type * prototype  );\
   type ( );\
   GameData * createInstanceOfSelf();\
private:\
};
CLASS_DEFINITION(UnitEntity2,UnitEntity)

#endif

