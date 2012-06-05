/* 
 * File:   EquipItemOrderDialog.cpp
 * Author: alex
 * 
 * Created on March 25, 2012
 */
#include <sstream>
#include "UnitEntity.h"
#include "FactionEntity.h"
#include "LocationEntity.h"
#include "ViewManager.h"
#include "EquipItemOrderDialog.h"
//
//        ORDER EQUIP item-tag [number]
//   
// 
OrderWindow * EquipItemOrderDialog::show(TokenEntity * token, OrderLine * order, ViewManager * view, ExtendedCommand extendedCommandFlag)
{
    GameData * data =0;
    GameData * item=0;
    IntegerData * num;
    int counter;
    OrderWindow * orderWindow;
    item = view->getActiveData();
    ItemRule * activeItem = dynamic_cast<ItemRule *>(item);
    if(extendedCommandFlag == ExtendedCommand::EQUIP) // Equip
    {
    if(activeItem)
    {
    counter = token->hasItem(activeItem);
    }
    else
    {
      counter = 0;  
    }
     orderWindow = this->prepareWindow(token,order,view,"Equip");
    }
    else // UNEQUIP
    {
       counter = 0;  
     orderWindow = this->prepareWindow(token,order,view,"Unequip");
    }
    
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
                    cerr<<"ERROR: wrong 1-st parameter ["<<(params[0])->print()<<"] in order "
                    <<order->print() <<" for "<<token <<endl;               
            }
            item = data;
        
        
        
        
         if(params.size() >= 2)
        {  
             num =  dynamic_cast<IntegerData *>(params[1]);
            if(!num) 
            {
                    cerr<<"ERROR: wrong 2-nd parameter ["<<(params[1])->print()<<"] in order "
                    <<order->print() <<" for "<<token <<endl;               
            }
            else
            {
                    counter = num->getValue();
            }

        }
       
        
     }
 


     
      
       itemCB_ = new OvlComboBox(item);
       amountSB_ = orderWindow->createSpinBox(counter,0,99);    

  // Add items to ComboBoxes and add widgets     
    orderWindow->addWidget(itemCB_); 
    vector <InventoryElement> inventory = token->getAllInventory();
    for (InventoryIterator iterEquip = inventory.begin();
                      iterEquip != inventory.end(); iterEquip++)
      {
          itemCB_->addGameItem((*iterEquip).getItemType());
      }    
    
    

    

 
    orderWindow->addWidget(amountSB_);




      showWindow(orderWindow);
      return orderWindow;
}



OrderLine * EquipItemOrderDialog::getOrderLine()
{
    stringstream s;
    string itemTag;
    
    GameData * item = itemCB_->getCurrentItem();
    if(item)
    {
      itemTag = item->getTag();  
    }    
    s <<keyword_ <<" "<<itemTag <<" "<<amountSB_->value()<<endl;
    return updateOrderLine(s.str());   
}

