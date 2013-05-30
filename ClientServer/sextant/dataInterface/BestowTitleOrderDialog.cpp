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
#include "FactionEntity.h"
#include "ClickableLabel.h"
// BESTOW title-tag location-id unit-id 

// Still Doesn't work


OrderWindow * BestowTitleOrderDialog::show(TokenEntity * token, 
     OrderLine * order, ViewManager * view, ExtendedCommand extendedCommandFlag)
{
    TitleRule * data=0;
    TitleElement * title=0;
    LocationEntity * location=0;
    UnitEntity * targetUnit = 0;

    data       = dynamic_cast<TitleRule *>(view->getActiveData());// active data may be Title or target unit
    targetUnit = dynamic_cast<UnitEntity *>(view->getActiveData());


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


     titleCB_ = new OvlElementComboBox(title);// title may be 0


     if(!isNewOrder_ && params.size() >= 3)
     {
            targetUnit = dynamic_cast<UnitEntity *>(params[2]);
            if(!targetUnit) 
            {
                    cerr<<"ERROR: wrong  parameter ["<<(params[2])->print()<<"] in order "
                    <<order->print() <<" for "<<token <<endl;               
            }        
     }
     

       unitCB_ = new OvlComboBox(targetUnit);// <- here should be TitleElement // Why?

  // Add items to ComboBoxes and add widgets


    vector < TitleElement *> * titles =   unit->getTitlesAttribute()->getAll();
   for( vector < TitleElement *>::iterator iter =  titles->begin(); iter != titles->end(); ++iter)
    {
        if((*iter)->getTitle() != data || (*iter)->getTitleLocation() != location ) // Already added // Not added
        {
           titleCB_->addGameItem ((*iter));
        }
    }
      orderWindow->addWidget(titleCB_);
      ClickableLabel * l2 = new ClickableLabel(" to ");
     orderWindow->addWidget(l2);    
// Add items to ComboBoxes and add widgets     

////    vector < TitleElement *> * titles =   unit->getTitlesAttribute()->getAll();
//    for( vector < TitleElement *>::iterator iter =  titles->begin(); iter != titles->end(); ++iter)
//    {
//        if((*iter)->getTitle() != data) // Already added
//        {
//           unitCB_->addGameItem ((*iter)->getTitle());
//        }
//    }
    

  
    // get list of all units here, starting from own
    vector <UnitEntity *> units = unit->getLocation()->unitsPresent();
    for (vector <UnitEntity *>::iterator iter = units.begin(); iter != units.end(); ++iter)
    {
        if ((*iter)->getFaction() == unit->getFaction())
        {
            unitCB_->addGameItem((*iter));
        }
    }
    //  Now Allies
    for (vector <UnitEntity *>::iterator iter = units.begin(); iter != units.end(); ++iter)
    {
        if (((*iter)->getFaction() != unit->getFaction()) &&
                (unit->getFaction()->stanceAtLeast((*iter)->getFaction(), friendlyStance)))
        {
            unitCB_->addGameItem((*iter));
        }
    }
orderWindow->addWidget(unitCB_);



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
    s <<getKeyword()<<" "<<titleTag<<" "<<locationTag<<" "<<unitTag<<endl;
    //cout<<s.str()<<endl;
    return updateOrderLine(s.str());   
}

