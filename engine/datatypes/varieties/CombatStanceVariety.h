/***************************************************************************
                          CombatStanceVariety.h  -  description
                             -------------------
    begin                : Thu Jan 9 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef COMBAT_STANCE_H
#define COMBAT_STANCE_H

#include <iostream>
#include "Variety.h"
using namespace std;



class CombatStanceVariety : public Variety  {
public:
      CombatStanceVariety ( const string & keyword, GameData * parent): Variety(keyword, parent){}
      CombatStanceVariety ( const CombatStanceVariety * prototype ): Variety(prototype){code_ = 0;}
      ~CombatStanceVariety (){}
      virtual STATUS     initialize      ( Parser *parser);
      GameData * createInstanceOfSelf();
      bool operator <   (CombatStanceVariety stance2);  
      bool operator <=  (CombatStanceVariety stance2); 
      bool operator ==  (CombatStanceVariety stance2);
      bool operator >=  (CombatStanceVariety stance2);
      bool operator >   (CombatStanceVariety stance2);
       
  /** No descriptions */
  		inline int getCode(){return code_;}
friend  ostream &operator << ( ostream &out, CombatStanceVariety * stance);
    protected:
			int code_;
    private:
};
extern CombatStanceVariety     sampleCombatStance;
extern CombatStanceVariety *  avoidStance;
extern CombatStanceVariety *  defenceStance;
extern CombatStanceVariety *  attackStance;
extern VarietiesCollection <CombatStanceVariety>    combatStances;
extern CombatStanceVariety  *defaultCombatStance;
#endif                                              
