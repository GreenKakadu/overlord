/* 
 * File:   EnterConstructionOrderDialog.cpp
 * Author: alex
 * 
 * Created on May 20, 2013, 10:05 PM
 */

#include <sstream>
#include "ConstructionEntity.h"
#include "LocationEntity.h"
#include "ViewManager.h"
#include "EnterConstructionOrderDialog.h"

//
//        ORDER ENTER structure-id 
//   
// 
OrderWindow * EnterConstructionOrderDialog::show(TokenEntity * token, OrderLine * order, ViewManager * view, ExtendedCommand extendedCommandFlag)
{
    ConstructionEntity * data =0;
    OrderWindow * orderWindow;  
    orderWindow = this->prepareWindow(token,order,view,"Enter");
      
     if(orderWindow==0)
     {
         return 0;
     }
    
     
// for existing order determine parameters      
     if(!isNewOrder_ && params.size() >= 1)
     {        
        data = dynamic_cast<ConstructionEntity *>(params[0]);
            if(!data) 
            {
                    cerr<<"ERROR: wrong 1-st parameter ["<<(params[0])->print()<<"] in order "
                    <<order->print() <<" for "<<token <<endl;               
            }
     }
      
       constructionCB_ = new OvlComboBox(data);
       orderWindow->addWidget(constructionCB_);
// Buildings
vector <ConstructionEntity *> constructions = token->getLocation()->constructionsPresent();
for (vector <ConstructionEntity *>::iterator iter = constructions.begin(); iter != constructions.end(); ++iter)
    {
//    if(faction->getStance((*iter))== stance)
        {
            constructionCB_->addGameItem((*iter));
        }
    }

      showWindow(orderWindow);
      return orderWindow;
}


OrderLine * EnterConstructionOrderDialog::getOrderLine()
{
    stringstream s;
    string constructionTag;
    
    GameData * construction = constructionCB_->getCurrentItem();
    if(construction)
    {
      constructionTag = construction->getTag();  
    }  
 //   cout<<"EnterConstructionOrderDialog::getOrderLine["<<keyword_ << "]"<<endl;
    s <<keyword_ <<" "<<constructionTag <<endl;
     cout<<"EnterConstructionOrderDialog::getOrderLine["<<s.str() << "]"<<endl;
   return updateOrderLine(s.str());   
}

