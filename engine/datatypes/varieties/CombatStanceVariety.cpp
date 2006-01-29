/***************************************************************************
                          CombatStanceVariety.cpp  
                             -------------------
    begin                : Thu Jan 9 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "CombatStanceVariety.h"

CombatStanceVariety    sampleCombatStance    ("COMBAT_STANCE",   &sampleGameData);
VarietiesCollection <CombatStanceVariety>    combatStances(new DataStorageHandler("combat_stances.var"));
//CombatStanceVariety * findStanceByTag(const string &tag)
//{
// return GET_FROM_COLLECTION<CombatStanceVariety>(&stances,tag);
//}
CombatStanceVariety *  avoidStance = 0;
CombatStanceVariety *  defenceStance= 0;
CombatStanceVariety *  attackStance= 0;
CombatStanceVariety * defaultCombatStance = 0;





GameData * CombatStanceVariety::createInstanceOfSelf()
{
  return CREATE_INSTANCE<CombatStanceVariety> (this);
}


STATUS
CombatStanceVariety::initialize        ( Parser *parser )
{

	
  if (parser->matchKeyword ("NAME") )
    {
      setName(parser->getText());
      return OK;
    }
  if (parser->matchKeyword("DESCRIPTION"))
    {
      setDescription(parser->getText());
      return OK;
    }
  if (parser->matchKeyword("CODE"))
    {
      code_ = parser->getInteger();
      return OK;
    }
      return OK;

 }



ostream &operator << ( ostream &out, CombatStanceVariety * stance)
{
	out << stance->getTag() << " ";
	return out;
}



bool CombatStanceVariety::operator <   (CombatStanceVariety stance2)
{
  return this->code_ <  stance2.getCode();
}

bool CombatStanceVariety::operator <=  (CombatStanceVariety stance2)
{
  return this->code_ <= stance2.getCode();
}

bool CombatStanceVariety::operator ==  (CombatStanceVariety stance2)
{
  return this->code_ == stance2.getCode();
}

bool CombatStanceVariety::operator >=  (CombatStanceVariety stance2)
{
  return this->code_ >= stance2.getCode();
}


bool CombatStanceVariety::operator >   (CombatStanceVariety stance2)
{
  return this->code_ >  stance2.getCode();
}


