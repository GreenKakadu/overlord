/***************************************************************************
                          TitleRule.cpp  
                             -------------------
    begin                : Fri Nov  9 19:24:42 IST 2001
    copyright            : (C) 2001 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "TitleRule.h"
#include "PrototypeManager.h"


TitleRule::TitleRule(const string & keyword, GameData* parent) : Rule(keyword, parent)
{
    name_    = "Sample";
    keyword_   = keyword;
    parent_   = parent;
  prototypeManager->addToRegistry(this);
}

TitleRule::TitleRule(const TitleRule * prototype) : Rule(prototype)
{
    cost_    = 0;
    control_ = 0;
    type_    = 0;
    range_   = 0;
}



GameData* TitleRule::createInstanceOfSelf()
{
  return CREATE_INSTANCE<TitleRule> (this);
}



STATUS 
TitleRule::initialize        ( Parser *parser)
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
  if (parser->matchKeyword ("SKILL") )
    {
      //            setSkill(skills[ parser->getWord()]);
      
      return OK;
    }
  if (parser->matchKeyword ("LEVEL") )
    {
      //  setSkill( getSkills()->matchLevel( parser->getInteger() ) );
      return OK;
    }
  if ( parser->matchKeyword ("COST") )
    {
      setCost( parser->getInteger() );
      return OK;
    }
  if ( parser->matchKeyword ("CONTROL") )
    {
      setControl( parser->getInteger() );
      return OK;
    }
  if ( parser->matchKeyword ("TYPE") )
    {
      setType( parser->getInteger() );
      return OK;
    }
  if ( parser->matchKeyword ("RANGE") )
    {
      setRange( parser->getInteger() );
      return OK;
    }
  
  return OK;
}



// Skill* 
// TitleRule::getSkill() const  
//  {
//    return skill_;
//  }

void TitleRule::print()
{
    cout  << getName();
    cout << " [" << getTag()  << "] ";
    cout << " Type:" << getType();
    cout << " Cost:" << getCost();
    cout << " Control:" << getControl();
    cout << " Range:" << getRange()<< endl;
      
}



int 
TitleRule::getCost()    const  
{
  return cost_;
}



int 
TitleRule::getControl() const  
{
  return control_;
}



int 
TitleRule::getType()    const  
{
  return type_;
}



int 
TitleRule::getRange()   const  
{
  return range_;
}





void 
TitleRule::setCost        ( int cost)      
{
  cost_=cost;
}



void 
TitleRule::setControl     ( int control) 
{
  control_=control;
}



void 
TitleRule::setType        ( int type)      
{
  type_=type;
}



void 
TitleRule::setRange       ( int range)    
{
  range_=range;
}




// void 
// TitleRule::setSkill( Skill *skill)
// {
//   skill_=skill;
// }


void OverlordTitleRule::print()
{
    cout  << getName();
    cout << " [" << getTag()  << "] ";
    cout << " Type:" << getType();
    cout << " Cost:" << getCost();
    cout << " Control:" << getControl();
    cout << " Range:" << getRange()<< " <-- This is Overlord (Polymorphism test)"<<endl;

}

GameData* OverlordTitleRule::createInstanceOfSelf()
{
  return CREATE_INSTANCE<OverlordTitleRule> (this);
}

GameData* StandardTitleRule::createInstanceOfSelf()
{
  return CREATE_INSTANCE<StandardTitleRule> (this);
}

GameData* MajorTitleRule::createInstanceOfSelf()
{
  return CREATE_INSTANCE<MajorTitleRule> (this);
}

GameData* MinorTitleRule::createInstanceOfSelf()
{
  return CREATE_INSTANCE<MinorTitleRule> (this);
}

