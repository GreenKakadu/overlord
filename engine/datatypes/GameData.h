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
#include "OverlordTypes.h"
#include "BasicData.h"
using namespace std;

class ProcessingMode;
class PrototypeManager;
/**Base class for polymorphic persistent  data
  *@author Alex Dribin
  */

class GameData : public BasicData
{
    public:
                    GameData (const string & keyword, GameData * parent );
                    GameData (const GameData * prototype);
   virtual         ~GameData ( ){}

           string   getKeyword()      const;
           GameData * getParent()       const;
//           GameData * checkObjectType  ( const string  &keyword); 
           GameData * createByKeyword(const string &keyword);
   virtual GameData * createInstanceOfSelf ();
   virtual STATUS      initialize ( Parser *parser );
   virtual void      save (ostream &out);
   
  /** Checks data consistency */
   virtual STATUS dataConsistencyCheck();
   static PrototypeManager * prototypeManager;
           bool operator ==  (GameData data2);

    protected:
           string   keyword_;
           GameData * parent_;
    private:

};


template <class T>  GameData * CREATE_INSTANCE(const T * prototype)
{
      T * object = new T(prototype); 
      return object;
     
}
#include <functional>
class TagSearch : public binary_function<GameData *, string *, bool> {
public:
   bool operator () (const GameData * data, string * tag) const
      { return data->getTag() == *tag; }
};


      
#endif
