/* 
 * File:   GiveItemOrderWindow.cpp
 * Author: alex
 * 
 * Created on January 31, 2012, 2:30 PM
 */
#include <sstream>
#include "ClickableLabel.h"
#include "UnitEntity.h"
#include "FactionEntity.h"
#include "LocationEntity.h"
#include "GiveItemOrderDialog.h"
#include "ViewManager.h"

//
//        GIVE unit-id number item-tag [kept]
//   or   GIVE unit-id item-tag [number [kept]]
//        GET unit-id | [location-id] [number] item-tag  [left]
//
// May be invoked by right-clicking on Unit (unit to give) or Item (item to give)

OrderWindow * GiveItemOrderDialog::show(TokenEntity * token, OrderLine * order, ViewManager * view, ExtendedCommand command)
{

    GameData * data, * targetUnit, *itemToGive;
    IntegerData * amount;
    int itemNumber =0;
    targetUnit =0;



    ItemRule * activeItem = dynamic_cast<ItemRule *>(view->getActiveData());// will be 0 if unit was clicked
    targetUnit = dynamic_cast<UnitEntity *>(view->getActiveData());         // will be 0 if item was clicked

    itemToGive = activeItem;
    if(activeItem)
    {
     itemNumber = token->hasItem(activeItem);
    }

   
      OrderWindow * orderWindow = this->prepareWindow(token,order,view,"Give");
     if(orderWindow==0)
     {
         return 0;
     }
      
     
     if(!isNewOrder_)
     {
        data =  dynamic_cast<GameData *>(params[0]);
        if(data ==0)
        {
            cerr<<"ERROR: wrong 1-st parameter ["<<(params[0])->print()<<"] in order "
                    <<order->print() <<" for "<<token <<endl;  
        }        
        targetUnit = data;
        amount = dynamic_cast<IntegerData *>(params[2]);
        if(amount) // This is the 1-st form of order
        {
            itemNumber = amount->getValue();
           data =  dynamic_cast<GameData *>(params[1]);
            if(!data) 
            {
               cerr<<"ERROR: wrong 2-nd parameter ["<<(params[1])->print()<<"] in order "
                       <<order->print() <<" for "<<token <<endl;
           }
            itemToGive = data;
 
        }
        else// This is the 2-nd form of order
        {
            amount = dynamic_cast<IntegerData *>(params[1]);
            if(amount)
            {
                itemNumber = amount->getValue();
               
            }
            else
            {
                           cerr<<"ERROR: wrong 2-nd parameter ["<<(params[1])->print()<<"] in order "
                       <<order->print() <<" for "<<token <<endl; 
            } 
           data =  dynamic_cast<GameData *>(params[2]);
            if(!data) 
            {
               cerr<<"ERROR: wrong 3-rd parameter ["<<(params[1])->print()<<"] in order "
                       <<order->print() <<" for "<<token <<endl;
           }
           itemToGive = data;
        }
     }
       amountSB_ = orderWindow->createSpinBox(itemNumber,0,99); 
       itemCB_ = new OvlComboBox(itemToGive);
       targetCB_ = new OvlComboBox(targetUnit);   
     
    orderWindow->addWidget(amountSB_);

    vector <InventoryElement> inventory = token->getAllInventory();
    for (InventoryIterator iterEquip = inventory.begin();
                      iterEquip != inventory.end(); iterEquip++)
      {
//          if((*iterEquip).getItemType() != itemToGive)
//          {
          itemCB_->addGameItem((*iterEquip).getItemType());

//          }
      }
    orderWindow->addWidget(itemCB_);     
     ClickableLabel * l2 = new ClickableLabel(" to ");
     orderWindow->addWidget(l2);

    if (targetCB_)
    {
        // get list of all units here, starting from own
        vector <UnitEntity *> units = token->getLocation()->unitsPresent();
        for (vector <UnitEntity *>::iterator iter = units.begin(); iter != units.end(); ++iter)
        {
            if ((*iter)->getFaction() == token->getFaction())
            {
                targetCB_->addGameItem((*iter));
            }
        }
        //  Now Allies
        for (vector <UnitEntity *>::iterator iter = units.begin(); iter != units.end(); ++iter)
        {
            if (((*iter)->getFaction() != token->getFaction()) &&
                    (token->getFaction()->stanceAtLeast((*iter)->getFaction(), friendlyStance)))
            {
                targetCB_->addGameItem((*iter));
            }
        }
    }

  orderWindow->addWidget(targetCB_);
//::setCustomStyle(l1,ORDER_TEXT);
//::setCustomStyle(cb,ORDER_COMBO);
//::setCustomStyle(cb2,ORDER_COMBO);


////?      connect(this,SIGNAL(closeOrderWindow()),orderWindow,SLOT(closeWin()));
////        connect(orderWindow,SIGNAL(mouseOverSignal(AbstractData*)),
////                this,SLOT(mouseOverSignalHandler(AbstractData*)));
//
//      orderWindow->move(QPoint(500,800));
//      orderWindow->show();

    showWindow(orderWindow);  
    return orderWindow;
}



OrderLine * GiveItemOrderDialog::getOrderLine()
{
    stringstream s;
    string targetTag, itemTag;
    GameData * target = targetCB_->getCurrentItem();
    if(target)
    {
      targetTag = target->getTag();  
    }
    GameData * item = itemCB_->getCurrentItem();
    if(item)
    {
      itemTag = item->getTag();  
    }
    
    s <<keyword_<<" "<<targetTag <<" "<<itemTag <<" "<<amountSB_->value()<<endl;

    return updateOrderLine(s.str());  
}

//QFrame * GiveOrderDialog::showOrderBar(TokenEntity * token, OrderLine * order, ViewManager * view)
//{
//  ParameterList params;
//  ClickableLabel * l;
//        if(order)
//     {
//       params = order->getParameterList();  
//
//     }
//     else
//     {
//          return 0;
//     }
//  QFrame * frame = new  QFrame(); 
//  QHBoxLayout *   mainLayout = new QHBoxLayout;
//  frame->setLayout(mainLayout);
//  ClickableLabel * l1 = new ClickableLabel("Give ");
//  mainLayout->addWidget(l1);
//
//  for(ParameterList::iterator iter = params.begin(); iter != params.end(); ++iter)
//  {
//     l = new ClickableLabel((*iter)->print());
//      mainLayout->addWidget(l);
//  }
// 
//  return frame;
//}
