/***************************************************************************
                          FactionEntity.h  -  description
                             -------------------
    begin                : Tue Nov 19 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef FACTION_ENTITY_H
#define FACTION_ENTITY_H

#include <Entity.h>

class FactionEntity : public Entity  {
    public:
      FactionEntity (const string & keyword, Entity * parent );
      FactionEntity ( const FactionEntity * prototype );
      FactionEntity ();
      ~FactionEntity ();
      STATUS  initialize      ( Parser *parser );
       void      save (ostream &out);
      GameData * createInstanceOfSelf();
      void  print();
    protected:

    private:
 };

#endif
