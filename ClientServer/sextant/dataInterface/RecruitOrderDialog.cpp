
#include "RecruitOrderDialog.h"
/*
 * File:   RecruitOrderDialog.cpp
 * Author: Alex
 *
 * Created on RecruitOrderDialog
 */
#include <sstream>
#include "ClickableLabel.h"
#include "UnitEntity.h"
#include "FactionEntity.h"
#include "LocationEntity.h"
#include "ViewManager.h"
#include "MarketStrategy.h"
#include "LocalMarketRequest.h"

//
//     RECRUIT unit-id number race price-per-figure
//
// May be invoked by right-clicking on race
int RecruitOrderDialog::newUnitPlaceholderCounter =0;

RecruitOrderDialog::RecruitOrderDialog(string keyword) : OrderDialog(keyword)
{
    //newUnitPlaceholder_=gameFacade->getGameConfig()->getNewEntityPrefix();
    newUnitPlaceholderCounter_ = RecruitOrderDialog::newUnitPlaceholderCounter;
//    cout<< "RecruitOrderDialog::keyword_ is "<<keyword_<<endl;
}



OrderWindow * RecruitOrderDialog::show(TokenEntity * token, OrderLine * order, ViewManager * view, ExtendedCommand command)
{
    GameData * data =0;
    IntegerData * num =0;
    GameData * raceToRecruit;
    int price =0;
    int numToRecruit =0;
    int newUnitId;
    int maxRecruitNum=999;
    raceToRecruit= view->getActiveData();
    string label;
    StringData * str=0;
    LocationEntity * location = view->getSelectedLocation();

    FactionEntity * faction = token->getFaction();
    if(faction)
    {
        newUnitPlaceholder_=faction->getTag() + gameFacade->getGameConfig()->getNewEntityPrefix() + "U";
    }

     OrderWindow * orderWindow = this->prepareWindow(token,order,view,label);
     if(orderWindow==0)
     {
         return 0;
     }


     if(!isNewOrder_ && params.size() >= 2)
     {
         NewEntityPlaceholder * placeholder = dynamic_cast<NewEntityPlaceholder *>(params[0]);
         if(placeholder==0)
         {
             cerr<<"ERROR: wrong 1-st parameter ["<<(params[0])->print()<<"] in order "
                     <<order->print() <<" for "<<token <<endl;
         }
         else
         {

            newUnitPlaceholder_ = placeholder->print();
            if(!gameFacade->getGameConfig()->isNewEntityName(newUnitPlaceholder_,faction))
            { cerr<<"ERROR:  1-st parameter ["<<(params[0])->print()<<"] in order "
                 <<order->print() <<" for "<<token <<" is not a placeholder"<<endl;
            }
         }
        num =  dynamic_cast<IntegerData *>(params[1]);
        if(num ==0)
        {
            cerr<<"ERROR: wrong 2-nd parameter ["<<(params[1])->print()<<"] in order "
                    <<order->print() <<" for "<<token <<endl;
        }
        else
        {
                numToRecruit = num->getValue();
        }

            data = dynamic_cast<GameData *>(params[2]);
            if(!data)
            {
                    cerr<<"ERROR: wrong 3-rd parameter ["<<(params[2])->print()<<"] in order "
                    <<order->print() <<" for "<<token <<endl;
            }
            raceToRecruit = data;

         if(params.size() >= 3)
        {
             num =  dynamic_cast<IntegerData *>(params[3]);
            if(!num)
            {
                    cerr<<"ERROR: wrong 4-th parameter ["<<(params[3])->print()<<"] in order "
                    <<order->print() <<" for "<<token <<endl;
            }
            else
            {
                    price = num->getValue();
            }

        }


     }
     if(isNewOrder_) // New order
     {
       RecruitOrderDialog::newUnitPlaceholderCounter++;
     }

     ClickableLabel * l1 = new ClickableLabel(newUnitPlaceholder_);

     orderWindow->addWidget(l1);
       // get newUnitId from newUnitPlaceholder_
       newUnitId = gameFacade->getGameConfig()->getNewEntitySuffix(newUnitPlaceholder_);
             // cout<<"newUnitPlaceholder is " << newUnitPlaceholder_<<" newUnitId = "<<newUnitId<<endl;
       if(newUnitId==0)
       {
        newUnitId =  RecruitOrderDialog::newUnitPlaceholderCounter;
         //cout<<" newUnitId is set to "<<newUnitId<<endl;
       }

       // Find price and max amout to hire
       if(location)
       {
           if (location->getMarket())
           {
               vector <LocalMarketRequest *> sellList = location
                       ->getMarket()->getAllRecruitRequests();
               for (vector <LocalMarketRequest *>::iterator iter = sellList.begin();
                       iter != sellList.end(); ++iter)
               {
                   if((*iter)->getType()==raceToRecruit)
                   {
                       //cout<<"LocalMarketRequest for "<<raceToRecruit->print() <<" is found:"<< (*iter)->print()<<" price "<< (*iter)->getPrice()<<"("<< price <<")"<<" num "<<(*iter)->getAmount()<<endl;
                       if(price==0)
                       {
                           price = (*iter)->getPrice();
                       }
                       maxRecruitNum = (*iter)->getAmount();
                   }
               }
           }
       }

       newUnitCB_= orderWindow->createSpinBox(newUnitId,0/*RecruitOrderDialog::newUnitPlaceholderCounter*/,99);
       amountSB_ = orderWindow->createSpinBox(numToRecruit,1,maxRecruitNum);
       raceCB_ = new OvlComboBox(raceToRecruit);
       priceSB_ = orderWindow->createSpinBox(price,0,999);


    orderWindow->addWidget(newUnitCB_);
    orderWindow->addWidget(amountSB_);
    orderWindow->addWidget(raceCB_);



      // add recruits
        if (location)
        {
            if (location->getMarket())
            {
                vector <LocalMarketRequest *> sellList = location
                        ->getMarket()->getAllRecruitRequests();
                for (vector <LocalMarketRequest *>::iterator iter = sellList.begin();
                        iter != sellList.end(); ++iter)
                {
                    raceCB_->addGameItem(dynamic_cast<GameData *> ((*iter)->getType()));
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



OrderLine * RecruitOrderDialog::getOrderLine()
{
    stringstream s;
    string raceTag;
    GameData * race = raceCB_->getCurrentItem();
    if(race)
    {
      raceTag = race->getTag();
    }

    if(RecruitOrderDialog::newUnitPlaceholderCounter < newUnitPlaceholderCounter_)
    {
        RecruitOrderDialog::newUnitPlaceholderCounter = newUnitPlaceholderCounter_;
    }
    if(RecruitOrderDialog::newUnitPlaceholderCounter < newUnitCB_->value())
    {
        RecruitOrderDialog::newUnitPlaceholderCounter = newUnitCB_->value();
    }

    gameFacade->getGameConfig()->setNewEntitySuffix(newUnitPlaceholder_,newUnitCB_->value());
    s <<keyword_<<" "<< newUnitPlaceholder_ <<" "<<amountSB_->value()<<" "<<raceTag <<" "<<priceSB_->value()<<endl;

    return updateOrderLine(s.str());
}


