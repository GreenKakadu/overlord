/* 
 * File:   OvlComboBox.cpp
 * Author: alex
 * 
 * Created on February 14, 2012, 10:31 PM
 */

#include "OvlComboBox.h"
#include "GameData.h"
#include "GameFacade.h"

OvlComboBox::OvlComboBox(GameData * data) :QComboBox()
{
  maxLen_ = 0;
  setEditable(true);
  addGameItem(data);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}



OvlComboBox::~OvlComboBox() {
}

void OvlComboBox::addGameItem (GameData * item, bool showTag)
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
  else
  {
    string = item->getName().c_str();  
  }
  size =string.size(); 
  addItem(string);
  if(size > maxLen_ )
  {
      maxLen_ = size;
      setMinimumContentsLength(maxLen_);
  }
    }
}
GameData * OvlComboBox::getCurrentItem()
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
        GameData * data = gameFacade->getDataManipulator()->findGameDataByName(currentText().toStdString());
        if (data == 0) // try interpret string as tag
        {
            GameData * data = gameFacade->getDataManipulator()->findGameData(currentText().toStdString());
            if (data == 0)
            {
                cout << "OvlComboBox::getCurrentItem() name [" << currentText().toStdString() << "] was not found in game data " << endl;
            }
        }
        return data;
    }
    //cout<<"OvlComboBox::getCurrentItem() index is "<<index <<" size= "<<items_.size()<<endl;
    return 0;
}
