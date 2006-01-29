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
#include "OverlordTypes.h"
#include "BasicData.h"
#include "LineParser.h"
#include "DataStorageHandler.h"
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

   static        GameData * createByKeyword(const string &keyword);
   virtual GameData * createInstanceOfSelf ();
     bool isDescendantFrom  (GameData * object);
   virtual STATUS     initialize ( Parser *parser );
   virtual void       save (ostream &out);
   static  void       staticInitialization (){}

   inline  string     getKeyword() const{ return keyword_;}
   inline  GameData * getParent() const { return parent_;}

   virtual STATUS dataConsistencyCheck();
           bool operator ==  (GameData data2);

    protected:
           string   keyword_;
           GameData * parent_;
    private:

};


/** Macro for cloning */
template <class T>  GameData * CREATE_INSTANCE(const T * prototype)
{
      T * object = new T(prototype);
      return object;

}
/** Function object for use in STL algorithms (not used yet) */
#include <functional>
class TagSearch : public binary_function<GameData *, string *, bool> {
public:
   bool operator () (const GameData * data, string * tag) const
      { return data->getTag() == *tag; }
};
extern GameData       sampleGameData;
#endif
