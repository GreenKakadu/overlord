/* 
 * File:   StanceOrderDialog.cpp
 * Author: alex
 * 
 * Created on May 21, 2013, 8:53 PM
 */

#include "StanceOrderDialog.h"

#include <sstream>
#include "GameFacade.h"
#include "ViewManager.h"
#include "UnitEntity.h"
#include "ConstructionEntity.h"
#include "FactionEntity.h"
#include "LocationEntity.h"
//
//       
//       STANCE  faction-id |unit-id | structure-id   stance
//
OrderWindow * StanceOrderDialog::show(TokenEntity * token, OrderLine * order, ViewManager * view, ExtendedCommand command)
{
    GameData * data =0;
    GameData * stanceTarget=0;
    GameData * activeData =view->getActiveData();
    FactionEntity * faction = token->getFaction();
    GameData * stance=faction->getDefaultStance();

    if(activeData)
    {
        stanceTarget = view->getSelectedToken();

         data = dynamic_cast<UnitEntity *>(activeData);
         if(!data)
         {
           data = dynamic_cast<FactionEntity *>(activeData);
           if(!data)
           {
             data = dynamic_cast<ConstructionEntity *>(activeData);
             if(!data)
             {
                 cerr <<"ERROR: wrong target ["<<activeData->print()<<"] in order "
                     <<order->print() <<" for "<<token <<endl;
             }
           }
         }
         stanceTarget = data;     
    }


     OrderWindow * orderWindow = this->prepareWindow(token,order,view,"Stance");
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
            stanceTarget = data;
            
          if(!isNewOrder_ && params.size() >= 2)
         {
                data = dynamic_cast<GameData *>(params[1]);
                if(!data)
                {
                        cerr<<"ERROR: wrong 2-nd parameter ["<<(params[1])->print()<<"] in order "
                        <<order->print() <<" for "<<token <<endl;
                }
                stance = data;
         }

     }





       stanceTargetCB_ = new OvlComboBox(stanceTarget);
       stanceCB_ = new OvlComboBox(stance);


  // Add items to ComboBoxes and add widgets
    orderWindow->addWidget(stanceTargetCB_);

     for (int i = 0; i < gameFacade->factions.size(); ++i)
     {
        if(gameFacade->factions[i] != 0 && gameFacade->factions[i] != token->getFaction())
        {
          stanceTargetCB_->addGameItem(gameFacade->factions[i]);
        }
     }


     vector <UnitEntity *> units = token->getLocation()->unitsPresent();
    for (vector <UnitEntity *>::iterator iter = units.begin(); iter != units.end(); ++iter)
        {
            if (((*iter)->getFaction() != token->getFaction()))
            {
                stanceTargetCB_->addGameItem((*iter));
            }
        }


// Buildings
vector <ConstructionEntity *> constructions = token->getLocation()->constructionsPresent();
for (vector <ConstructionEntity *>::iterator iter = constructions.begin(); iter != constructions.end(); ++iter)
    {
    if(((*iter)->getFaction() != token->getFaction()))
        {
            stanceTargetCB_->addGameItem((*iter));
        }
    }


orderWindow->addWidget(stanceCB_);
     for (int i = 0; i < gameFacade->stances.size(); ++i)
     {  
         if((gameFacade->stances[i])== privateStance || (gameFacade->stances[i])==unknownStance)
             continue;
          stanceCB_->addGameItem(gameFacade->stances[i]);       
     }
   

      showWindow(orderWindow);
      return orderWindow;
}



OrderLine * StanceOrderDialog::getOrderLine()
{
    stringstream s;
    string stanceTargetTag;
    string stanceTag;

    GameData * stanceTarget = stanceTargetCB_->getCurrentItem();
    GameData * stance = stanceCB_->getCurrentItem();
    if(stanceTarget)
    {
      stanceTargetTag = stanceTarget->getTag();
    }
    if(stance)
    {
        stanceTag =  stance->getTag();
    }
    s <<keyword_ <<" "<<stanceTargetTag <<" "<<stanceTag <<endl;
    return updateOrderLine(s.str());
}


