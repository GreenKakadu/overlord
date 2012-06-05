/* 
 * File:   ReleaseTitleOrderDialog.cpp
 * Author: alex
 * 
 * Created on May 1, 2012, 11:48 AM
 */
#include <sstream>
#include "ReleaseTitleOrderDialog.h"
#include "TitleRule.h"
#include "ViewManager.h"
#include "LocationEntity.h"
#include "UnitEntity.h"
// YIELD title-tag location-id 
// CANCEL title-tag location-id 
OrderWindow * ReleaseTitleOrderDialog::show(TokenEntity * token, 
     OrderLine * order, ViewManager * view, ExtendedCommand extendedCommandFlag)
{
    TitleRule * data=0;

    data = dynamic_cast<TitleRule *>(view->getActiveData());
    
    
     OrderWindow * orderWindow = this->prepareWindow(token,order,view,"Release");
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
     if(!isNewOrder_ && params.size() >= 1)
     {
            data = dynamic_cast<TitleRule *>(params[0]);
            if(!data) 
            {
                    cerr<<"ERROR: wrong  parameter ["<<(params[0])->print()<<"] in order "
                    <<order->print() <<" for "<<token <<endl;               
            }        
     }
     

       CB_ = new OvlComboBox(data);


  // Add items to ComboBoxes and add widgets     
    orderWindow->addWidget(CB_); 
    vector < TitleElement *> * titles =   unit->getTitlesAttribute()->getAll();
    for( vector < TitleElement *>::iterator iter =  titles->begin(); iter != titles->end(); ++iter)
    {
        if((*iter)->getTitle() != data) // Already added
        {
           CB_->addGameItem ((*iter)->getTitle());
        }
    }
    

    orderWindow->addWidget(CB_);   





      showWindow(orderWindow);
      return orderWindow;
}



OrderLine * ReleaseTitleOrderDialog::getOrderLine()
{
    stringstream s;
    string itemTag;
    
    GameData * item = CB_->getCurrentItem();
    if(item)
    {
      itemTag = item->getTag();  
    }    
    s <<keyword_<<" "<<itemTag<<endl;
    
    return updateOrderLine(s.str());   
}

