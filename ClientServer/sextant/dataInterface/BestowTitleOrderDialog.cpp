/* 
 * File:   BestowTitleOrderDialog.cpp
 * Author: alex
 * 
 * Created on May 1, 2012, 12:40 PM
 */

#include "BestowTitleOrderDialog.h"
#include <sstream>
#include "TitleRule.h"
#include "ViewManager.h"
#include "LocationEntity.h"
#include "UnitEntity.h"
#include "ClickableLabel.h"
// BESTOW title-tag location-id unit-id 

// Still Doesn't work


OrderWindow * BestowTitleOrderDialog::show(TokenEntity * token, 
     OrderLine * order, ViewManager * view, ExtendedCommand extendedCommandFlag)
{
    TitleRule * data=0;
    TitleElement * title;
    LocationEntity * location=0;
    UnitEntity * targetUnit = 0;

    data = dynamic_cast<TitleRule *>(view->getActiveData());
    location = dynamic_cast<LocationEntity *>(view->getSelectedLocation()); 
            if((data)&&(location))
            {
              title = new TitleElement(data,location,0);  
            }


     
     OrderWindow * orderWindow = this->prepareWindow(token,order,view,"Bestow");
     if(orderWindow==0)
     {
         return 0;
     }
    
     UnitEntity * unit =  dynamic_cast<UnitEntity *>(token);
     if(unit==0)
     {
         return 0;
     }
// for existing order determine parameters      
     if(!isNewOrder_ && params.size() >= 2)
     {
            data = dynamic_cast<TitleRule *>(params[0]);
            if(!data) 
            {
                    cerr<<"ERROR: wrong  parameter ["<<(params[0])->print()<<"] in order "
                    <<order->print() <<" for "<<token <<endl;               
            }        
            location = dynamic_cast<LocationEntity *>(params[1]);
            if(!location) 
            {
                    cerr<<"ERROR: wrong  parameter ["<<(params[1])->print()<<"] in order "
                    <<order->print() <<" for "<<token <<endl;               
            } 
            if((data)&&(location))
            {
                if(title)
                {
                    delete title;
                }
              title = new TitleElement(data,location,0);  
            }
     }
     
     if(title)
     {
        titleCB_ = new OvlElementComboBox(title);  
     }
     else
            {
                 cerr<<"ERROR: wrong  title definition in order for "<<token <<endl;  
            }

     if(!isNewOrder_ && params.size() >= 3)
     {
            targetUnit = dynamic_cast<UnitEntity *>(params[2]);
            if(!targetUnit) 
            {
                    cerr<<"ERROR: wrong  parameter ["<<(params[2])->print()<<"] in order "
                    <<order->print() <<" for "<<token <<endl;               
            }        
     }
     

       unitCB_ = new OvlComboBox(targetUnit);

  // Add items to ComboBoxes and add widgets     
    orderWindow->addWidget(titleCB_); 
    vector < TitleElement *> * titles =   unit->getTitlesAttribute()->getAll();
    for( vector < TitleElement *>::iterator iter =  titles->begin(); iter != titles->end(); ++iter)
    {
        if((*iter)->getTitle() != data || (*iter)->getTitleLocation() != location ) // Already added
        {
           titleCB_->addGameItem ((*iter));
        }
    }
       
      ClickableLabel * l2 = new ClickableLabel(" to ");
     orderWindow->addWidget(l2);    
// Add items to ComboBoxes and add widgets     
    orderWindow->addWidget(unitCB_); 
//    vector < TitleElement *> * titles =   unit->getTitlesAttribute()->getAll();
    for( vector < TitleElement *>::iterator iter =  titles->begin(); iter != titles->end(); ++iter)
    {
        if((*iter)->getTitle() != data) // Already added
        {
           unitCB_->addGameItem ((*iter)->getTitle());
        }
    }
    

  





      showWindow(orderWindow);
      return orderWindow;
}



OrderLine * BestowTitleOrderDialog::getOrderLine()
{
    stringstream s;
    string titleTag;
    string locationTag;
    string unitTag;
    
    AbstractData * data = titleCB_->getCurrentItem();
    if(data)
    {
      TitleElement *  title = dynamic_cast<TitleElement *>(data);
      if(title)
      {
        titleTag = title->getTitle()->getTag();  
        locationTag = title->getTitleLocation()->getTag();
      }  
    }    
    
    GameData * unit = unitCB_->getCurrentItem();
    if(unit)
    {
      unitTag = unit->getTag();  
    }    
    s <<keyword_<<" "<<titleTag<<" "<<locationTag<<" "<<unitTag<<endl;
    
    return updateOrderLine(s.str());   
}

