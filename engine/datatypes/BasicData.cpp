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



/** returns tag for use in reports and messages
 *  (enbraced in square brackets)
 */
string BasicData::printTag() const{
 return ( l_bracket + tag_ + r_bracket);
}



/** returns name for use in reports and messages
 * (followed by tag enbraced in square brackets)
 */
string BasicData::printName() /*const*/
{
 return (name_ + l_bracket + tag_ + r_bracket);
}



/** returns data identificator in a form of parameter accepted by order */
void      BasicData::saveAsParameter (ostream &out)
{
  out <<  " " <<tag_;
}



string   BasicData::getDescription()  const
{
  if(description_.empty())
    return string("No description");
  else
    return description_;
}
