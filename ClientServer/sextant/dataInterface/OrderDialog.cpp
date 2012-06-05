/* 
 * File:   OrderDialog.cpp
 * Author: alex
 * 
 * Created on January 25, 2012, 7:32 PM
 */
#include "TokenEntity.h"
#include "ViewManager.h"
#include "ClickableLabel.h"
#include "OrderDialog.h"
#include "OrderPrototypesCollection.h"
#include "GiveItemOrderDialog.h"
#include "UseSkillOrderDialog.h"
#include "BuyItemOrderDialog.h"
#include "OathOrderDialog.h"
#include "UnitOrderDialog.h"
#include "MoveToLocationOrderDialog.h"
#include "ClaimTitleOrderDialog.h"
#include "ReleaseTitleOrderDialog.h"
#include "BestowTitleOrderDialog.h"
#include "EquipItemOrderDialog.h"
extern OrderPrototypesCollection  * orderPrototypesCollection;
vector<OrderDialog *> OrderDialog::orderDialogRegister;
// all specific orders instantiations should be here  after orderDialogRegister
GiveItemOrderDialog sampleGiveItemOrderWindow("give");
UseSkillOrderDialog sampleUseSkillOrderWindow("use");
BuyItemOrderDialog sampleBuyItemOrderWindow("buy");
OathOrderDialog sampleOathOrderWindow("oath");
MoveToLocationOrderDialog sampleMoveToLocationOrderWindow("move");
// for future use// UseItemOrderDialog sampleUseItemOrderDialog("use");

 BuyItemOrderDialog sampleSellItemOrderDialog("sell");
 EquipItemOrderDialog sampleEquipItemOrderDialog("equip");

 
// GetItemOrderDialog sampleGetItemOrderDialog("get");
// SwapItemOrderDialog sampleSwapItemOrderDialog("swap");       
// LearnSkillOrderDialog sampleLearnSkillOrderDialog("study");        
// TeachSkillOrderDialog sampleTeachSkillOrderDialog("teach");        
UseSkillOrderDialog sampleForgetSkillOrderWindow("forget");
      
// RecruitOrderDialog  sampleRecruitOrderDialog("recruit");           
 ClaimTitleOrderDialog sampleClaimTitleOrderDialog("claim");       
 ReleaseTitleOrderDialog sampleReleaseTitleOrderDialog("yield");       
 BestowTitleOrderDialog sampleBestowTitleOrderDialog("bestow");       
// RevokeTitleOrderDialog sampleRevokeTitleOrderDialog("cancel");        
UnitOrderDialog sampleAcceptUnitOrderDialog("accept");       
// AttackUnitOrderDialog sampleAttackUnitOrderDialog("attack");       
// BestowUnitOrderDialog sampleBestowUnitOrderDialog("bestow");       
UnitOrderDialog sampleEjectUnitOrderDialog("eject");       
// GetFromUnitOrderDialog sampleGetFromUnitOrderDialog("get");       
// GiveToUnitOrderDialog sampleGiveToUnitOrderDialog("give");              
UnitOrderDialog samplePromoteUnitOrderDialog("promote");       
UnitOrderDialog sampleStackToUnitOrderDialog("stack");       
// StanceToUnitOrderDialog sampleStanceToUnitOrderDialog("stance");       
// SwapWithUnitOrderDialog sampleSwapWithUnitOrderDialog("swap");       
// TeachUnitOrderDialog sampleTeachUnitOrderDialog("teach");       
// TargetUnitOrderDialog sampleTargetUnitOrderDialog("target");             
// AttackConstructionOrderDialog sampleAttackConstructionOrderDialog("attack");       
// EnterConstructionOrderDialog sampleEnterConstructionOrderDialog("enter");       
// ExitConstructionOrderDialog sampleExitConstructionOrderDialog("exit");       
// TargetConstructionOrderDialog sampleTargetConstructionOrderDialog("target");       
// BuildConstructionOrderDialog sampleBuildConstructionOrderDialog("build");            
// AttackLocationOrderDialog sampleAttackLocationOrderDialog("attack");             
MoveToLocationOrderDialog sampleMarchToLocationOrderWindow("march");      
// CaravanToLocationOrderDialog  sampleCaravanToLocationOrderDialog("caravan");       
// TargetLocationOrderDialog sampleTargetLocationOrderDialog("target");           
// StanceOrderDialog sampleStanceOrderDialog("stance");       


