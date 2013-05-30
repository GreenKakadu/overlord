/* 
 * File:   UnitOrderDialog.cpp
 * Author: alex
 * 
 * Created on March 29, 2012, 2:08 PM
 */
#include <sstream>
#include "GameFacade.h"
#include "ViewManager.h"
#include "UnitEntity.h"
//#include "FactionEntity.h"
#include "LocationEntity.h"
#include "UnitOrderDialog.h"
//
//        ORDER  ACCEPT  unit-id 
//   
//


 
OrderWindow * UnitOrderDialog::show(TokenEntity * token, OrderLine * order, ViewManager * view, ExtendedCommand command)
{
    GameData * data =0;
    GameData * unit=0;
    string commandName;
    unit = view->getActiveData();
        if(order)
    {
        if(order->getOrderPrototype()->getKeyword() == "accept")
        {
          command = ExtendedCommand::ACCEPT;
        }
         if(order->getOrderPrototype()->getKeyword() == "eject")
        {
          command = ExtendedCommand::EJECT;
        }
        if(order->getOrderPrototype()->getKeyword() == "stack")
        {
          command = ExtendedCommand::STACK;
        }
        if(order->getOrderPrototype()->getKeyword() == "promote")
        {
          command = ExtendedCommand::PROMOTE;
        }
        if(order->getOrderPrototype()->getKeyword() == "exit")
       {
         command = ExtendedCommand::EXIT;
       }
     }
 
    switch(command)
    {
        case ExtendedCommand::ACCEPT :
        {
           commandName = "Accept";
           break;
        }
        case ExtendedCommand::EJECT :
        {
           commandName = "Eject"; 
           break;
        }
        case ExtendedCommand::STACK :
        {
           commandName = "Stack"; 
            break;
       }
         case ExtendedCommand::PROMOTE :
        {
           commandName = "Promote"; 
            break;
       }
    case ExtendedCommand::EXIT :
   {
      commandName = "Exit";
       break;
  }
   }
    
     OrderWindow * orderWindow = this->prepareWindow(token,order,view,commandName);
     if(orderWindow==0)
     {
         return 0;
     }
     if(command == ExtendedCommand::EXIT)
     {
         showWindow(orderWindow);
         return orderWindow;
     }
     UnitEntity * currentUnit = dynamic_cast<UnitEntity *>(token);
     if(currentUnit==0)
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
            unit = data;       
     }
 


     

       unitCB_ = new OvlComboBox(unit);
       

  // Add items to ComboBoxes and add widgets     
    orderWindow->addWidget(unitCB_); 
   vector <UnitEntity *> units = token->getLocation()->unitsPresent();
   vector <UnitEntity *>::iterator iter;

    switch(command)
    {
        case ExtendedCommand::ACCEPT :
        {
           // foreign units

        for (iter = units.begin(); iter != units.end(); ++iter)
        {
            if ((*iter)->getFaction() != token->getFaction())
            {
                unitCB_->addGameItem((*iter));
            }
        } 
        break;
        }
        case ExtendedCommand::EJECT :
        case ExtendedCommand::PROMOTE :
        {
            //  unit in stack under you
        for (iter = units.begin(); iter != units.end(); ++iter)
        {
            if ((*iter)->isFollowingInStackTo(currentUnit))
            {
                unitCB_->addGameItem((*iter));
            }
        }          
           break;
        }
        case ExtendedCommand::STACK :
        {
            //first your's
        for (iter = units.begin(); iter != units.end(); ++iter)
        {
            if (((*iter)->getFaction() == token->getFaction()) /*&& ((*iter)->getPrevStack() !=currentUnit)*/)
            {
                unitCB_->addGameItem((*iter));
            }
        } 
            //Than all other's
        for (iter = units.begin(); iter != units.end(); ++iter)
        {
            if ((*iter)->getFaction() != token->getFaction())
            {
                unitCB_->addGameItem((*iter));
            }
        } 
           break;
       }
    }

 

 
      showWindow(orderWindow);
      return orderWindow;
}



OrderLine * UnitOrderDialog::getOrderLine()
{
    stringstream s;
    string unitTag;
    if(unitCB_)
    {
    GameData * unit = unitCB_->getCurrentItem();
    if(unit)
    {
      unitTag = unit->getTag();  
    }
    }
    s <<keyword_ <<" "<<unitTag <<endl;
    return updateOrderLine(s.str());   
}

