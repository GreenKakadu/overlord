/***************************************************************************
                          Entity.h  
                             -------------------
    begin                : Wen jul  15:21:12 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef ENTITY_H
#define ENTITY_H

#include "GameData.h"
class OrderElementNode;
#include "OrderElementNode.h"
#include "OverlordTypes.h"
class Entity : public GameData
{
    public:
      Entity (const string & keyword, GameData * parent );
      Entity ( const Entity * prototype );
      Entity ();
      ~Entity ();
       STATUS  initialize      ( Parser *parser ); // temporary: may be virtual?
       void      save (ostream &out);
       void      load (iostream &in);
      GameData * createInstanceOfSelf();
      void  print();
   STATUS process(int phase, ostream & out);
    protected:
      list <OrderElementNode *> orders_;
    private:
};

#endif





