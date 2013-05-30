#include <QtGui/QMainWindow>
#include "ClickableLabel.h"
#include "ClickableLine.h"
#include "ViewManager.h"
#include "MainClientWindow.h"
#include "MapView.h"
#include "LocationView.h"
#include "FactionView.h"
#include "UnitView.h"
#include "FactionEntity.h"
#include "TitleRule.h"
#include "InfoWindow.h"
#include "PopupMenuWindow.h"
#include "OrderWindow.h"
#include "SkillLevelElement.h"
#include "RaceElement.h"
#include "OrderPrototypesCollection.h"
#include "OrderDialog.h"
#include "GiveItemOrderDialog.h"
#include "LineParser.h"

using namespace std;
ViewManager::ViewManager(MainClientWindow * parent)
{

isOpened_ = false;
parent_ = parent;
infoWindow= 0;
popupMenuWindow =0;
tokenSelected_ = 0;
orderWindow =0;
activeData_ = 0;

    // Create  Layouts ------------------------------------
     leftDock = new QDockWidget(tr("Faction"),parent);
     leftDock->setAllowedAreas(Qt::LeftDockWidgetArea);

     center = new QDockWidget(tr("Map"),parent);
     center->setAllowedAreas(Qt::TopDockWidgetArea);

     rightDock1 = new QDockWidget(tr("Location"), parent);
     rightDock1->setAllowedAreas(Qt::RightDockWidgetArea);

     rightDock2 = new QDockWidget(tr("Unit"), parent);
     rightDock2->setAllowedAreas(Qt::RightDockWidgetArea);

     connect(parent,SIGNAL(showFactionView()),this,SLOT(showFactionView()));
     connect(parent,SIGNAL(showMapView()),this,SLOT(showMapView()));
     connect(parent,SIGNAL(showUnitView()),this,SLOT(showUnitView()));
     connect(parent,SIGNAL(showLocationView()),this,SLOT(showLocationView()));
}



void ViewManager::openView()
{
    activeData_ = 0;
    // Create FactionView -------------------------------

    FactionEntity * faction=parent_->gameViewConf.currentFaction;
    if(faction==0)
    {
        const string factionTag = "f08";
        faction = gameFacade->factions[factionTag];
    }
    factionView_   = new FactionView(faction);
    factionSelected_ = faction;

    // Create UnitView ---------------------------------------
    TokenEntity * currentUnit = parent_->gameViewConf.currentUnit;
    if(currentUnit ==0)
    {
        if(faction)
        {
            if(!faction->getLoyalUnits().empty())
            {
                currentUnit = (*(faction->getLoyalUnits().begin()));
            }
        }
    }
    unitView_     = new UnitView(currentUnit);
    tokenSelected_ = currentUnit;

    // Create LocationView ---------------------------------
    LocationEntity * local = parent_->gameViewConf.currentLocation;
    if(local == 0)
    {
        if(currentUnit)
        {
            local = currentUnit->getLocation();
        }
    }
    locationSelected_ = local;
    // Create views ------------------------------------
    locationView_ = new LocationView(local,rightDock2);
    map      = new MapView("Map view",locationView_,center,&(parent_->gameViewConf));
    connect(map,SIGNAL(sendMousePosSignal(int,int)),
            parent_,SLOT(showMousePos(int,int)));

    // Set  initial Layout ------------------------------------
    leftDock->setWidget(factionView_);
    center->setWidget(map);
    parent_->setCentralWidget(center);
    rightDock1->setWidget(locationView_);
    rightDock2->setWidget(unitView_);
    parent_->addDockWidget(Qt::LeftDockWidgetArea, leftDock);
    parent_->addDockWidget(Qt::RightDockWidgetArea, rightDock1);
    parent_->addDockWidget(Qt::RightDockWidgetArea, rightDock2);

    // connect signals and slots of views------------------------------------
    connect(factionView_,SIGNAL(leftClickedSignal(AbstractData*)),
            this,SLOT(leftClickedSignalHandler(AbstractData*)));
    connect(locationView_,SIGNAL(leftClickedSignal(AbstractData*)),
            this,SLOT(leftClickedSignalHandler(AbstractData*)));
    connect(unitView_,SIGNAL(leftClickedSignal(AbstractData*)),
            this,SLOT(leftClickedSignalHandler(AbstractData*)));

    connect(factionView_,SIGNAL(rightClickedSignal(AbstractData*)),
            this,SLOT(rightClickedSignalHandler(AbstractData*)));
    connect(locationView_,SIGNAL(rightClickedSignal(AbstractData*)),
            this,SLOT(rightClickedSignalHandler(AbstractData*)));
    connect(unitView_,SIGNAL(rightClickedSignal(AbstractData*)),
            this,SLOT(rightClickedSignalHandler(AbstractData*)));

    connect(factionView_,SIGNAL(doubleClickedSignal(AbstractData*)),
            this,SLOT(doubleClickedSignalHandler(AbstractData*)));
    connect(locationView_,SIGNAL(doubleClickedSignal(AbstractData*)),
            this,SLOT(doubleClickedSignalHandler(AbstractData*)));
    connect(unitView_,SIGNAL(doubleClickedSignal(AbstractData*)),
            this,SLOT(doubleClickedSignalHandler(AbstractData*)));

    connect(factionView_,SIGNAL(mouseOverSignal(AbstractData*)),
            this,SLOT(mouseOverSignalHandler(AbstractData*)));
    connect(locationView_,SIGNAL(mouseOverSignal(AbstractData*)),
            this,SLOT(mouseOverSignalHandler(AbstractData*)));
    connect(unitView_,SIGNAL(mouseOverSignal(AbstractData*)),
            this,SLOT(mouseOverSignalHandler(AbstractData*)));

    connect(factionView_,SIGNAL(clickedSignal(AbstractData*, QMouseEvent *)),
            this,SLOT(clickedSignalHandler(AbstractData*, QMouseEvent *)));
    connect(locationView_,SIGNAL(clickedSignal(AbstractData*, QMouseEvent *)),
            this,SLOT(clickedSignalHandler(AbstractData*, QMouseEvent *)));
    connect(unitView_,SIGNAL(clickedSignal(AbstractData*, QMouseEvent *)),
            this,SLOT(clickedSignalHandler(AbstractData*, QMouseEvent *)));

    connect(this,SIGNAL(goToLocation(LocationEntity*)),
            locationView_,SLOT(updateLocation(LocationEntity*)));
    connect(this,SIGNAL(goToLocation(LocationEntity*)),
            map,SLOT(centerOnHex(LocationEntity*)));
    connect(this,SIGNAL(goToFaction(FactionEntity*)),
            factionView_,SLOT(updateFactionData(FactionEntity*)));
//    connect(this,SIGNAL(goToFaction(FactionEntity*)),
//            this,SLOT(showFactionView()));
    connect(this,SIGNAL(goToUnit(TokenEntity*)),
            unitView_,SLOT(updateUnitView(TokenEntity*)));
//    connect(this,SIGNAL(goToUnit(TokenEntity*)),
//            this,SLOT(showUnitView()));
    QObject::connect(this, SIGNAL(updateUnitView(TokenEntity*)), unitView_, SLOT(updateUnitView(TokenEntity*)));
    
    // Connect goToRule

    factionView_->setMinimumSize(340, 600);
    unitView_->setMinimumSize(500, 300);
    locationView_->setMinimumSize(500, 300);
    map->setMinimumSize(600,600);

  leftDock->show();
  center->show();
  rightDock1->show();
  rightDock2->show();
  isOpened_ = true;
}
//=====================================================================================
//
//          Signal Handlers
//
//=====================================================================================








