/*
  This object is responsible for handling clicking, hovering over and drag-and-drop
  operation with active elements shown on Views (Map, Unit, Location and Faction)
*/
#include "ContextManager.h"
#include "MainClientWindow.h"
#include "CustomStyles.h"
#include "ClickableLabel.h"
#include "LocationEntity.h"
#include "FactionEntity.h"
#include "PopupMenuWindow.h"
#include "OrderWindow.h"
#include "InfoWindow.h"
ContextManager::ContextManager(MainClientWindow * parent):QObject(parent)
{
    parent_ = parent;
 selectedLocation = 0;
 popupMenuWindow =0;
 orderWindow=0;
}
void ContextManager::leftClickedSignalHandler(AbstractData * data)
{

}

void ContextManager::doubleClickedSignalHandler(AbstractData * data)
{

}

void ContextManager::rightClickedSignalHandler(AbstractData * data)
{
    if(data)
    {
//       if(infoWindow)//Clicked element already has info opened. Close it.
//        {
//            emit closeInfoWindow();
//        }
//          //cout << "ViewManager::Mouse right-clicked "<<data->print()<<endl;
//          showSkillPopupMenu(data);
          showItemPopupMenu(data);
    }

}

void ContextManager::mouseOverSignalHandler(AbstractData * data)
{

}

void ContextManager::clickedSignalHandler(AbstractData * data, QMouseEvent * event )
{

}


void ContextManager::showItemPopupMenu(AbstractData * data)
{
  ItemRule  * item = dynamic_cast<ItemRule *>(data);
  {
      if(item)
      {
          activeItem = item;
         //cout<<"ContextManager::showItemPopupMenu invoked "<< item->print()<<endl;
          popupMenuWindow = new PopupMenuWindow(0,Qt::ToolTip);
          popupMenuWindow->clean();
          //equip, unequip, buy, sell, give, use, drop
          ClickableLabel * l1 = popupMenuWindow->addMenuItem("Use");
          ClickableLabel * l2 = popupMenuWindow->addMenuItem("Buy");
          ClickableLabel * l3 = popupMenuWindow->addMenuItem("Sell");
          ClickableLabel * l4 = popupMenuWindow->addMenuItem("Equip");
          ClickableLabel * l5 = popupMenuWindow->addMenuItem("Unequip");
          ClickableLabel * l6 = popupMenuWindow->addMenuItem("Give");
          ClickableLabel * l7 = popupMenuWindow->addMenuItem("Get");
          ClickableLabel * l8 = popupMenuWindow->addMenuItem("Swap");

          //connect(this,SIGNAL(closeInfoWindow()),popupMenuWindow,SLOT(close()));

          connect(l1,SIGNAL(leftClicked(AbstractData*)),this,SLOT(useItemSignalHandler(AbstractData*)));
          connect(l6,SIGNAL(leftClicked(AbstractData*)),this,SLOT(giveItemSignalHandler(AbstractData*)));
          // Connect l2 - l8

          ::setCustomStyle(l1,BLUE_COLOR);
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

void ContextManager::useItemSignalHandler(AbstractData*) //Temp
{
    cout<<"ContextManager::useItemSignalHandler invoked"<<endl;
    popupMenuWindow->doClose();
}

void ContextManager::giveItemSignalHandler(AbstractData * data) //Temp
{

if(data)
{
    cout<<"ContextManager::giveItemSignalHandler"<<data->print() <<endl;
}
else
{
     cout<<"ContextManager::giveItemSignalHandler invoked"<<endl;
}



//        orderWindow = new OrderWindow(0,Qt::ToolTip);
        orderWindow = new OrderWindow(0,Qt::Popup | Qt::WindowTitleHint);
//        orderWindow = new OrderWindow(0,0x00001008);

        orderWindow->clean();
        orderWindow->setMyLayout();
        //orderWindow->setWindowTitle("Give");

       ClickableLabel * l1 = new ClickableLabel("Give ");
       orderWindow->addWidget(l1);



          QSpinBox * sb = orderWindow->createSpinBox(5,0,99);
          orderWindow->addWidget(sb);
          //QComboBox * cb = orderWindow->createComboBox("Item");
          // add actually item itself but show it's name only
          // next step - validation
          QComboBox * cb = orderWindow->createComboBox(activeItem->getName().c_str());

          cb->addItem("Item 1");
          cb->addItem("Item 2");
          cb->addItem("Item 3");
          orderWindow->addWidget(cb);

          orderWindow->addWidget(new ClickableLabel("  to "));

          QComboBox * cb1 = orderWindow->createComboBox("unit");
          cb1->addItem("Unit 1");
          cb1->addItem("Unit 2");
          cb1->addItem("Unit 3");
          orderWindow->addWidget(cb1);


  //        orderWindow->addWidget(new ClickableLabel("  another unit "));


      ::setCustomStyle(l1,ORDER_TEXT);
      ::setCustomStyle(cb,ORDER_COMBO);

        connect(this,SIGNAL(closeOrderWindow()),orderWindow,SLOT(close()));
//        connect(orderWindow,SIGNAL(mouseOverSignal(AbstractData*)),
//                this,SLOT(mouseOverSignalHandler(AbstractData*)));

        orderWindow->move(QPoint(500,800));
        orderWindow->show();







        popupMenuWindow->doClose();
}
