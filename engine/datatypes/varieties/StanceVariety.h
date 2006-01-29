/***************************************************************************
                          StanceVariety.h  -  description
                             -------------------
    begin                : Thu Jan 9 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef STANCE_H
#define STANCE_H

#include <iostream>
#include "Variety.h"
using namespace std;



class StanceVariety : public Variety  {
public:
      StanceVariety ( const string & keyword, GameData * parent): Variety(keyword, parent){}
      StanceVariety ( const StanceVariety * prototype ): Variety(prototype){code_ = 0;}
      ~StanceVariety (){}
      virtual STATUS     initialize      ( Parser *parser);
      GameData * createInstanceOfSelf();
      bool operator <   (StanceVariety stance2);  
      bool operator <=  (StanceVariety stance2); 
      bool operator ==  (StanceVariety stance2);
      bool operator >=  (StanceVariety stance2);
      bool operator >   (StanceVariety stance2);
       
  /** No descriptions */
  		inline int getCode(){return code_;}
friend  ostream &operator << ( ostream &out, StanceVariety * stance);
    protected:
			int code_;
    private:
};
extern StanceVariety     sampleStance;
extern StanceVariety *  alliedStance;
extern StanceVariety *  friendlyStance;
extern StanceVariety *  neutralStance;
extern StanceVariety *  hostileStance;
extern VarietiesCollection <StanceVariety>    stances;
extern StanceVariety    sampleStance;
#endif