void ViewManager::leftClickedSignalHandler(AbstractData * data)
 {
    //   cout << "ViewManager::Mouse left-clicked "<<endl;

// If in order mode forward this click to ContextManager
//    if()
//    {
//        emit leftClickedSignal(data);
//        return;
//    }

// Data may be order. Check keyword    
//    StringData * str = dynamic_cast<StringData  *> (data);
//    if(str)
//    {
//        //cout<< "ViewManager::leftClickedSignalHandler "<<endl;
//        Parser * parser = new Parser(str->print());
//        string keyword =  parser->getWord();       
//
//        OrderPrototype * orderPrototype_ = orderPrototypesCollection->find(keyword);
//        if(orderPrototype_)
//        {
//            cout<< "ViewManager::leftClickedSignalHandler Order "<< keyword<<" detected"<<endl;
//            return;
//        }
//    }
    
// Data may be order.     
    OrderLine * order = dynamic_cast< OrderLine *> (data);
    {
        if(order)
        {
            order->printOrderLine(cout);
            showEditOrderDialog(order);
         return;
        }
                
    }
    
    
    
     LocationEntity * location = dynamic_cast< LocationEntity *> (data);
     if(location)
     {
         //cout<<data->print()<< "(Location)"<<endl;
         parent_->gameViewConf.currentLocation = location;
         emit goToLocation(location);
         return;
     }
     FactionEntity * faction = dynamic_cast< FactionEntity *> (data);
     if(faction)
     {
       // cout<<data->print()<< "(Faction)"<<endl;
         parent_->gameViewConf.currentFaction = faction;
         emit goToFaction(faction);
         return;
     }
     TokenEntity * token = dynamic_cast< TokenEntity *> (data);
     if(token)
     {
         cout<<data->print()<< "(Unit)"<<endl;
         parent_->gameViewConf.currentUnit = token;
         tokenSelected_ = token;
         emit goToUnit(token);
         return;
     }
   // Data is rule.
   //   showRuleInfo(data);

    // Data is Skill Level
    // showSkillInfo(data);
//emit leftClickedSignal(data);
 }




void ViewManager::doubleClickedSignalHandler(AbstractData * data)
 {
    //emit doubleClickedSignal(data);
}
void ViewManager::clickedSignalHandler(AbstractData * data, QMouseEvent * event )
 {
    //emit clickedSignal(data,event);
}

 void ViewManager::rightClickedSignalHandler(AbstractData * data)
{
      //cout << "ViewManager::Mouse right-clicked "<<endl;
//emit rightClickedSignal(data);
     if(data)
     {
        if(infoWindow)//Clicked element already has info opened. Close it.
         {
             emit closeInfoWindow();
         }
           //cout << "ViewManager::Mouse right-clicked "<<data->print()<<endl;
           showSkillPopupMenu(data);
           showTitlePopupMenu(data);
           showRacePopupMenu(data);
           showItemPopupMenu(data);
           showUnitPopupMenu(data);
           showConstructionPopupMenu(data);
           showLocationPopupMenu(data);
           showFactionPopupMenu(data);
     }
}

 void ViewManager::mouseOverSignalHandler(AbstractData * data)
 {
     //emit mouseOverSignal(data);
      //currentInfo_ = data;
     if(data)
     {
         //cout<<"ViewManager::mouseOverSignalHandler Open invoked"<<endl;
         if(popupMenuWindow)//do not open info if popup menu is already opened
         {
            if(popupMenuWindow->isVisible())
            {
             return;
            }
         }

         showLocationInfo(data);
         showFactionInfo(data);
         showTokenInfo(data);
         showRuleInfo(data);
         showSkillInfo(data);
     }
     else
     {
         //cout<<"ViewManager::mouseOverSignalHandler Close invoked"<<endl;
         emit closeInfoWindow();
     }
 }

 void ViewManager::updateView()
 {
     emit updateUnitView(this->getSelectedToken());
 }
 
 
//=======================================================================================
//
// Actions in response on signals
//
//====================================================================================

