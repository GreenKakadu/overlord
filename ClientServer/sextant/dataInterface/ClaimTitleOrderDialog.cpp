/* 
 * File:   ClaimTitleOrderDialog.cpp
 * Author: alex
 * 
 * Created on April 23, 2012, 11:10 AM
 */
#include <sstream>
#include "ClaimTitleOrderDialog.h"
#include "TitleRule.h"
#include "ViewManager.h"
#include "LocationEntity.h"
//
//        ORDER  CLAIM title [FREE] 
//   
// 
OrderWindow * ClaimTitleOrderDialog::show(TokenEntity * token, 
     OrderLine * order, ViewManager * view, ExtendedCommand extendedCommandFlag)
{
    TitleRule * data=0;
    StringData * s;
    bool isFree = false;
    data = dynamic_cast<TitleRule *>(view->getActiveData());
    
    
     OrderWindow * orderWindow = this->prepareWindow(token,order,view,"Claim");
     if(orderWindow==0)
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
                            
        if(params.size() >= 2)
        {            
            s = dynamic_cast<StringData *>(params[1]);
            if(!s) 
            {
                    cerr<<"ERROR: wrong 2-nd parameter ["<<(params[1])->print()<<"] in order "
                    <<order->print() <<" for "<<token <<endl;               
            }
            if(s->print()=="FREE")
            {
              isFree = true;  
            }
        }

       
        
     }
     

       CB_ = new OvlComboBox(data);
       freeButton_ = new QCheckBox("claim title only if it is free");
       if(isFree)
       {
          freeButton_->setChecked(isFree);
       }

  // Add items to ComboBoxes and add widgets     
    orderWindow->addWidget(CB_); 
  vector < TitleElement *> * titles =   token->getLocation()->getTitles().getAll();
    for( vector < TitleElement *>::iterator iter =  titles->begin(); iter != titles->end(); ++iter)
    {
        if((*iter)->getTitle() != data) // Already added
        {
           CB_->addGameItem ((*iter)->getTitle());
        }
    }
    

    orderWindow->addWidget(CB_);   
    orderWindow->addWidget(freeButton_);




      showWindow(orderWindow);
      return orderWindow;
}



OrderLine * ClaimTitleOrderDialog::getOrderLine()
{
    stringstream s;
    string itemTag;
    
    GameData * item = CB_->getCurrentItem();
    if(item)
    {
      itemTag = item->getTag();  
    }    
    s <<keyword_<<" "<<itemTag;
    if(freeButton_->isChecked())
    {
        s<<" FREE";
    }
    
    s<<endl;
    return updateOrderLine(s.str());   
}

