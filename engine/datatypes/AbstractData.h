/***************************************************************************
                          AbstractData.h 
    Abstract data class provides homogeneous interface for  printing
                of itnegers, strings and game objects.
                                -------------------
    begin                : Tue Mar 11 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef ABSTRACT_DATA_H
#define ABSTRACT_DATA_H
#include <string>
#include <vector>
#include <iostream>

/**Abstract data class provides homogeneous interface for  using
   itnegers, strings and game objects as report and order parameters
  *@author Alex Dribin
  */
using namespace std;
class Parser;

class AbstractData {
public: 
	virtual ~AbstractData(){}
  virtual string print(){return "";}
/*
 * aPrint is a function that performs print() in a form convinient for interpretaion in GUI application 
 *  as a sequence of  AbstractData tokens.
 */ 
        virtual vector <AbstractData *> aPrint();
	AbstractData(){}
   static        AbstractData * createByKeyword(const string &keyword);
   static        AbstractData * loadByKeyword(const string &keyword, Parser *parser);
   virtual AbstractData * createAbstractInstance();
   virtual AbstractData * loadInstance(Parser *parser);
   virtual inline  string     getKeyword() const{ return "ABSTRACT";}
   virtual void saveAsParameter (ostream &){}
   virtual void clean(){}// selective Destructor.  For persistent objects it does nothing.
                         // For  non-persistent objects derived from AbstractData
                         // it  destructs object.
  protected:
};

   inline ostream& operator << (ostream& out,  AbstractData& data)
                                { out<<data.print(); return out;}

typedef vector <AbstractData *> AbstractArray;

#endif
