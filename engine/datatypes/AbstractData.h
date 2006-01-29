/***************************************************************************
                          AbstractData.h 
    Abstract data class provides homogeneous interface for  printing
                of itnegers, strings and game objects.
                                -------------------
    begin                : Tue Mar 11 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef ABSTRACT_DATA_H
#define ABSTRACT_DATA_H
#include <string>
#include <iostream>
/**Abstract data class provides homogeneous interface for  using
   itnegers, strings and game objects as report and order parameters
  *@author Alex Dribin
  */
using namespace std;

class AbstractData {
public: 
	virtual ~AbstractData(){}
  virtual string print(){return "";}
	AbstractData(){}
   virtual void saveAsParameter (ostream &){}
   virtual void clean(){}// selective Destructor.  For persistent objects it does nothing.
                         // For  non-persistent objects derived from AbstractData
                         // it  destructs object.
  protected:
};

   inline ostream& operator << (ostream& out,  AbstractData& data)
                                { out<<data.print(); return out;}

#endif
