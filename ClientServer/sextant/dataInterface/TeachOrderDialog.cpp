/* 
 * File:   TeachOrderDialog.cpp
 * Author: alex
 * 
 * Created on May 27, 2013, 8:39 AM
 */
#include <sstream>
#include "UnitEntity.h"
#include "FactionEntity.h"
#include "LocationEntity.h"
#include "ViewManager.h"
#include "ClickableLabel.h"
#include "TeachOrderDialog.h"

//
//        TEACH <skill tag> [level] 
//
OrderWindow * TeachOrderDialog::show(TokenEntity * token, OrderLine * order, ViewManager * view, ExtendedCommand command)
{
    
    GameData * data =0;
    SkillRule * skillToStudy;
    IntegerData * num;
    int level =0;

     OrderWindow * orderWindow=0;

   skillToStudy = view->getSelectedSkill();
   levelSB_ =0;
    level = token->getSkillLevel(skillToStudy);

    orderWindow = this->prepareWindow(token,order,view,"Teach");
    
     if(orderWindow==0)
     {
         return 0;
     }
  
     
     
     
     
     if(!isNewOrder_ && params.size() >= 1)
     {
        skillToStudy = dynamic_cast<SkillRule *>(params[0]);
        if(skillToStudy ==0)
        {
            cerr<<"ERROR: wrong 1-st parameter ["<<(params[0])->print()<<"] in order "
                    <<order->print() <<" for "<<token <<endl;  
        }        
        
        if(params.size() >= 2)
        {
            num = dynamic_cast<IntegerData *>(params[1]);
            if(num) 
            {
            level = num->getValue();
            }
        }
     }
       skillCB_ = new OvlComboBox(skillToStudy);
       


    {
     for(SkillIterator iter =  token->getAllSkills().begin(); iter != token->getAllSkills().end(); ++iter)
    {
        if((*iter).getSkill() != skillToStudy) // Already added
        {
        skillCB_->addGameItem ((*iter).getSkill());
        }
    }       
    }

          orderWindow->addWidget(skillCB_); 

 


    
 


   showWindow(orderWindow); 
    return orderWindow;
}



OrderLine * TeachOrderDialog::getOrderLine()
{
    stringstream s;
    string  skillTag;



    GameData * item = skillCB_->getCurrentItem();
    if(item)
    {
      skillTag = item->getTag();  
    }
    
    s <<keyword_<<" "<<skillTag; 

    s<<endl;  
    return updateOrderLine(s.str());  
}