bool ViewManager::showRuleInfo(AbstractData * data)
{
    Rule * rule = dynamic_cast< Rule *> (data);
    if(rule)
    {
    //infoWindow = new InfoWindow(/*parent_*/0,Qt::FramelessWindowHint);
    infoWindow = new InfoWindow(0,Qt::ToolTip);
    infoWindow->clean();
    vector <AbstractData * > v = rule->aPrint();
  for(vector<AbstractData * >::iterator iter = v.begin();
      iter != v.end(); ++iter)
  {
      //cout<<(*iter)->print()<< "(Rule adding)"<<endl;
      infoWindow->addWidget(new ClickableLabel(*iter));
  }

    infoWindow->setMyLayout();
    connect(this,SIGNAL(closeInfoWindow()),infoWindow,SLOT(close()));
    connect(infoWindow,SIGNAL(mouseOverSignal(AbstractData*)),
            this,SLOT(mouseOverSignalHandler(AbstractData*)));
    infoWindow->move(parent_->mapFromGlobal(QCursor::pos()) + QPoint(20,30));
    infoWindow->show();
    return true;
}
    return false;
}



bool ViewManager::showSkillInfo(AbstractData * data)
{
    SkillLevelElement * skill = dynamic_cast<SkillLevelElement *>(data);
    if(skill != 0)
    {
        infoWindow = new InfoWindow(0,Qt::ToolTip);
        infoWindow->clean();
        vector <AbstractData * > v = skill->getSkill()->aPrint(skill->getLevel());
        for(vector<AbstractData * >::iterator iter = v.begin();
            iter != v.end(); ++iter)
        {
            //cout<<(*iter)->print()<< "(Rule adding)"<<endl;
            infoWindow->addWidget(new ClickableLabel(*iter));
        }
            infoWindow->setMyLayout();
            connect(this,SIGNAL(closeInfoWindow()),infoWindow,SLOT(close()));
            connect(infoWindow,SIGNAL(mouseOverSignal(AbstractData*)),
                    this,SLOT(mouseOverSignalHandler(AbstractData*)));
            infoWindow->move(parent_->mapFromGlobal(QCursor::pos()) + QPoint(20,30));
            infoWindow->show();
            return true;

    }
    return false;
}


