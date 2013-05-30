/*
 * File:   OathOrderDialog.cpp
 * Author: alex
 *
 * Created on March 19, 2013
 */
#include <sstream>
#include "GameFacade.h"
#include "ViewManager.h"
#include "AttackOrderDialog.h"
#include "UnitEntity.h"
#include "FactionEntity.h"
#include "ConstructionEntity.h"
#include "LocationEntity.h"
//
//        ORDER  ATTACK [ unit-id|faction-id|structure-id ]
//
//
OrderWindow * AttackOrderDialog::show(TokenEntity * token, OrderLine * order, ViewManager * view, ExtendedCommand command)
{
    GameData * data =0;
    GameData * attackTarget=0;
    GameData * activeData =view->getActiveData();
    FactionEntity * faction = token->getFaction();

    if(!activeData)
    {
        cerr <<"ERROR: void target for order "<<order->print() <<" for "<<token <<endl;
        return 0;
    }

    attackTarget = view->getSelectedToken();

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
     attackTarget = data;

     OrderWindow * orderWindow = this->prepareWindow(token,order,view,"Attack");
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
            attackTarget = data;
     }





       targetCB_ = new OvlComboBox(attackTarget);


  // Add items to ComboBoxes and add widgets
    orderWindow->addWidget(targetCB_);

    //Enemies first.
    addTargetsByStance(token,enemyStance);
    addTargetsByStance(token,hostileStance);
    addTargetsByStance(token,unknownStance);
    addTargetsByStance(token,neutralStance);
    addTargetsByStance(token,friendlyStance);



      showWindow(orderWindow);
      return orderWindow;
}



OrderLine * AttackOrderDialog::getOrderLine()
{
    stringstream s;
    string targetTag;

    GameData * target = targetCB_->getCurrentItem();
    if(target)
    {
      targetTag = target->getTag();
    }
    s <<keyword_ <<" "<<targetTag <<endl;
    return updateOrderLine(s.str());
}


void AttackOrderDialog::addTargetsByStance(TokenEntity * token, StanceVariety * stance)
{
     FactionEntity * faction = token->getFaction();
for (int i = 0; i < gameFacade->factions.size(); ++i)
{
   if(gameFacade->factions[i] == 0 )
       continue;
   if(faction->getStance(gameFacade->factions[i])== stance)
   {
     targetCB_->addGameItem(gameFacade->factions[i]);
   }
}
  //Units
vector <UnitEntity *> units = token->getLocation()->unitsPresent();// only units that can be seen?
for (vector <UnitEntity *>::iterator iter = units.begin(); iter != units.end(); ++iter)
   {
   if(faction->getStance((*iter))== stance)
       {
           targetCB_->addGameItem((*iter));
       }
   }


// Buildings
vector <ConstructionEntity *> constructions = token->getLocation()->constructionsPresent();
for (vector <ConstructionEntity *>::iterator iter = constructions.begin(); iter != constructions.end(); ++iter)
    {
    if(faction->getStance((*iter))== stance)
        {
            targetCB_->addGameItem((*iter));
        }
    }


}
