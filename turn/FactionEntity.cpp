/***************************************************************************
                          FactionEntity.cpp  -  description
                             -------------------
    begin                : Tue Nov 19 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "FactionEntity.h"

FactionEntity::FactionEntity()
{
}

FactionEntity::~FactionEntity()
{
  list<OrderElementNode *>::const_iterator iter;
  for ( iter = orders_.begin(); iter != orders_.end(); iter++)
    {
      // delete (*iter);
    }
}


FactionEntity::FactionEntity(const string & keyword, Entity * parent)
{
    keyword_   = keyword;
    parent_   = parent;
    prototypeManager->addToRegistry(this);
}
FactionEntity::FactionEntity(const FactionEntity * prototype)
{
      parent_ = prototype->getParent();
      keyword_ = prototype->getKeyword();
}


GameData * FactionEntity::createInstanceOfSelf()
{
   return CREATE_INSTANCE<FactionEntity> (this);
}

STATUS
FactionEntity::initialize        ( Parser *parser )
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
       if (parser->matchKeyword("ORDER"))
 	{
	  //          cout << "Reading order..." << endl;
	  orders_.push_back(new OrderElementNode(new OrderElement(parser->getText())));
 	}

	  return OK;

}
void
FactionEntity::save(ostream &out)
{
  out << keyword_ << " " <<tag_ << endl;
  if(name_.size()) out << "NAME " <<name_ << endl;
  if(description_.size()) out << "DESCRIPTION " <<description_  << endl;
  out << endl;
  list<OrderElementNode *>::const_iterator iter;
  for ( iter = orders_.begin(); iter != orders_.end(); iter++)
    {
           ((*iter)->getOrderElement())->save(out);
    }
}


void FactionEntity::print() // For debugging
{
    cout  << getName();
    cout << " [" << getTag()  << "] "<< endl;

  list<OrderElementNode *>::const_iterator iter;
  for ( iter = orders_.begin(); iter != orders_.end(); iter++)
    {
           ((*iter)->getOrderElement())->save(cout);
    }

}