void ViewManager::showEditOrderDialog(OrderLine * order)
{
    cout << "Called showEditOrderDialog"<<endl;
  OrderWindow::editOrderWindow(getSelectedToken(),  order, this);
}

 bool ViewManager::showSkillPopupMenu(AbstractData * data)
 {
     SkillLevelElement * skill = dynamic_cast<SkillLevelElement *>(data);
     if(skill != 0)
     {
          activeSkill = skill->getSkill();
         popupMenuWindow = new PopupMenuWindow(0,Qt::ToolTip);
         popupMenuWindow->clean();
         ClickableLabel * l1 = popupMenuWindow->addMenuItem("Use");
         ClickableLabel * l2 = popupMenuWindow->addMenuItem("Learn");
         ClickableLabel * l3 = popupMenuWindow->addMenuItem("Teach");
         ClickableLabel * l4 = popupMenuWindow->addMenuItem("Forget");
         ::setCustomStyle(l1,BLUE_COLOR);
         ::setCustomStyle(l2,BLUE_COLOR);
         ::setCustomStyle(l3,BLUE_COLOR);
         ::setCustomStyle(l4,BLUE_COLOR);
         popupMenuWindow->setMyLayout();
         //connect(this,SIGNAL(closeInfoWindow()),popupMenuWindow,SLOT(close()));
         connect(this,SIGNAL(closePopupMenu()),popupMenuWindow,SLOT(doClose()));
         
         connect(l1,SIGNAL(leftClicked(AbstractData*)),
                         this,SLOT(useSkillSignalHandler(AbstractData*)));
         connect(l2,SIGNAL(leftClicked(AbstractData*)),
                         this,SLOT(learnSkillSignalHandler(AbstractData*)));
         connect(l3,SIGNAL(leftClicked(AbstractData*)),
                         this,SLOT(teachSkillSignalHandler(AbstractData*)));
         connect(l4,SIGNAL(leftClicked(AbstractData*)),
                         this,SLOT(forgetSkillSignalHandler(AbstractData*)));
         popupMenuWindow->move(parent_->mapFromGlobal(QCursor::pos()) + QPoint(20,30));
         popupMenuWindow->show();
         return true;
     }
     return false;
 }


 void ViewManager::useSkillSignalHandler(AbstractData*) //Temp
{
      OrderWindow::showOrderWindow(tokenSelected_,"use",this); 
     popupMenuWindow->doClose();
 }
 
 
 
 void ViewManager::learnSkillSignalHandler(AbstractData*) //Temp
{
      OrderWindow::showOrderWindow(tokenSelected_,"study",this); 
     popupMenuWindow->doClose();
 }
 
 
 
 void ViewManager::teachSkillSignalHandler(AbstractData*) //Temp
{
      OrderWindow::showOrderWindow(tokenSelected_,"teach",this); 
     popupMenuWindow->doClose();
 }
 
 
 void ViewManager::forgetSkillSignalHandler(AbstractData*) //Temp
{
      OrderWindow::showOrderWindow(tokenSelected_,"forget",this, ExtendedCommand::FORGET); 
     popupMenuWindow->doClose();
 }

 
 void ViewManager::showItemPopupMenu(AbstractData * data)
 {
     ItemRule  * item = dynamic_cast<ItemRule *>(data);
     {
         
         if(item)
         {
             activeItem = item;
             setActiveData(item);
            //cout<<"ContextManager::showItemPopupMenu invoked "<< item->print()<<endl;
             popupMenuWindow = new PopupMenuWindow(0,Qt::ToolTip);
             popupMenuWindow->clean();
             //equip, unequip, buy, sell, give, use, drop
 //            ClickableLabel * l1 = popupMenuWindow->addMenuItem("Use"); // Item can't be used now
             ClickableLabel * l2 = popupMenuWindow->addMenuItem("Buy");
             ClickableLabel * l3 = popupMenuWindow->addMenuItem("Sell");
             ClickableLabel * l4 = popupMenuWindow->addMenuItem("Equip");
             ClickableLabel * l5 = popupMenuWindow->addMenuItem("Unequip");
             ClickableLabel * l6 = popupMenuWindow->addMenuItem("Give");
             ClickableLabel * l7 = popupMenuWindow->addMenuItem("Get");
             ClickableLabel * l8 = popupMenuWindow->addMenuItem("Swap");

             //connect(this,SIGNAL(closeInfoWindow()),popupMenuWindow,SLOT(close()));
               connect(this,SIGNAL(closePopupMenu()),popupMenuWindow,SLOT(doClose()));

 //            connect(l1,SIGNAL(leftClicked(AbstractData*)),this,SLOT(useItemSignalHandler(AbstractData*)));
             connect(l2,SIGNAL(leftClicked(AbstractData*)),this,SLOT(buyItemSignalHandler(AbstractData*)));
             connect(l3,SIGNAL(leftClicked(AbstractData*)),this,SLOT(sellItemSignalHandler(AbstractData*)));
             connect(l4,SIGNAL(leftClicked(AbstractData*)),this,SLOT(equipItemSignalHandler(AbstractData*)));
             connect(l5,SIGNAL(leftClicked(AbstractData*)),this,SLOT(unequipItemSignalHandler(AbstractData*)));
             connect(l6,SIGNAL(leftClicked(AbstractData*)),this,SLOT(giveItemSignalHandler(AbstractData*)));
             connect(l7,SIGNAL(leftClicked(AbstractData*)),this,SLOT(getItemSignalHandler(AbstractData*)));
             connect(l8,SIGNAL(leftClicked(AbstractData*)),this,SLOT(swapItemSignalHandler(AbstractData*)));

//             ::setCustomStyle(l1,BLUE_COLOR);
             ::setCustomStyle(l2,BLUE_COLOR);
             ::setCustomStyle(l3,BLUE_COLOR);
             ::setCustomStyle(l4,BLUE_COLOR);
             ::setCustomStyle(l5,BLUE_COLOR);
             ::setCustomStyle(l6,BLUE_COLOR);
             ::setCustomStyle(l7,BLUE_COLOR);
             ::setCustomStyle(l8,BLUE_COLOR);
             popupMenuWindow->setMyLayout();
             popupMenuWindow->move(parent_->mapFromGlobal(QCursor::pos()) + QPoint(20,30));
             popupMenuWindow->show();
         }
     }
 }

 void ViewManager::useItemSignalHandler(AbstractData*)
{
     OrderWindow::showOrderWindow(tokenSelected_,"use",this); 
     emit closePopupMenu();     
     return;
 }

 void ViewManager::buyItemSignalHandler(AbstractData*)
{
       OrderWindow::showOrderWindow(tokenSelected_,"buy",this, ExtendedCommand::BUY); 
       emit closePopupMenu();
    return;
 
 }


 void ViewManager::sellItemSignalHandler(AbstractData*)
{
      OrderWindow::showOrderWindow(tokenSelected_,"sell",this,ExtendedCommand::SELL); 
      emit closePopupMenu(); 
     return;
 }


 void ViewManager::equipItemSignalHandler(AbstractData*)
{
     OrderWindow::showOrderWindow(tokenSelected_,"equip",this,ExtendedCommand::EQUIP); 
     emit closePopupMenu();     
     return;
 }


 void ViewManager::unequipItemSignalHandler(AbstractData*)
{
     OrderWindow::showOrderWindow(tokenSelected_,"equip",this,ExtendedCommand::UNEQUIP); 
     emit closePopupMenu();     
     return;

 }


 void ViewManager::giveItemSignalHandler(AbstractData* )
{    
     OrderWindow::showOrderWindow(tokenSelected_,"give",this, ExtendedCommand::GIVE); 
     emit closePopupMenu();     
     return;
 }

 void ViewManager::getItemSignalHandler(AbstractData*)
{
     OrderWindow::showOrderWindow(tokenSelected_,"get",this /*, ExtendedCommand::GET*/); 
     emit closePopupMenu();     
     return;
 }


 void ViewManager::swapItemSignalHandler(AbstractData*)
{
      OrderWindow::showOrderWindow(tokenSelected_,"swap",this /*, ExtendedCommand::SWAP*/); 
     emit closePopupMenu();     
     return; 
 }
 
 
 
void ViewManager::showRacePopupMenu(AbstractData * data)
{
    RaceRule  * race = dynamic_cast<RaceRule *>(data);
     {
         if(race)
         {
             // set active item
             setActiveData(race);
             activeRace = race;
             popupMenuWindow = new PopupMenuWindow(0,Qt::ToolTip);
             popupMenuWindow->clean();
             // Create buttons
             ClickableLabel * l1 = popupMenuWindow->addMenuItem("Recruit");
             connect(this,SIGNAL(closePopupMenu()),popupMenuWindow,SLOT(doClose()));            
             // Connect handlers
             connect(l1,SIGNAL(leftClicked(AbstractData*)),this,SLOT(recruitSignalHandler(AbstractData*)));
              ::setCustomStyle(l1,BLUE_COLOR);
             popupMenuWindow->setMyLayout();
             popupMenuWindow->move(parent_->mapFromGlobal(QCursor::pos()) + QPoint(20,30));
             popupMenuWindow->show();             
         }
     }
}
void ViewManager::recruitSignalHandler(AbstractData*)
{

     OrderWindow::showOrderWindow(tokenSelected_,"recruit",this /*, ExtendedCommand::RECRUIT*/);
     emit closePopupMenu();     
     return;
 }