//OrderDialog::OrderDialog() {
//    
//}



OrderDialog::OrderDialog(string keyword)
{
  keyword_ =   keyword;
  registerOrder(keyword_);
}




OrderDialog::~OrderDialog() {
}

void OrderDialog::registerOrder(string keyword) 
{
    // Try to find this object in registry
    OrderDialog * current = OrderDialog::findOrderDialogByKeyword(keyword);
    if (current == 0) // Add if not found
    {      
        OrderDialog::orderDialogRegister.push_back(this);
    }
    cout<<endl;
}



OrderDialog * OrderDialog::findOrderDialog(OrderLine * order)
{
    if(order == 0)
    {
         cerr<< "showOrderWindow:: Order line was not provided"<<endl;
        return 0;
    }
 OrderPrototype * orderPrototype = order->getOrderPrototype();
 if(orderPrototype ==0)
 {
     cerr<< "Order line [";
     order->printOrderLine(cerr);
     cerr << "] contains no prototype"<<endl;
     return 0;
 }
 return findOrderDialogByKeyword(orderPrototype->getKeyword());
}



OrderDialog * OrderDialog::findOrderDialogByKeyword(string keyword)
{
   vector<OrderDialog *>::iterator iter;
  for(iter = OrderDialog::orderDialogRegister.begin();
          iter != OrderDialog::orderDialogRegister.end(); ++iter)
  {
     if ((*iter)->getKeyword() == keyword)
      {
           return (*iter);   
        }
  }
  return 0;
}



//OrderLine * OrderDialog::showOrderWindow(TokenEntity * token, 
//                                        OrderLine * order, ViewManager * view)
//{
// OrderDialog * dialog = findOrderDialog(order);
// if(dialog ==0)
// {
//     cerr<< "Dialog is not defined for order "
//             <<order->getOrderPrototype()->getKeyword()<<endl;
//     return 0;
// }
//  return dialog->show(token,order,view);    
//}
//
//
//
//OrderLine * OrderDialog::showOrderWindow(TokenEntity * token, string keyword, ViewManager * view)
//{
//
// OrderDialog * dialog = findOrderDialogByKeyword(keyword);
// if(dialog ==0)
// {
//     cerr<< "Dialog is not defined for order "<<keyword<<endl;
//     return 0;
// }
// return dialog->show(token,0,view);     
//}
//OrderWindow * OrderDialog::edit(TokenEntity * token, OrderLine * order, ViewManager * view)
//{
//    cout<<"OrderDialog::edit (empty)"<<endl;
//    return 0;
//}




OrderWindow * OrderDialog::show(TokenEntity * token, OrderLine * order, ViewManager * view, ExtendedCommand extendedCommandFlag)
{
    return 0;
}


OrderLine * OrderDialog::getOrderLine()
{
  return 0;  
}

void OrderDialog::rewriteOrder(OrderLine * order)
{
    ParameterList   newParams = order->getParameterList();
    ParameterList &  params = order_->getParameterList();
    params = newParams;
    
}

OrderWindow * OrderDialog::prepareWindow(TokenEntity * token, OrderLine * order, ViewManager * view, string orderLabel)
{
     if(!token)
     {
        return 0;
     }
     token_ = token;
     order_ = order;
     
     if(order)
     {
       params = order->getParameterList();  
       isNewOrder_ = false;
     }
     else
     {
       isNewOrder_ = true;  
     }


      OrderWindow * orderWindow = new OrderWindow(0,Qt::Popup | Qt::WindowTitleHint);
      orderWindow->clean();
      orderWindow->setMyLayout();
      orderWindow->setDialog(this);
      QObject::connect(orderWindow, SIGNAL(orderLineChanged()), view, SLOT(updateView()));

     ClickableLabel * l1 = new ClickableLabel(orderLabel + " "); // Add Label
     orderWindow->addWidget(l1); 
     return orderWindow;
}

void OrderDialog::showWindow(OrderWindow * orderWindow)
{
    orderWindow->move(QPoint(500,800));
    orderWindow->show();       
}

OrderLine * OrderDialog::updateOrderLine(string s)
{
    OrderLine * orderLine = new OrderLine(s, token_);
    if(isNewOrder_)
    {
      token_->addOrder(s);  
    }
    else
    {
        this->rewriteOrder(orderLine);
    }
    return orderLine; 
}
