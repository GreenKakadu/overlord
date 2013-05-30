/* 
 * File:   LearnSkillOrderDialog.cpp
 * Author: alex
 * 
 * Created on May 23, 2013, 12:51 PM
 */

#include <sstream>
#include "UnitEntity.h"
#include "FactionEntity.h"
#include "LocationEntity.h"
#include "ViewManager.h"
#include "ClickableLabel.h"
#include "LearnSkillOrderDialog.h"
//
//        STUDY <skill tag> [level] 
//        FORGET skill-tag
//
OrderWindow * LearnSkillOrderDialog::show(TokenEntity * token, OrderLine * order, ViewManager * view, ExtendedCommand command)
{
    
    GameData * data =0;
    SkillRule * skillToStudy;
    IntegerData * num;
    int level =0;
    string commandName;
     OrderWindow * orderWindow=0;
    //SkillLevelElement * skillElement = 0;
   skillToStudy = view->getSelectedSkill();
   levelSB_ =0;
    level = token->getSkillLevel(skillToStudy);
    if(order)
    {
           if(order->getOrderPrototype()->getKeyword() == "study")
           {
           }
            if(order->getOrderPrototype()->getKeyword() == "forget")
           {
             command = ExtendedCommand::FORGET;
           
           }
   }
    
    
    if(command != ExtendedCommand::FORGET)
    {
       commandName = "Study"; 
    }else
    {
        commandName = "Forget";
    }
    orderWindow = this->prepareWindow(token,order,view,commandName);
    
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
       
    if( command != ExtendedCommand::FORGET)
   {
       for (RulesIterator skillIter = gameFacade->skills.begin(); skillIter != gameFacade->skills.end(); skillIter++)
        {
            SkillRule * skill = dynamic_cast<SkillRule*> (*skillIter);
            if (skill == 0)
                continue;
            if(skill == skillToStudy)
                continue;
            if (token->mayStudySkill(skill))
            {
                skillCB_ ->addGameItem(skill);
            }
        }
    }
    else
    {
     for(SkillIterator iter =  token->getAllSkills().begin(); iter != token->getAllSkills().end(); ++iter)
    {
        if((*iter).getSkill() != skillToStudy) // Already added
        {
        skillCB_->addGameItem ((*iter).getSkill());
        }
    }       
    }
//      skillElement = new SkillLevelElement(skillToStudy,level);     
//       skillCB_ = new OvlElementComboBox(skillElement);
          orderWindow->addWidget(skillCB_); 
   if( command != ExtendedCommand::FORGET)
   {
       levelSB_ = orderWindow->createSpinBox(level,0,99); 
       orderWindow->addWidget(levelSB_);
   }
//    for(SkillIterator iter =  token->getAllSkills().begin(); iter != token->getAllSkills().end(); ++iter)
//    {
//        if((*iter).getSkill() != skillToStudy) // Already added
//        {
//        skillCB_->addGameItem ((*iter).getSkill());
//        }
//    }
//    


    
 


   showWindow(orderWindow); 
    return orderWindow;
}



OrderLine * LearnSkillOrderDialog::getOrderLine()
{
    stringstream s;
    string  skillTag;
    int level;

//    AbstractData * data = skillCB_->getCurrentItem();
//    if(data)
//    {
//        SkillLevelElement * skillLevel = dynamic_cast<SkillLevelElement *>(data);
//      if(skillLevel)
//      {
//         skillTag=skillLevel->getSkill()->getTag();  
//        level = skillLevel->getLevel();
//      }  
//    }
//    
//    s <<keyword_<<" "<<skillTag ; 
//    if(level)
//    {
//        s <<" "<<level;
//    }
//    s<<endl;  

    GameData * item = skillCB_->getCurrentItem();
    if(item)
    {
      skillTag = item->getTag();  
    }
    
    s <<keyword_<<" "<<skillTag; 
    if(levelSB_)
    {
        if(levelSB_->value())
        {
            s <<" "<<levelSB_->value()<<endl;
        }
    }
    s<<endl;  
    return updateOrderLine(s.str());  
}