void ViewManager::showTitlePopupMenu(AbstractData * data)
{
    TitleRule  * title = dynamic_cast<TitleRule *>(data);
     {
         if(title)
         {
             // set active item
             setActiveData(title);
             activeTitle = title;
             popupMenuWindow = new PopupMenuWindow(0,Qt::ToolTip);
             popupMenuWindow->clean();
             // Create buttons
         ClickableLabel * l1 = popupMenuWindow->addMenuItem("Claim Title");
         ClickableLabel * l2 = popupMenuWindow->addMenuItem("Release Title");
         ClickableLabel * l3 = popupMenuWindow->addMenuItem("Bestow Title");
         ClickableLabel * l4 = popupMenuWindow->addMenuItem("Revoke Title");
         ::setCustomStyle(l1,BLUE_COLOR);
         ::setCustomStyle(l2,BLUE_COLOR);
         ::setCustomStyle(l3,BLUE_COLOR);
         ::setCustomStyle(l4,BLUE_COLOR);            
             connect(this,SIGNAL(closePopupMenu()),popupMenuWindow,SLOT(doClose()));            
             // Connect handlers
          connect(l1,SIGNAL(leftClicked(AbstractData*)),
                         this,SLOT(claimTitleSignalHandler(AbstractData*)));
         connect(l2,SIGNAL(leftClicked(AbstractData*)),
                         this,SLOT(releaseTitleSignalHandler(AbstractData*)));
         connect(l3,SIGNAL(leftClicked(AbstractData*)),
                         this,SLOT(bestowTitleSignalHandler(AbstractData*)));
         connect(l4,SIGNAL(leftClicked(AbstractData*)),
                         this,SLOT(revokeTitleSignalHandler(AbstractData*)));            
             
             popupMenuWindow->setMyLayout();
             popupMenuWindow->move(parent_->mapFromGlobal(QCursor::pos()) + QPoint(20,30));
             popupMenuWindow->show();             
         }
     }
}

void ViewManager::claimTitleSignalHandler(AbstractData*)
{

     OrderWindow::showOrderWindow(tokenSelected_,"claim",this); 
     emit closePopupMenu();     
     return;
 }

         
void ViewManager::releaseTitleSignalHandler(AbstractData*)
{

     OrderWindow::showOrderWindow(tokenSelected_,"yield",this); 
     emit closePopupMenu();     
     return;
 }

         


void ViewManager::bestowTitleSignalHandler(AbstractData*)
{

     OrderWindow::showOrderWindow(tokenSelected_,"bestow",this); 
     emit closePopupMenu();     
     return;
 }

         
void ViewManager::revokeTitleSignalHandler(AbstractData*)
{

     OrderWindow::showOrderWindow(tokenSelected_,"cancel",this); 
     emit closePopupMenu();     
     return;
 }

         







void ViewManager::showUnitPopupMenu(AbstractData * data)
{
    UnitEntity  * unit = dynamic_cast<UnitEntity *>(data);
     {
         if(unit)
         {
             // set active item
             setActiveData(unit);
              //tokenSelected_ = unit;
            popupMenuWindow = new PopupMenuWindow(0,Qt::ToolTip);
             popupMenuWindow->clean();
             // Create buttons
          ClickableLabel * l1 = popupMenuWindow->addMenuItem("Attack");
          ClickableLabel * l2 = popupMenuWindow->addMenuItem("Accept");
          ClickableLabel * l3 = popupMenuWindow->addMenuItem("Bestow title to");
          ClickableLabel * l4 = popupMenuWindow->addMenuItem("Eject from stack");
          ClickableLabel * l5 = popupMenuWindow->addMenuItem("Get from");
          ClickableLabel * l6 = popupMenuWindow->addMenuItem("Give to");
          ClickableLabel * l7 = popupMenuWindow->addMenuItem("Oath to");
          ClickableLabel * l8 = popupMenuWindow->addMenuItem("Promote in stack");
          ClickableLabel * l9 = popupMenuWindow->addMenuItem("Stack to");
          ClickableLabel * l10 = popupMenuWindow->addMenuItem("Stance to");
          ClickableLabel * l11 = popupMenuWindow->addMenuItem("Swap with");
          ClickableLabel * l12 = popupMenuWindow->addMenuItem("Teach");
          ClickableLabel * l13 = popupMenuWindow->addMenuItem("Target");

          ::setCustomStyle(l1,BLUE_COLOR);
         ::setCustomStyle(l2,BLUE_COLOR);
         ::setCustomStyle(l3,BLUE_COLOR);
         ::setCustomStyle(l4,BLUE_COLOR);
         ::setCustomStyle(l5,BLUE_COLOR);
          ::setCustomStyle(l6,BLUE_COLOR);
         ::setCustomStyle(l7,BLUE_COLOR);
         ::setCustomStyle(l8,BLUE_COLOR);
         ::setCustomStyle(l9,BLUE_COLOR);
         ::setCustomStyle(l10,BLUE_COLOR);
          ::setCustomStyle(l11,BLUE_COLOR);
         ::setCustomStyle(l12,BLUE_COLOR);
         ::setCustomStyle(l13,BLUE_COLOR);

             
             connect(this,SIGNAL(closePopupMenu()),popupMenuWindow,SLOT(doClose()));            
             // Connect handlers
         connect(l1,SIGNAL(leftClicked(AbstractData*)),
                         this,SLOT(attackUnitSignalHandler(AbstractData*)));
         connect(l2,SIGNAL(leftClicked(AbstractData*)),
                         this,SLOT(acceptUnitSignalHandler(AbstractData*)));
         connect(l3,SIGNAL(leftClicked(AbstractData*)),
                         this,SLOT(bestowUnitSignalHandler(AbstractData*)));
         connect(l4,SIGNAL(leftClicked(AbstractData*)),
                         this,SLOT(ejectUnitSignalHandler(AbstractData*)));
         connect(l5,SIGNAL(leftClicked(AbstractData*)),
                         this,SLOT(getFromUnitSignalHandler(AbstractData*)));
         connect(l6,SIGNAL(leftClicked(AbstractData*)),
                         this,SLOT(giveToUnitSignalHandler(AbstractData*)));
        connect(l7,SIGNAL(leftClicked(AbstractData*)),
                         this,SLOT(oathSignalHandler(AbstractData*)));
         connect(l8,SIGNAL(leftClicked(AbstractData*)),
                         this,SLOT(promoteUnitSignalHandler(AbstractData*)));
         connect(l9,SIGNAL(leftClicked(AbstractData*)),
                         this,SLOT(stackToUnitSignalHandler(AbstractData*)));
          connect(l10,SIGNAL(leftClicked(AbstractData*)),
                         this,SLOT(stanceToUnitSignalHandler(AbstractData*)));
         connect(l11,SIGNAL(leftClicked(AbstractData*)),
                         this,SLOT(swapWithUnitSignalHandler(AbstractData*)));
         connect(l12,SIGNAL(leftClicked(AbstractData*)),
                         this,SLOT(teachUnitSignalHandler(AbstractData*)));
        connect(l13,SIGNAL(leftClicked(AbstractData*)),
                         this,SLOT(targetUnitSignalHandler(AbstractData*)));
              
             
             popupMenuWindow->setMyLayout();
             popupMenuWindow->move(parent_->mapFromGlobal(QCursor::pos()) + QPoint(20,30));
             popupMenuWindow->show();             
         }
     }
}

