/***************************************************************************
                          BasicData.h
                    Basic  Object for game data.
                             -------------------
    begin                : Sun Mar 16 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef BASIC_DATA_H
#define BASIC_DATA_H
#include "AbstractData.h"

/**Base class for all game data
  *@author Alex Dribin
  */

class BasicData : public AbstractData  {
public: 
	BasicData(){}
	~BasicData(){}
          string   getTag()          const;
  inline  string   getName()         const { return name_;}
          string   getDescription()  const;
  inline  void     setTag          ( const string  &tag) {  tag_ = tag;}
  inline  void     setName         ( const string  &name){ name_ = name;}
  inline  void     setDescription  ( const string  &description){ description_ = description;}
          void     saveAsParameter (ostream &out);
  string printTag() const;
  virtual string print() /*const*/;
    protected:
  static const string l_bracket;
  static const string r_bracket;
           string   tag_;                //unigue object identifier
           string   name_;               //name of the object
           string   description_;        //description of the object
};

#endif
