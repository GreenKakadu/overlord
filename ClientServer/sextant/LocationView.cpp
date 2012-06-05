#include "LocationView.h"
#include "UnitEntity.h"
#include "ConstructionEntity.h"
#include "FactionEntity.h"
#include "WeatherRule.h"
#include "ListViewer.h"
#include <QHBoxLayout>
#include "BasicExit.h"
#include "DirectionVariety.h"
#include "ClickableLabel.h"
#include "ClickableLine.h"
#include "SeasonRule.h"
#include "RaceRule.h"
#include "MarketStrategy.h"


LocationView::LocationView(LocationEntity * location, QWidget *parent)
{
    mainLayout = new QVBoxLayout;
     QPalette palette = this->palette();
    palette.setColor(QPalette::Window,QColor(228,218,228));
    this->setPalette(palette);
    updateLocation(location);
}

void LocationView::leftClickOnLocation(LocationEntity * location)
{
   emit leftClickedSignal(location);
}

void LocationView::updateLocation(LocationEntity * location)
{
    if(location == 0)
    {
        return;
    }
    this->clean();
    
    {
        ClickableLine * l1 = new ClickableLine(false,this);
        l1->addString(location->print());
        l1->addString(" (");
        l1->addItem(location->getTerrain());
        l1->addString("). ");
        FactionEntity * legalOwner = location->getLegalOwner();
        FactionEntity * realOwner = location->getRealOwner();
        if(legalOwner)
        {
            l1->addString("The lands of ");
            l1->addString(legalOwner->getName());
            l1->addString(". ");
        }
        if(legalOwner!= realOwner)
        {
            l1->addString("This land is occupied by ");
            l1->addString(realOwner->getName());
        }
        addWidget(l1);
    }
    // Description
    if(!location->getDescription().empty())
    {
        ClickableLine * l2 = new ClickableLine(false,this);
        l2->addString(location->getDescription());
        addWidget(l2);
    }    // Climate
    if(location->getSeason() ||location->getWeather() )
    {
        ClickableLine * weather = new ClickableLine(false,this);
        if(location->getSeason())
        {
            weather->addString("It is ");
            weather->addItem(location->getSeason());
            weather->addString(". ");
        }
        if(location->getWeather())
        {
            weather->addString("This turn weather was  ");
            weather->addItem(location->getWeather());
            if(location->getNextWeather())
            {
                weather->addString(" next turn it should be ");
                weather->addItem(location->getNextWeather());
            }
            weather->addString(".");
        }
        addWidget(weather);
    }



        int i=0;
// Location Titles
    //cout<<"Location "<<location->getTag();
    if(location->getTitles().getAll()->size()>0)
    {
        vector <AbstractArray> v = location->getTitles().aPrintAll();
        ClickableLine * titles = new ClickableLine(false,this);
        titles->addString("Titles:");
        ListViewer * lv1 = new ListViewer(titles,this);

        for (vector<AbstractArray>::iterator iter = v.begin();
        iter != v.end(); ++iter)
        {
            i++;
            lv1->addItem( new ClickableLine(*iter));
        }
        //cout<<"Titles i= "<<i<<endl;
        addWidget(lv1);
    }

// Exits
     ClickableLine * exits = new ClickableLine(false);
    exits->addString("Directions:");
    ListViewer * lv1 = new ListViewer(exits,this);
    i=0;
    for (vector<BasicExit *>::iterator iter = location->getAllExits().begin();
    iter != location->getAllExits().end(); ++iter)
    {
        i++;
        lv1->addItem( new ClickableLine((*iter)->aPrint()));
    }
    //cout<<"Exit: i= "<<i<<endl;
    addWidget(lv1);

 // Populatiuon, Wages, Taxes
    if(location->getPopulation())
      {
        ClickableLine * l2 = new ClickableLine(false,this);
        l2->addString("Population: ");
//        l2->addItem(new ClickableLabel(new IntegerData(location->getPopulation())));
        l2->addInteger(location->getPopulation());

        if(location->getRace())
        {
            l2->addString(" ");
            l2->addLabel(new ClickableLabel(location->getRace(),true));
            l2->addString(".");
        }
        l2->addString(" Wages: ");
        l2->addInteger(location->getWages());
        l2->addString(". ");
        l2->addString(" Tax income: $");
        l2->addInteger(location->getTaxes());
        l2->addString(". ");
        addWidget(l2);

//Market
        MarketStrategy * market =location->getMarket();
        if(market)
          {
            ClickableLine * l2 = new ClickableLine(false,this);
            l2->addString("Market: ");
            ListViewer * lv2 = new ListViewer(l2,this);

            vector<AbstractArray> v = market->aPrintRecruitReport();
            if(v.size()>0)
            {
                ClickableLine * l = new ClickableLine(true,this);
                l->addString("For hire: ");
                ListViewer * lv = new ListViewer(l,this);
                for(vector<AbstractArray>::iterator iter= v.begin(); iter != v.end(); iter++)
                  {
                    (*iter).insert((*iter).begin(),new StringData("    ") );
                      lv->addItem( new ClickableLine(*iter));
                  }
                lv2->addList(lv);
            }
            if(true)
            {
                vector<AbstractArray> v = market->aPrintSaleReport();
                if(v.size()>0)
                {
                    ClickableLine * l = new ClickableLine(true,this);
                    l->addString("For sale: ");
                    ListViewer * lv = new ListViewer(l,this);
                    for(vector<AbstractArray>::iterator iter= v.begin(); iter != v.end(); iter++)
                    {
                        (*iter).insert((*iter).begin(),new StringData("    ") );
                        lv->addItem( new ClickableLine(*iter));
                    }
                    lv2->addList(lv);
                }
            }

            if(true)
            {
                vector<AbstractArray> v = market->aPrintBuyReport();
                if(v.size()>0)
                {
                    ClickableLine * l = new ClickableLine(true,this);
                    l->addString("Buying: ");
                    ListViewer * lv = new ListViewer(l,this);
                    for(vector<AbstractArray>::iterator iter= v.begin(); iter != v.end(); iter++)
                    {
                        (*iter).insert((*iter).begin(),new StringData("    ") );
                        lv->addItem( new ClickableLine(*iter));
                    }
                    lv2->addList(lv);
                }
            }


//            vector<AbstractArray> v = market->aPrintReport();
//  //          cout<<"Location "<<location->getTag()<<" Market: "<< v.size()<<" items" <<endl;
//            for(vector<AbstractArray>::iterator iter= v.begin(); iter != v.end(); iter++)
//              {
//                  lv2->addItem( new ClickableLine(*iter));
//              }
            addWidget(lv2);
          }
    }
// Resources
    if(location->getResources().size()>0)
    {
        ClickableLine * resources = new ClickableLine(false,this);
        resources->addString("Resources:");
        ListViewer * lv2 = new ListViewer(resources,this);
        for (vector<ResourceElement *>::iterator iter = location->getResources().begin();
        iter != location->getResources().end(); ++iter)
        {
            lv2->addItem( new ClickableLine((*iter)->ResourceElement::aPrint()));
        }
        addWidget(lv2);

    }
// Skill teaching,
  if(!location->getAllSkillTeaching().empty())
    {
      ClickableLine * l2 = new ClickableLine(false,this);
      l2->addString("Skill Teaching: ");
      ListViewer * lv2 = new ListViewer(l2,this);
      //vector<AbstractArray> v
              vector <SkillLevelElement *> v= location->getAllSkillTeaching();
                cout<<"Location "<<location->getTag()<<"Skill Teaching : "<< v.size()<<" items" <<endl;
      for(vector<SkillLevelElement *>::iterator iter= v.begin(); iter != v.end(); iter++)
//          for(vector<AbstractArray>::iterator iter= v.begin(); iter != v.end(); iter++)
        {
            lv2->addItem( new ClickableLine((*iter)->aPrint()));
        }
      addWidget(lv2);

  }
// Skill bonuses
  if(!location->getAllSkillBonuses().isEmpty())
    {
      ClickableLine * l2 = new ClickableLine(false,this);
      l2->addString("Skill Bonuses: ");
      ListViewer * lv2 = new ListViewer(l2,this);
      vector<AbstractArray> v = location->getAllSkillBonuses().aPrintReport();
 //               cout<<"Location "<<location->getTag()<<"Skill Bonuses : "<< v.size()<<" items" <<endl;
      for(vector<AbstractArray>::iterator iter= v.begin(); iter != v.end(); iter++)
        {
            lv2->addItem( new ClickableLine(*iter));
        }
      addWidget(lv2);

  }

// Local items
    if(!location->getLocalItems().empty())
      {
        ClickableLine * l2 = new ClickableLine(false,this);
        l2->addString("Here are items laying on the ground: ");
        ListViewer * lv2 = new ListViewer(l2,this);

        for(ItemElementIterator iter= location->getLocalItems().begin(); iter != location->getLocalItems().end(); iter++)
          {
              lv2->addItem( new ClickableLine((*iter).ItemElement::aPrint()));
          }
        addWidget(lv2);
      }
//Units
    if(location->unitsPresent().size()>0)
    {
        ClickableLine * units = new ClickableLine("Units:",false,this);
        ListViewer * list = new ListViewer(units,this);
        for (vector<UnitEntity *>::iterator iter = location->unitsPresent().begin();
        iter != location->unitsPresent().end(); ++iter)
        {
            if( (*iter)->getContainingConstruction() != 0 )
            {
                continue;
            }
            if((*iter)->getLeader()==0)
            {
                recursiveShowStack(*iter,list);
            }
            else
            {
                    continue;
                }

 //           list->addItem( new ClickableLine((*iter)));

        }
        addWidget(list);
    }

//Buildings
    if(location->constructionsPresent().size()>0)
    {
        ClickableLine * buildings = new ClickableLine("Buildings:",false,this);
        ListViewer * lv4 = new ListViewer(buildings,this);
        for (vector<ConstructionEntity *>::iterator iter = location->constructionsPresent().begin();
        iter != location->constructionsPresent().end(); ++iter)
        {
            ClickableLine * buildingHeader = new ClickableLine();
            ClickableLine * construction = new ClickableLine();
            construction->addItem(*iter);
            construction->addString(" (");
            construction->addItem((*iter)->getConstructionType());
            construction->addString(").");

            if((*iter)->getAllUnits().size()>0) // Units inside
            {
                buildingHeader->addLabel(new ClickableLabel(QPixmap(":/new/prefix1/icons/redo.png"),16,16));
                buildingHeader->addItem(*iter);
                ListViewer * newList = new ListViewer(buildingHeader,0,true);
                for (vector<UnitEntity *>::iterator iter2 = (*iter)->getAllUnits().begin();
                iter2 != (*iter)->getAllUnits().end(); ++iter2)
                {
                    if((*iter2)->getLeader()==0)
                    {
                        recursiveShowStack(*iter2,newList);
                    }
                    else
                    {
                            continue;
                        }
                }
                lv4->addList(newList);
            }


            else
            {
                buildingHeader->addString("      ");
                buildingHeader->addItem(*iter);
                 lv4->addItem(buildingHeader);
            }
        }


        addWidget(lv4);
    }

//    mainLayout->setSpacing(0);
//    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
//    mainLayout->addItem(verticalSpacer);

    setScrolledLayout();

}


