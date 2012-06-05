/* 
 * File:   UseSkillOrderDialog.cpp
 * Author: alex
 * 
 * Created on February 27, 2012, 2:12 PM
 */
#include <sstream>
#include "UseSkillOrderDialog.h"
#include "UnitEntity.h"
#include "FactionEntity.h"
#include "LocationEntity.h"
#include "ViewManager.h"
#include "ClickableLabel.h"
//
//        USE skill-tag [target] [num of products]
//   
// 
OrderWindow * UseSkillOrderDialog::show(TokenEntity * token, OrderLine * order, ViewManager * view, ExtendedCommand command)
{
    
    GameData * data =0;
    GameData * skillToUse;
    GameData * target=0;
    IntegerData * amount;
    int useCounter =0;
    skillToUse = view->getSelectedSkill();
    
       OrderWindow * orderWindow = this->prepareWindow(token,order,view,"Use");
     if(orderWindow==0)
     {
         return 0;
     }
  
    
//     if(!token)
//     {
//        return 0;
//     }
//     token_ = token;
//     order_ = order;
//     if(order)
//     {
//       params = order->getParameterList();  
//       isNewOrder_ = false;
//     }
//     else
//     {
//       isNewOrder_ = true;  
//     }
//
//
//
//
//     OrderWindow * orderWindow = new OrderWindow(0,Qt::Popup | Qt::WindowTitleHint);
//      orderWindow->clean();
//      orderWindow->setMyLayout();
//      orderWindow->setDialog(this);
//      QObject::connect(orderWindow, SIGNAL(orderLineChanged()), view, SLOT(updateView()));
//
//     ClickableLabel * l1 = new ClickableLabel("Use "); // Add Label
//     orderWindow->addWidget(l1);
//     
     
     
     
     if(!isNewOrder_ && params.size() >= 1)
     {
        data =  dynamic_cast<GameData *>(params[0]);
        if(data ==0)
        {
            cerr<<"ERROR: wrong 1-st parameter ["<<(params[0])->print()<<"] in order "
                    <<order->print() <<" for "<<token <<endl;  
        }        
        skillToUse = data;
        
        if(params.size() >= 2)
        {
            amount = dynamic_cast<IntegerData *>(params[1]);
            if(amount) // 2-nd parameter is integer. No target defined
            {
            useCounter = amount->getValue();
            }
            else// 2-nd parameter is not integer. Then it should be  target.
            {
                data = dynamic_cast<GameData *>(params[1]);
                if(data == 0)
                {
                    cerr<<"ERROR: wrong 2-nd parameter ["<<(params[1])->print()<<"] in order "
                    <<order->print() <<" for "<<token <<endl; 
                } 
                target =  data;
                // now try to find counter
                if(params.size() >= 3)
                {           
                    amount = dynamic_cast<IntegerData *>(params[2]); 
                    if(amount) 
                    {
                        useCounter = amount->getValue();
                    }
                }           
            }
        }
     }
       amountSB_ = orderWindow->createSpinBox(useCounter,0,99); 
       skillCB_ = new OvlComboBox(skillToUse);
       targetCB_ = new OvlComboBox(target);
       
    orderWindow->addWidget(skillCB_); 
    for(SkillIterator iter =  token->getAllSkills().begin(); iter != token->getAllSkills().end(); ++iter)
    {
        if((*iter).getSkill() != skillToUse) // Already added
        {
        skillCB_->addGameItem ((*iter).getSkill());
        }
    }
    

    orderWindow->addWidget(targetCB_);
    // suggest targets (?)
    // 
    
    orderWindow->addWidget(amountSB_);

//    vector <InventoryElement> inventory = token->getAllInventory();
//    for (InventoryIterator iterEquip = inventory.begin();
//                      iterEquip != inventory.end(); iterEquip++)
//      {
////          if((*iterEquip).getItemType() != itemToGive)
////          {
//          itemCB_->addGameItem((*iterEquip).getItemType());
//
////          }
//      }

//::setCustomStyle(l1,ORDER_TEXT);
//::setCustomStyle(cb,ORDER_COMBO);
//::setCustomStyle(cb2,ORDER_COMBO);




//      orderWindow->move(QPoint(500,800));
//      orderWindow->show();

   showWindow(orderWindow); 
    return orderWindow;
}



OrderLine * UseSkillOrderDialog::getOrderLine()
{
    stringstream s;
    string targetTag, skillTag;
    GameData * target = targetCB_->getCurrentItem();
    if(target)
    {
      targetTag = target->getTag();  
    }
    GameData * item = skillCB_->getCurrentItem();
    if(item)
    {
      skillTag = item->getTag();  
    }
    
    s <<keyword_<<" "<<skillTag  <<" "<<targetTag; 
    if(amountSB_->value())
    {
        s <<" "<<amountSB_->value()<<endl;
    }
    s<<endl;  

    return updateOrderLine(s.str());  
}

