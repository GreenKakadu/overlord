/* 
 * File:   TargetOrderDialog.cpp
 * Author: alex
 * 
 * Created on May 22, 2013, 10:20 AM
 */

#include "TargetOrderDialog.h"
#include <sstream>
#include "ConstructionEntity.h"
#include "LocationEntity.h"
#include "ViewManager.h"
#include "UnitEntity.h"
#include "GameData.h"

//
//        ORDER TARGET unit-id|location-id|structure-id|structure-tag
//   
// 
OrderWindow * TargetOrderDialog::show(TokenEntity * token, OrderLine * order, ViewManager * view, ExtendedCommand extendedCommandFlag)
{
    GameData * data =0;
    OrderWindow * orderWindow;  
    GameData * activeData =view->getActiveData();
   orderWindow = this->prepareWindow(token,order,view,"Target");
      
     if(orderWindow==0)
     {
         return 0;
     }
  
     data = dynamic_cast<UnitEntity *>(activeData);
     if(!data)
     {
       data = dynamic_cast<LocationEntity *>(activeData);
       if(!data)
       {
        data = dynamic_cast<ConstructionEntity *>(activeData);
         if(!data)
         {
           data = dynamic_cast<ConstructionRule *>(activeData);
             if(!data)
             {
                  cerr <<"ERROR: wrong target ["<<activeData->print()<<"] in order "
                     <<order->print() <<" for "<<token <<endl;
             }
         }
       }
       
     }

   
   
   
   
     
// for existing order determine parameters      
    if (!isNewOrder_ && params.size() >= 1)
    {

       data = dynamic_cast<UnitEntity *> (params[0]);
        if (!data)
        {
            data = dynamic_cast<LocationEntity *> (params[0]);
            if (!data)
            {
                data = dynamic_cast<ConstructionEntity *> (params[0]);
                if (!data)
                {
                 data = dynamic_cast<ConstructionRule *> (params[0]);
                  if (!data)
                  {
                      StringData * sdata = dynamic_cast<StringData *> (params[0]);
                        if (!sdata)
                        {
                           cerr << "ERROR: wrong 1-st parameter [" << (params[0])->print() << "] in order "
                                    << order->print() << " for " << token->getTag() << endl;
                        }
                        else
                        {
                            data =gameFacade->getDataManipulator()->findGameData(sdata->print());
                            //if(data) cout <<data->print()<<endl;
                        }
                  }
                }
            }
        }
    }
                    activeData = data;  
       targetCB_ = new OvlComboBox(activeData);
       orderWindow->addWidget(targetCB_);

       // Do not add targets (too many))

      showWindow(orderWindow);
      return orderWindow;
}

OrderLine * TargetOrderDialog::getOrderLine()
{
    stringstream s;
    string targetTag;
    
    GameData * target = targetCB_->getCurrentItem();
    if(target)
    {
      targetTag = target->getTag();  
    }    
    s <<keyword_ <<" "<<targetTag <<endl;
    return updateOrderLine(s.str());   
}


