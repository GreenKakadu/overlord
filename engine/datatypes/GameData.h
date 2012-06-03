/***************************************************************************
                          GameData.h
                    Basic Polymorphic Persistent Object.
                             -------------------
    begin                : Fri Nov  9 19:24:42 IST 2001
    copyright            : (C) 2001 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

#ifndef GAME_DATA_H
#define GAME_DATA_H
#include "OverlordTypes.h"
#include "AbstractData.h"
#include "DataStorageHandler.h"
using namespace std;

class ProcessingMode;
class PrototypeManager;
/**Base class for polymorphic persistent  data
  *@author Alex Dribin
  */

class GameData : public AbstractData
{
    public:
                    GameData (const string & keyword, GameData * parent );
                    GameData (const GameData * prototype);
                    GameData ();
   virtual         ~GameData ( ){}

   static        GameData * createByKeyword(const string &keyword);
   AbstractData * createAbstractInstance();
   virtual GameData * createInstanceOfSelf ();
   AbstractData *  loadInstance(Parser *parser);
          string   getTag()          const;
  inline  string   getName()         const { return name_;}
          string   getDescription()  const;
  inline  void     setTag          ( const string  &tag) {  tag_ = tag;}
  inline  void     setName         ( const string  &name){ name_ = name;}
  inline  void     setDescription  ( const string  &description){ description_ = description;}
          void     saveAsParameter (ostream &out);
          string   printTag() const;
  virtual string   print() /*const*/;

     bool isDescendantFrom  (GameData * object);
   virtual STATUS     initialize ( Parser *parser );
   virtual void       save (ostream &out);
   virtual void save(ostream &out, string prefix);
   static  void       staticInitialization (){}

   inline  string     getKeyword() const{ return keyword_;}
   inline  GameData * getParent() const { return parent_;}

   virtual STATUS dataConsistencyCheck();
           bool operator ==  (GameData data2);
           static GameData *  registerObject();
           static GameData *  objectParent;
           static string objectKeyword;
    protected:
           string   keyword_;
           GameData * parent_;
  static const string l_bracket;
  static const string r_bracket;
           string   tag_;                //unigue object identifier
           string   name_;               //name of the object
           string   description_;        //description of the object
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
