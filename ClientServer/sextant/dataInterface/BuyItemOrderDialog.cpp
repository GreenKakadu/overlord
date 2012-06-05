/* 
 * File:   BuyItemOrderDialog.cpp
 * Author: alex
 * 
 * Created on March 25, 2012
 */
#include <sstream>
#include "ViewManager.h"
#include "ClickableLabel.h"
#include "BuyItemOrderDialog.h"
#include "UnitEntity.h"
#include "FactionEntity.h"
#include "LocationEntity.h"
#include "MarketStrategy.h"
#include "MarketRequest.h"
#include "LocalMarketRequest.h"
//
//        ORDER BUY  number item-tag [price]
//        ORDER SELL number item-tag [price] 
//   
// 
OrderWindow * BuyItemOrderDialog::show(TokenEntity * token, OrderLine * order, ViewManager * view, ExtendedCommand extendedCommandFlag)
{
    ParameterList params;
    GameData * data =0;
    IntegerData * num =0;
    GameData * itemToBuy;
    int price =0;
    int amountToBuy =0;
    itemToBuy= /*view->getSelectedItem();*/view->getActiveData();
    string label;
    
    if( order)
    {
        if(order->getOrderPrototype()->getKeyword() == "sell")
        {
          extendedCommandFlag = ExtendedCommand::SELL;
         // label = "Sell";
        }
         if(order->getOrderPrototype()->getKeyword() == "buy")
        {
          extendedCommandFlag = ExtendedCommand::BUY;
         // label = "Buy"; 
        }
   }
    
    
    if(extendedCommandFlag == ExtendedCommand::SELL)
    {
      label = "Sell";  
    }
    else
    {
      label = "Buy";   
    }
    
     
     OrderWindow * orderWindow = this->prepareWindow(token,order,view,label);
     if(orderWindow==0)
     {
         return 0;
     }
     
     if(!isNewOrder_ && params.size() >= 1)
     {
        num =  dynamic_cast<IntegerData *>(params[0]);
        if(num ==0)
        {
            cerr<<"ERROR: wrong 1-st parameter ["<<(params[0])->print()<<"] in order "
                    <<order->print() <<" for "<<token <<endl;  
        }
        else
        {
                amountToBuy = num->getValue();
        }

        
        if(params.size() >= 2)
        {            
            data = dynamic_cast<GameData *>(params[1]);
            if(!data) 
            {
                    cerr<<"ERROR: wrong 2-nd parameter ["<<(params[1])->print()<<"] in order "
                    <<order->print() <<" for "<<token <<endl;               
            }
            itemToBuy = data;
        }
         if(params.size() >= 3)
        {  
             num =  dynamic_cast<IntegerData *>(params[2]);
            if(!num) 
            {
                    cerr<<"ERROR: wrong 3-rd parameter ["<<(params[2])->print()<<"] in order "
                    <<order->print() <<" for "<<token <<endl;               
            }
            else
            {
                    price = num->getValue();
            }

        }
       
        
     }
     
     
       amountSB_ = orderWindow->createSpinBox(amountToBuy,0,99); 
       itemCB_ = new OvlComboBox(itemToBuy);
       priceSB_ = orderWindow->createSpinBox(price,0,99); 

    orderWindow->addWidget(amountSB_);   
    orderWindow->addWidget(itemCB_);

    if (extendedCommandFlag == ExtendedCommand::SELL) 
    {
        // add from market
        if (view->getSelectedLocation())
        {
            if (view->getSelectedLocation()->getMarket())
            {
                vector <LocalMarketRequest *> buyList = view->getSelectedLocation()
                        ->getMarket()->getAllBuyRequests();
                for (vector <LocalMarketRequest *>::iterator iter = buyList.begin();
                        iter != buyList.end(); ++iter)
                {
                    itemCB_->addGameItem(dynamic_cast<GameData *> ((*iter)->getType()));
                }
            }
        }


    }
    else    // Buy
    {
      // add from market
        if (view->getSelectedLocation())
        {
            if (view->getSelectedLocation()->getMarket())
            {
                vector <LocalMarketRequest *> sellList = view->getSelectedLocation()
                        ->getMarket()->getAllSellRequests();
                for (vector <LocalMarketRequest *>::iterator iter = sellList.begin();
                        iter != sellList.end(); ++iter)
                {
                    itemCB_->addGameItem(dynamic_cast<GameData *> ((*iter)->getType()));
                }
            }
        }
    }

    
     ClickableLabel * l2 = new ClickableLabel(" for ");
     orderWindow->addWidget(l2);
   
    orderWindow->addWidget(priceSB_);

     ClickableLabel * l3 = new ClickableLabel(" coins.");
     orderWindow->addWidget(l3);


      showWindow(orderWindow);
      return orderWindow;
}



OrderLine * BuyItemOrderDialog::getOrderLine()
{
    stringstream s;
    string itemTag;
    
    GameData * item = itemCB_->getCurrentItem();
    if(item)
    {
      itemTag = item->getTag();  
    }    
    s <<keyword_<<" "<<amountSB_->value()  <<" "<<itemTag <<" "<<priceSB_->value()<<endl;
    return updateOrderLine(s.str());  
}
