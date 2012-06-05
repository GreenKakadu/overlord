/* 
 * File:   OvlElementComboBox.cpp
 * Author: alex
 * 
 * Created on May 29, 2012, 11:30 AM
 */

#include "OvlElementComboBox.h"
#include "OvlComboBox.h"
#include "AbstractData.h"
#include "GameFacade.h"
#include "TitleElement.h"

OvlElementComboBox::OvlElementComboBox(AbstractData * data) :QComboBox()
{
  maxLen_ = 0;
  setEditable(true);
  addGameItem(data);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}



OvlElementComboBox::~OvlElementComboBox() {
}

void OvlElementComboBox::addGameItem (AbstractData * item, bool showTag)
{
    if(item)
    {
  items_.push_back(item);
  int size;
  QString string;

  if(showTag)
  {
      string = item->print().c_str(); 
  }
//  else
//  {
//    string = item->getName().c_str();  
//  }
  size =string.size(); 
  addItem(string);
  if(size > maxLen_ )
  {
      maxLen_ = size;
      setMinimumContentsLength(maxLen_);
  }
    }
}
AbstractData * OvlElementComboBox::getCurrentItem()
{
    int index = currentIndex();
    if (index < 0)
    {
        return 0;
    }
    if (index < items_.size())
    {
        return items_[index];
    }
    if (index >= items_.size())
    {
        Parser * parser = new Parser(currentText().toStdString());
        AbstractData * data = TitleElement::readElement(parser);
        return data;
    }
    //cout<<"OvlComboBox::getCurrentItem() index is "<<index <<" size= "<<items_.size()<<endl;
    return 0;
}