void ViewManager::attackUnitSignalHandler(AbstractData*)
{

     OrderWindow::showOrderWindow(tokenSelected_,"attack",this); 
     emit closePopupMenu();     
     return;
 }

void ViewManager::acceptUnitSignalHandler(AbstractData*)
{

     OrderWindow::showOrderWindow(tokenSelected_,"accept",this, ExtendedCommand::ACCEPT); 
     emit closePopupMenu();     
     return;
 }


void ViewManager::bestowUnitSignalHandler(AbstractData*)
{

     OrderWindow::showOrderWindow(tokenSelected_,"bestow",this); 
     emit closePopupMenu();     
     return;
 }

void ViewManager::ejectUnitSignalHandler(AbstractData*)
{

     OrderWindow::showOrderWindow(tokenSelected_,"eject",this, ExtendedCommand::EJECT); 
     emit closePopupMenu();     
     return;
 }




void ViewManager::getFromUnitSignalHandler(AbstractData*)
{

     OrderWindow::showOrderWindow(tokenSelected_,"get",this); 
     emit closePopupMenu();     
     return;
 }


void ViewManager::giveToUnitSignalHandler(AbstractData*)
{

     OrderWindow::showOrderWindow(tokenSelected_,"give",this); 
     emit closePopupMenu();     
     return;
 }


//void ViewManager::oathToUnitSignalHandler(AbstractData*)
//{
//
//     OrderWindow::showOrderWindow(tokenSelected_,"oath",this); 
//     emit closePopupMenu();     
//     return;
// }


void ViewManager::promoteUnitSignalHandler(AbstractData*)
{

     OrderWindow::showOrderWindow(tokenSelected_,"promote",this,ExtendedCommand::PROMOTE); 
     emit closePopupMenu();     
     return;
 }


void ViewManager::stackToUnitSignalHandler(AbstractData*)
{

     OrderWindow::showOrderWindow(tokenSelected_,"stack",this, ExtendedCommand::STACK); 
     emit closePopupMenu();     
     return;
 }


void ViewManager::stanceToUnitSignalHandler(AbstractData*)
{

     OrderWindow::showOrderWindow(tokenSelected_,"stance",this); 
     emit closePopupMenu();     
     return;
 }


void ViewManager::swapWithUnitSignalHandler(AbstractData*)
{

     OrderWindow::showOrderWindow(tokenSelected_,"swap",this); 
     emit closePopupMenu();     
     return;
 }


void ViewManager::teachUnitSignalHandler(AbstractData*)
{

     OrderWindow::showOrderWindow(tokenSelected_,"teach",this); 
     emit closePopupMenu();     
     return;
 }


void ViewManager::targetUnitSignalHandler(AbstractData*)
{

     OrderWindow::showOrderWindow(tokenSelected_,"target",this); 
     emit closePopupMenu();     
     return;
 }






void ViewManager::showConstructionPopupMenu(AbstractData * data)
{
    ConstructionEntity  * building = dynamic_cast<ConstructionEntity *>(data);
     {
         if(building)
         {
             setActiveData(building);
             if(building->mayMove())
             {
                 // this is vessel
             }
             // set active item
  //           tokenSelected_ = building;
            popupMenuWindow = new PopupMenuWindow(0,Qt::ToolTip);
             popupMenuWindow->clean();
             // Create buttons
          ClickableLabel * l1 = popupMenuWindow->addMenuItem("Attack");
          ClickableLabel * l2 = popupMenuWindow->addMenuItem("Enter");
          ClickableLabel * l3 = popupMenuWindow->addMenuItem("Exit");
          ClickableLabel * l4 = popupMenuWindow->addMenuItem("Target");
          ClickableLabel * l5 = popupMenuWindow->addMenuItem("Build");
          ::setCustomStyle(l1,BLUE_COLOR);
         ::setCustomStyle(l2,BLUE_COLOR);
         ::setCustomStyle(l3,BLUE_COLOR);
         ::setCustomStyle(l4,BLUE_COLOR);
         ::setCustomStyle(l5,BLUE_COLOR);
            
             connect(this,SIGNAL(closePopupMenu()),popupMenuWindow,SLOT(doClose()));            
             // Connect handlers
         connect(l1,SIGNAL(leftClicked(AbstractData*)),
                         this,SLOT(attackConstructionSignalHandler(AbstractData*)));
         connect(l2,SIGNAL(leftClicked(AbstractData*)),
                         this,SLOT(enterConstructionSignalHandler(AbstractData*)));
         connect(l3,SIGNAL(leftClicked(AbstractData*)),
                         this,SLOT(exitConstructionSignalHandler(AbstractData*)));
         connect(l4,SIGNAL(leftClicked(AbstractData*)),
                         this,SLOT(targetConstructionSignalHandler(AbstractData*)));             
          connect(l5,SIGNAL(leftClicked(AbstractData*)),
                         this,SLOT(buildConstructionSignalHandler(AbstractData*)));
             
            
             popupMenuWindow->setMyLayout();
             popupMenuWindow->move(parent_->mapFromGlobal(QCursor::pos()) + QPoint(20,30));
             popupMenuWindow->show();             
         }
     }
}
void ViewManager::attackConstructionSignalHandler(AbstractData*)
{

     OrderWindow::showOrderWindow(tokenSelected_,"attack",this); 
     emit closePopupMenu();     
     return;
 }
