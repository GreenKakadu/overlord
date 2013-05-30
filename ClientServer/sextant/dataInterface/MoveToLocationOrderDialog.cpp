/* 
 * File:   MoveToLocationOrderDialog.cpp
 * Author: alex
 * 
 * Created on March 25, 2012
 */
#include <sstream>
#include "ViewManager.h"
#include "MoveToLocationOrderDialog.h"
#include "UnitEntity.h"
#include "DirectionVariety.h"
#include "FactionEntity.h"
#include "LocationEntity.h"
#include "BasicExit.h"
//
//        ORDER  MOVE direction|location-id
//               MARCH [direction|location-id]
// 
OrderWindow * MoveToLocationOrderDialog::show(TokenEntity * token, OrderLine * order, ViewManager * view, ExtendedCommand command)
{
    GameData * data =0;
    GameData * destination=0;
    LocationEntity * location;
    OrderWindow * orderWindow = 0;

   destination  = view->getSelectedLocation();    
   if(command == ExtendedCommand::MARCH)
   {
      orderWindow = this->prepareWindow(token,order,view,"March");
   }
   else
   {
     orderWindow = this->prepareWindow(token,order,view,"Move");
   }
     if(orderWindow==0)
     {
         return 0;
     }
    
     
// for existing order determine parameters      
     if(!isNewOrder_ && params.size() >= 1)
     {
         data = dynamic_cast<GameData *>(params[0]); // Location or destination
        if(data ==0)
        {
            cerr<<"ERROR: wrong 1-st parameter ["<<(params[0])->print()<<"] in order "
                    <<order->print() <<" for "<<token <<endl;  
        }
        else
        {
            location = dynamic_cast<LocationEntity *>(data);
            if(location)
            {
               destination = location; 
            }
            else
            {
            DirectionVariety * direction =   dynamic_cast< DirectionVariety*>(data);
      if( direction != 0)
        {
        destination = direction;
        }
      else
      {
                    cerr<<"ERROR: wrong 1-st parameter ["<<(params[0])->print()<<"] in order "
                    <<order->print() <<" for "<<token <<endl;  
      }
            }
                
        }      
        
     }
 
       destinationCB_ = new OvlComboBox(destination);

       

  // Add items to ComboBoxes and add widgets     
    orderWindow->addWidget(destinationCB_); 
    location = token->getLocation();
    if(location)
    {
    for (vector<BasicExit *>::iterator iter = location->getAllExits().begin();
    iter != location->getAllExits().end(); ++iter)
    {
        destinationCB_->addGameItem ((*iter)->getDestination(),true);
    }
    for(int i=0; i < gameFacade->directions.size(); ++i)
    {
       destinationCB_->addGameItem (gameFacade->directions[i]); 
    }
    
    }

      showWindow(orderWindow);
      return orderWindow;
}



OrderLine * MoveToLocationOrderDialog::getOrderLine()
{
    stringstream s;
    string destinationTag;
    
    GameData * destination = destinationCB_->getCurrentItem();
    if(destination)
    {
      destinationTag = destination->getTag();  
    }    
    s <<keyword_<<" "<<destinationTag <<endl;
    return updateOrderLine(s.str());   
}

