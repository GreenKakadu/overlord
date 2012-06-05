/* 
 * File:   OathOrderDialog.cpp
 * Author: alex
 * 
 * Created on March 25, 2012
 */
#include <sstream>
#include "GameFacade.h"
#include "ViewManager.h"
#include "OathOrderDialog.h"
#include "UnitEntity.h"
#include "FactionEntity.h"
#include "LocationEntity.h"
//
//        ORDER  OATH  unit-id | faction-id
//   
// 
OrderWindow * OathOrderDialog::show(TokenEntity * token, OrderLine * order, ViewManager * view, ExtendedCommand command)
{
    GameData * data =0;
    GameData * oathTarget=0;

    oathTarget = view->getSelectedToken();
    
    
     OrderWindow * orderWindow = this->prepareWindow(token,order,view,"Oath");
     if(orderWindow==0)
     {
         return 0;
     }
    
     
// for existing order determine parameters      
     if(!isNewOrder_ && params.size() >= 1)
     {
            data = dynamic_cast<GameData *>(params[0]);
            if(!data) 
            {
                    cerr<<"ERROR: wrong 2-nd parameter ["<<(params[0])->print()<<"] in order "
                    <<order->print() <<" for "<<token <<endl;               
            }
            oathTarget = data;       
     }
 


     

       oathTargetCB_ = new OvlComboBox(oathTarget);
       

  // Add items to ComboBoxes and add widgets     
    orderWindow->addWidget(oathTargetCB_); 

     for (int i = 0; i < gameFacade->factions.size(); ++i)
     {
        if(gameFacade->factions[i] != 0 && gameFacade->factions[i] != token->getFaction())
        {
          oathTargetCB_->addGameItem(gameFacade->factions[i]);  
        }
     }

 
     vector <UnitEntity *> units = token->getLocation()->unitsPresent();
    for (vector <UnitEntity *>::iterator iter = units.begin(); iter != units.end(); ++iter)
        {
            if (((*iter)->getFaction() != token->getFaction()))
            {
                oathTargetCB_->addGameItem((*iter));
            }
        }
 
      showWindow(orderWindow);
      return orderWindow;
}



OrderLine * OathOrderDialog::getOrderLine()
{
    stringstream s;
    string oathTargetTag;
    
    GameData * oathTarget = oathTargetCB_->getCurrentItem();
    if(oathTarget)
    {
      oathTargetTag = oathTarget->getTag();  
    }    
    s <<keyword_ <<" "<<oathTargetTag <<endl;
    return updateOrderLine(s.str());   
}