void ViewManager::enterConstructionSignalHandler(AbstractData*)
{

     OrderWindow::showOrderWindow(tokenSelected_,"enter",this); 
     emit closePopupMenu();     
     return;
 }
void ViewManager::exitConstructionSignalHandler(AbstractData*)
{

     OrderWindow::showOrderWindow(tokenSelected_,"exit",this,ExtendedCommand::EXIT);
     emit closePopupMenu();     
     return;
 }
void ViewManager::targetConstructionSignalHandler(AbstractData*)
{

     OrderWindow::showOrderWindow(tokenSelected_,"target",this);
     emit closePopupMenu();     
     return;
 }
void ViewManager::buildConstructionSignalHandler(AbstractData*)
{

     OrderWindow::showOrderWindow(tokenSelected_,"build",this); 
     emit closePopupMenu();     
     return;
 }



void ViewManager::showLocationPopupMenu(AbstractData * data)
{
    LocationEntity  * location = dynamic_cast<LocationEntity *>(data);
     {
         if(location)
         {
             setActiveData(location);
             // set active item
             locationSelected_ = location;
             popupMenuWindow = new PopupMenuWindow(0,Qt::ToolTip);
             popupMenuWindow->clean();
             // Create buttons
          ClickableLabel * l1 = popupMenuWindow->addMenuItem("Attack");
          ClickableLabel * l2 = popupMenuWindow->addMenuItem("Move to");
          ClickableLabel * l3 = popupMenuWindow->addMenuItem("March to");
          ClickableLabel * l4 = popupMenuWindow->addMenuItem("Caravan to");
          ClickableLabel * l5 = popupMenuWindow->addMenuItem("Target");
          ::setCustomStyle(l1,BLUE_COLOR);
         ::setCustomStyle(l2,BLUE_COLOR);
         ::setCustomStyle(l3,BLUE_COLOR);
         ::setCustomStyle(l4,BLUE_COLOR);
         ::setCustomStyle(l5,BLUE_COLOR);             
             connect(this,SIGNAL(closePopupMenu()),popupMenuWindow,SLOT(doClose()));            
             // Connect handlers
         connect(l1,SIGNAL(leftClicked(AbstractData*)),
                         this,SLOT(attackLocationSignalHandler(AbstractData*)));
         connect(l2,SIGNAL(leftClicked(AbstractData*)),
                         this,SLOT(moveToLocationSignalHandler(AbstractData*)));
         connect(l3,SIGNAL(leftClicked(AbstractData*)),
                         this,SLOT(marchToLocationSignalHandler(AbstractData*)));
         connect(l4,SIGNAL(leftClicked(AbstractData*)),
                         this,SLOT(caravanToLocationSignalHandler(AbstractData*)));
         connect(l5,SIGNAL(leftClicked(AbstractData*)),
                         this,SLOT(targetLocationSignalHandler(AbstractData*)));
             
             
             popupMenuWindow->setMyLayout();
             popupMenuWindow->move(parent_->mapFromGlobal(QCursor::pos()) + QPoint(20,30));
             popupMenuWindow->show();             
         }
     }
}
void ViewManager::attackLocationSignalHandler(AbstractData*)
{

     OrderWindow::showOrderWindow(tokenSelected_,"attack",this); 
     emit closePopupMenu();     
     return;
 }



void ViewManager::moveToLocationSignalHandler(AbstractData*)
{

     OrderWindow::showOrderWindow(tokenSelected_,"move",this); 
     emit closePopupMenu();     
     return;
 }
void ViewManager::marchToLocationSignalHandler(AbstractData*)
{

     OrderWindow::showOrderWindow(tokenSelected_,"march",this,ExtendedCommand::MARCH); 
     emit closePopupMenu();     
     return;
 }
void ViewManager::caravanToLocationSignalHandler(AbstractData*)
{

     OrderWindow::showOrderWindow(tokenSelected_,"caravan",this); 
     emit closePopupMenu();     
     return;
 }


void ViewManager::targetLocationSignalHandler(AbstractData*)
{

     OrderWindow::showOrderWindow(tokenSelected_,"target",this); 
     emit closePopupMenu();     
     return;
 }




void ViewManager::showFactionPopupMenu(AbstractData * data)
{
    FactionEntity  * faction = dynamic_cast<FactionEntity *>(data);
     {
         if(faction)
         {
             setActiveData(faction);
             // set active item
             factionSelected_ = faction;
             popupMenuWindow = new PopupMenuWindow(0,Qt::ToolTip);
             popupMenuWindow->clean();
             // Create buttons
          ClickableLabel * l1 = popupMenuWindow->addMenuItem("Attack");
          ClickableLabel * l2 = popupMenuWindow->addMenuItem("Stance to");
          ClickableLabel * l3 = popupMenuWindow->addMenuItem("Oath to");
         ::setCustomStyle(l1,BLUE_COLOR);
         ::setCustomStyle(l2,BLUE_COLOR);
         ::setCustomStyle(l3,BLUE_COLOR);
            
             connect(this,SIGNAL(closePopupMenu()),popupMenuWindow,SLOT(doClose()));            
             // Connect handlers
          connect(l2,SIGNAL(leftClicked(AbstractData*)),
                         this,SLOT(stanceSignalHandler(AbstractData*)));
         connect(l3,SIGNAL(leftClicked(AbstractData*)),
                         this,SLOT(oathSignalHandler(AbstractData*)));
         connect(l1,SIGNAL(leftClicked(AbstractData*)),
                         this,SLOT(attackFactionSignalHandler(AbstractData*)));
            
             
             popupMenuWindow->setMyLayout();
             popupMenuWindow->move(parent_->mapFromGlobal(QCursor::pos()) + QPoint(20,30));
             popupMenuWindow->show();             
         }
     }
}
void ViewManager::attackFactionSignalHandler(AbstractData*)
{

     OrderWindow::showOrderWindow(tokenSelected_,"attack",this); 
     emit closePopupMenu();     
     return;
 }
