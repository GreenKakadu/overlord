/***************************************************************************
                          BasicData.cpp 
                             -------------------
    begin                : Sun Mar 16 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "BasicData.h"
const string BasicData::l_bracket("[");
const string BasicData::r_bracket("]");
string   BasicData::getTag()          const { return tag_;}
void BasicData::print()
{
    cout  << getName() << " " << printTag()  << endl;

}

/** No descriptions */
string BasicData::printTag() const{
 return ( l_bracket + tag_ + r_bracket);
}

/** No descriptions */
string BasicData::printName() /*const*/{
 return (name_ + l_bracket + tag_ + r_bracket);
}



void      BasicData::saveAsParameter (ostream &out)
{
  out <<  " " <<tag_;
}
