/***************************************************************************
                          StanceVariety.cpp  
                             -------------------
    begin                : Thu Jan 9 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "StanceVariety.h"
StanceVariety    sampleStance    ("STANCE",   &sampleGameData);
VarietiesCollection <StanceVariety>    stances(new DataStorageHandler("stances.var"));

//StanceVariety * findStanceByTag(const string &tag)
//{
// return GET_FROM_COLLECTION<StanceVariety>(&stances,tag);
//}


StanceVariety *  alliedStance;
StanceVariety *  friendlyStance;
StanceVariety *  neutralStance;
StanceVariety *  hostileStance;
StanceVariety *  enemyStance;

GameData * StanceVariety::createInstanceOfSelf()
{
  return CREATE_INSTANCE<StanceVariety> (this);
}


STATUS
StanceVariety::initialize        ( Parser *parser )
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



ostream &operator << ( ostream &out, StanceVariety * stance)
{
	out << stance->getTag() << " ";
	return out;
}



bool StanceVariety::operator <   (StanceVariety stance2)
{
  return this->code_ <  stance2.getCode();
}

bool StanceVariety::operator <=  (StanceVariety stance2)
{
  return this->code_ <= stance2.getCode();
}

bool StanceVariety::operator ==  (StanceVariety stance2)
{
  return this->code_ == stance2.getCode();
}

bool StanceVariety::operator >=  (StanceVariety stance2)
{
  return this->code_ >= stance2.getCode();
}


bool StanceVariety::operator >   (StanceVariety stance2)
{
  return this->code_ >  stance2.getCode();
}


