#include <QtGui/QMainWindow>
#include "FactionView.h"
#include "ListViewer.h"
#include "FactionEntity.h"
#include "ClickableLabel.h"
#include "ClickableLine.h"
#include "ReportElement.h"
FactionView::FactionView(FactionEntity * faction, QWidget *parent)
{
    mainLayout = new QVBoxLayout;

    //    faction_ = faction;
        customisePalette();
        updateFactionData(faction);


}

FactionView::~FactionView()
{
}



void FactionView::customisePalette()
{
    QPalette palette = this->palette();
    QBrush brush(QColor(228,228,218,255));
    brush.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Window, brush);
    palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
    //palette.setColor(QPalette::Window,QColor(228,228,108));
    //palette.setBrush(backgroundRole(), QColor(228,228,108));
    this->setPalette(palette);
    setStyleSheet("FactionView {background-image:url(:/new/prefix1/images/Green-Ivy-Scrolls.png);}");
}

void FactionView::updateFactionData(FactionEntity * faction)
{
    if(faction == 0)
    {
        return;
    }
    this->clean();

    ClickableLine * l1 = new ClickableLine(false,this);
    l1->addString("Game ");
    l1->addString("Alpha. ");
//    l1->addString();
    l1->addString("Report for turn ");
    l1->addString(QString::number(gameFacade->getGameTurn()));
    l1->addString(" for ");
    l1->addItem(faction);
    l1->addString(".");
   addWidget(l1);

    // Resigned
   if(faction->isResigned())
   {
       ClickableLine * lr = new ClickableLine("Resigned");
       addWidget(lr);
      setScrolledLayout();
      return;
   }
    // Faction Statistics
    // Funds
    ListViewer * lv = new ListViewer("Faction funds:",this);
    for (vector<ItemElement>::iterator iter = faction->getFunds().begin();
    iter != faction->getFunds().end(); ++iter)
    {
        if((*iter).getItemType()  == gameFacade->items["coin"])
        {
         ClickableLine * line =new ClickableLine();
         QPixmap icon(":/new/prefix1/images/gold-coins.png");
         line->addLabel(new ClickableLabel(icon,20,20));
         line->addLabel(new ClickableLabel("  "));
         line->addItems((*iter).aPrint());
         lv->addItem(line);
        }
        else
        {
            if((*iter).getItemType()  == gameFacade->items["food"])
            {
             ClickableLine * line =new ClickableLine();
             QPixmap icon(":/new/prefix1/images/bread.png");
             line->addLabel(new ClickableLabel(icon,20,20));
             line->addLabel(new ClickableLabel("  "));
             line->addItems((*iter).aPrint());
             lv->addItem(line);
            }
            else
         {
        lv->addItem(new ClickableLine((*iter).aPrint(),true));
        }
     }
    }

    addWidget(lv);
    // Control points
    ClickableLine * l2 = new ClickableLine(false,this);
    l2->addString("Control Points ");
    l2->addString(QString::number(faction->getControlPoints()));
    l2->addString(" of ");
    l2->addString(QString::number(faction->getMaxControlPoints()));
    l2->addString(".");
    addWidget(l2);
    QProgressBar * progressBar = new QProgressBar(this);
    progressBar->setRange(0,faction->getMaxControlPoints());
    progressBar->setValue(faction->getControlPoints());
    addWidget(progressBar);
    progressBar->show();

    // Stances
    ListViewer * lv2 = new ListViewer("Faction stances:",this);
    ClickableLine * l3 = new ClickableLine(true);
    l3->addString("Default stances:");
    l3->addItem(faction->getDefaultStance());
     lv2->addItem(l3);
    for (vector<StanceElement>::iterator iter = faction->getStances().begin();
    iter != faction->getStances().end(); ++iter)
    {
        lv2->addItem(new ClickableLine((*iter).aPrint(),true));
    }
    addWidget(lv2);

    // Events
    ListViewer * lv3 = new ListViewer("Global Events:",this);
    auto eventList = faction->getAllCollectedEvents();

    for (auto iter = eventList.begin(); iter != eventList.end(); iter++)
      {
        lv3->addItem(new ClickableLine(*iter));
      }
    addWidget(lv3);


    // Combat
    ListViewer * lv4 = new ListViewer("Combat:",this);
    addWidget(lv4);
    // Knowledge
    ListViewer * lv5 = new ListViewer("New Knowledge:",this);
    vector<Rule *> newKnowledge =faction->getAllKnowledge();
    vector<SkillLevelElement *> newSkillKnowledge =faction->getAllSkillKnowledge();
    for(vector<Rule *>::iterator iter = newKnowledge.begin(); iter != newKnowledge.end(); ++iter)
    {
        lv5->addItem(new ClickableLine(*iter));
    }
    for(vector<SkillLevelElement *>::iterator iter = newSkillKnowledge.begin(); iter != newSkillKnowledge.end(); ++iter)
    {
        lv5->addItem(new ClickableLine(*iter));
    }
     addWidget(lv5);
   // Orders

//     mainLayout->setSpacing(0);
//     QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
//     mainLayout->addItem(verticalSpacer);



    setScrolledLayout();
}

