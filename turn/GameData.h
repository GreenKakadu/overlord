/***************************************************************************
                          GameData.h 
                    Basic Polymorphic Persistent Object.
                             -------------------
    begin                : Fri Nov  9 19:24:42 IST 2001
    copyright            : (C) 2001 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef GAME_DATA_H
#define GAME_DATA_H

#include "Parser.h"



class PrototypeManager;


class GameData
{
public:

                    GameData (const string & keyword, GameData * parent );
                    GameData ( );
                    GameData (const GameData * prototype);
   virtual         ~GameData ( ){};

           string   getKeyword()      const;
           GameData * getParent()       const;
             

           GameData * checkObjectType  ( const string  &keyword); 
   virtual GameData * createInstanceOfSelf ();
   virtual int      initialize ( Parser *parser );
   virtual void      print(); // for debugging
   static PrototypeManager * prototypeManager;

  string   getTag()          const;
           string   getName()         const;
           string   getDescription()  const;
             
           void      setTag           ( const string  &tag);
           void      setName          ( const string  &name);
           void      setDescription    ( const string  &description);
protected:
           string   keyword_;            
           GameData * parent_;
private:
           string   tag_;                //unigue object identifier
           string   name_;               //name of the object
           string   description_;        //description of the object


};


template <class T>  GameData * CREATE_INSTANCE(const T * prototype)
{
  T * object = new T(prototype); 
  return object;
     
}
#include "PrototypeManager.h"

      
#endif