void ViewManager::stanceSignalHandler(AbstractData*)
{

     OrderWindow::showOrderWindow(tokenSelected_,"stance",this); 
     emit closePopupMenu();     
     return;
 }
void ViewManager::oathSignalHandler(AbstractData*)
{
     OrderWindow::showOrderWindow(tokenSelected_,"oath",this); 
     emit closePopupMenu();     
     return;
 }

//============================================================================

void ViewManager::showLocationInfo(AbstractData * data)
{
     LocationEntity * location = dynamic_cast< LocationEntity *> (data);
     if(location)
     {

     }
}
void ViewManager::showFactionInfo(AbstractData * data)
 {
    FactionEntity * faction = dynamic_cast< FactionEntity *> (data);
    if(faction)
    {

    }
 }

void ViewManager::showTokenInfo(AbstractData * data)
{
    TokenEntity * token = dynamic_cast< TokenEntity *> (data);
    if(!token)
    {
        return;
    }
    infoWindow = new InfoWindow(0,Qt::ToolTip);
    infoWindow->clean();
    if(!token->getDescription().empty() && token->getDescription() != string("No description"))
    {
      infoWindow->addWidget(new ClickableLabel(token->getDescription()));
    }

    ConstructionEntity * construction = dynamic_cast< ConstructionEntity *> (data);
    if(construction)
    {

        // Construction type
         vector <AbstractData * > v = construction->getConstructionType()->aPrint();
         for(vector<AbstractData * >::iterator iter = v.begin();
             iter != v.end(); ++iter)
         {
             //cout<<(*iter)->print()<< "(Token adding)"<<endl;
             infoWindow->addWidget(new ClickableLabel(*iter));
         }
         infoWindow->addWidget(new ClickableLabel(". "));


    }
    UnitEntity * unit = dynamic_cast< UnitEntity *> (data);
    if(unit)
    {


       // Race composition
        vector <AbstractData * > v = unit->getRaceComposition()->aPrint();
        for(vector<AbstractData * >::iterator iter = v.begin();
            iter != v.end(); ++iter)
        {
            //cout<<(*iter)->print()<< "(Token adding)"<<endl;
            infoWindow->addWidget(new ClickableLabel(*iter));
        }
        infoWindow->addWidget(new ClickableLabel(". "));
    }
        // Inventory
        if(!token->getAllInventory().empty())
        {
            infoWindow->addWidget(new ClickableLabel(" With: "));

           vector <AbstractArray> vec =unit->aPrintInventory();
            for(vector<AbstractArray >::iterator vecIter = vec.begin();
                vecIter != vec.end(); ++vecIter)
            {
                for(vector<AbstractData * >::iterator iter = (*vecIter).begin();
                    iter != (*vecIter).end(); ++iter)
                {
                    //cout<<(*iter)->print()<< "(Token adding)"<<endl;
                    infoWindow->addWidget(new ClickableLabel(*iter));
                }
                infoWindow->addWidget(new ClickableLabel(", "));
            }

        }


    infoWindow->setMyLayout();
    connect(this,SIGNAL(closeInfoWindow()),infoWindow,SLOT(close()));
    connect(infoWindow,SIGNAL(mouseOverSignal(AbstractData*)),
            this,SLOT(mouseOverSignalHandler(AbstractData*)));

    infoWindow->move(parent_->mapFromGlobal(QCursor::pos()) + QPoint(20,30));
    infoWindow->show();
    return ;
}

void ViewManager::showMapView()
{
    if(isOpened_)
    {
    center->setWidget(map);
    center->setWindowTitle("Map");
    center->setVisible(true);

    rightDock1->setWidget(locationView_);
    rightDock1->setWindowTitle("Location");
    rightDock1->setVisible(true);


    rightDock2->setWidget(factionView_);
    rightDock2->setWindowTitle("Faction");
    rightDock2->setVisible(true);
}
}



void ViewManager::showLocationView()
{
    if(isOpened_)
    {

    center->setWidget(locationView_);
    center->setWindowTitle("Location");
    center->setVisible(true);



    rightDock1->setWidget(map);
    rightDock1->setVisible(true);
    rightDock1->setWindowTitle("Map");

    rightDock2->setWindowTitle("Unit");
    rightDock2->setWidget(unitView_);
    rightDock2->setVisible(true);
}
}



void ViewManager::showUnitView()
{
    if(isOpened_)
    {

    center->setWidget(unitView_);
    center->setWindowTitle("Unit");
    center->setVisible(true);


    rightDock1->setWindowTitle("Location");
    rightDock1->setWidget(locationView_);
    rightDock1->setVisible(true);

    rightDock2->setWindowTitle("Map");
    rightDock2->setWidget(map);
    rightDock2->setVisible(true);
}
}



void ViewManager::showFactionView()
{
    if(isOpened_)
    {

    center->setWidget(factionView_);
    center->setWindowTitle("Faction");
    center->setVisible(true);


    rightDock1->setWindowTitle("Map");
    rightDock1->setWidget(map);
    rightDock1->setVisible(true);

    rightDock2->setWindowTitle("Unit");
    rightDock2->setWidget(unitView_);
    rightDock2->setVisible(true);
}
}


void ViewManager::close()
{
  if(isOpened_)
    {
    isOpened_ = false;
  factionView_->close();
  locationView_->close();
  map->close();
  unitView_->close();
  center->hide();
  rightDock1->hide();
  rightDock2->hide();
  leftDock->hide();
  //delete global;
  //delete map;
  //delete unit;
  //delete location;
    }
}



void ViewManager::showMousePos(int x, int y)
{
  parent_->statusText->setText(QString::number(x) + QString(",") + QString::number(y));
}

