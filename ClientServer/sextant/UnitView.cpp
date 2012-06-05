#include "UnitView.h"
#include "ListViewer.h"
#include <QHBoxLayout>
#include <sstream>
#include "ClickableLine.h"
#include "ClickableLabel.h"
#include "LocationEntity.h"
#include "TravelElement.h"
#include "RaceElement.h"
#include "SkillUseElement.h"
#include "ConstructionWorksElement.h"
#include "FactionEntity.h"
#include "GameFacade.h"
UnitView::UnitView(TokenEntity * token)
{

    mainLayout = new QVBoxLayout;
      QPalette palette = this->palette();
    palette.setColor(QPalette::Window,QColor(218,228,228));
    this->setPalette(palette);
   updateUnitView(token);
}

UnitView::~UnitView()
{
}


void UnitView::updateUnitView(TokenEntity * token)
{
   // cout<<"Got request on updateUnitView"<<endl;
    if(token == 0)
    {
        return;
    }
    clean();
    //addWidget(new ClickableLine(token,false,this));

    UnitEntity * unit = dynamic_cast<UnitEntity *>(token);
    if(unit != 0)
    {
        showUnitView(unit);
    }

    ConstructionEntity * construction = dynamic_cast<ConstructionEntity *>(token);
    if(construction != 0)
    {
        showConstructionView(construction);
    }


   setScrolledLayout();
}



void UnitView::showUnitView(UnitEntity * unit)
{
ClickableLine * line = new ClickableLine(unit,false,this);

if(unit->isDisbanded())
    {
        line->addLabel(new ClickableLabel("Disbanded."));
        addWidget(line);
        return;
    }
// List titles

 line->addItems(unit->getTitlesAttribute()->aPrintUnitTitles());
 if(unit->getFaction())
   {
       line->addString(" (belongs to ");
       line->addLabel(new ClickableLabel(unit->getFaction(),false));
       line->addString(")");
   }
// Current location
// bool isNewSentence = false;
    if(true)// Just to restrict visibility of  line variable
    {
        line->addString(" at ");
        line->addItem(unit->getLocation());
        if(unit->isMoving())
        {
            // Moving (Following, Inside construction)
            line->addItem(unit->getTravelStatus()->getMode());
            line->addString(" from ");
            line->addItem(unit->getTravelStatus()->getOrigin());
            line->addString(" to ");
            line->addItem(unit->getTravelStatus()->getDestination());

            line->addString(" (");
            int timeToGo = unit->getTravelStatus()->getTravelTime() - unit->getTravelStatus()->getRemainingTime();
            line->addInteger(timeToGo);
            line->addString(" of ");
            line->addInteger(unit->getTravelStatus()->getTravelTime());
            line->addString(" days)");
       }
        if(unit->getLeader())
        {
            line->addString(" following ");
            line->addItem(unit->getLeader());
        }
        if(unit->getContainingConstruction())
        {
            line->addString(" inside ");
            line->addItem(unit->getContainingConstruction());
        }
        line->addString(". ");
        addWidget(line);
    }
// Race composition
    {
        ClickableLine * line = new ClickableLine(unit->getRaceComposition()->aPrint(),false,this);
        addWidget(line);
    }

// Inventory
    if(!unit->getAllInventory().empty())
    {
       vector <AbstractArray> v = unit->aPrintInventory();
       ClickableLine * inventory = new ClickableLine(false,this);
       inventory->addString("Inventory:");
       ListViewer * lv1 = new ListViewer(inventory,this);
       for (vector<AbstractArray>::iterator iter = v.begin();
       iter != v.end(); ++iter)
       {
           lv1->addItem( new ClickableLine(*iter));
       }
       addWidget(lv1);
    }

// Skills
    if(!unit->getAllSkills().empty())
    {
       vector <AbstractArray> v = unit->aPrintSkills();
       ClickableLine * skills = new ClickableLine(false,this);
       skills->addString("Skills:");
       ListViewer * lv = new ListViewer(skills,this);
       for (vector<AbstractArray>::iterator iter = v.begin();
       iter != v.end(); ++iter)
       {
           lv->addItem( new ClickableLine(*iter));
       }
       addWidget(lv);
    }

// Enchantments
    if(!unit->getAllEnchantments().empty())
    {
       vector <AbstractArray> v = unit->aPrintEnchant();
       ClickableLine * enchant = new ClickableLine(false,this);
       enchant->addString("Enchantments:");
       ListViewer * lv = new ListViewer(enchant,this);
       for (vector<AbstractArray>::iterator iter = v.begin();
       iter != v.end(); ++iter)
       {
           lv->addItem( new ClickableLine(*iter));
       }
       addWidget(lv);
    }


// Stats (mana, upkeep)
    if(true)
    {
       vector <AbstractArray> v = unit->getStats()->aPrint();
       ClickableLine * stats = new ClickableLine(false,this);
       stats->addString("Stats:");
       ListViewer * lv = new ListViewer(stats,this);
       for (vector<AbstractArray>::iterator iter = v.begin();
       iter != v.end(); ++iter)
       {
           lv->addItem( new ClickableLine(*iter));
       }
       ClickableLine * upkeep = new ClickableLine(true,this);
       upkeep->addString("upkeep:");
       upkeep->addInteger(unit->getUpkeep());
       lv->addItem(upkeep);

       addWidget(lv);
    }
// Targeting
    if(unit->getTarget())
    {
        ClickableLine * line = new ClickableLine(false,this);
        line->addString(" Targeting ");
        line->addItem(unit->getTarget());
        line->addString(". ");
        addWidget(line);
    }
// Using skils
    if(!unit->getAllSkillUse().empty())
    {
       vector <SkillUseElement *> skillUsing = unit->getAllSkillUse();
       ClickableLine * head = new ClickableLine(false,this);
       head->addString("Using skills: ");
       ListViewer * lv = new ListViewer(head,this);

       for (vector<SkillUseElement *>::iterator iter = skillUsing.begin();
       iter != skillUsing.end(); ++iter)
       {
           lv->addItem( new ClickableLine((*iter)->aPrintSkillUse()));
       }
       addWidget(lv);
    }

// flags
    if(unit->isGuarding() || unit->isDiscontent() || unit->isConsuming())
    {
        ClickableLine * flags = new ClickableLine(false,this);
        if( unit->isConsuming())
        {
            flags->addString(" Will not use food for upkeep.");
        }
        if(unit->isGuarding())
        {
            flags->addString("On guard");
        }
        if( unit->isDiscontent())
        {
            flags->addString(" Discontent.");
        }

        addWidget(flags);
    }
    // May Learn
    {
        bool isFirst = true;
        ClickableLine * mayLearnLine;
         ListViewer * lv =0;
        for (RulesIterator skillIter = gameFacade->skills.begin(); skillIter != gameFacade->skills.end(); skillIter++)
        {
            SkillRule * skill = dynamic_cast<SkillRule*> (*skillIter);
            if (skill == 0)
                continue;


            if ((skill-> getRequirement(0) == 0) /*&& (getSkillLevel(skill) == 0)*/)
                continue;

            if (unit->mayStudySkill(skill))
            {

                    mayLearnLine = new ClickableLine(false,this);
                if (isFirst)
                {

                    mayLearnLine->addString("May learn: ");
                    lv = new ListViewer(mayLearnLine,this);
                    isFirst = false;

                }



                SkillLevelElement skillLevel = SkillLevelElement(skill,unit->getSkillLevel(skill) + 1);
                vector<AbstractData *> v = skillLevel.aPrintLevel();
                for(vector<AbstractData *>::iterator iter = v.begin(); iter != v.end(); ++iter)
                {
                  mayLearnLine= new ClickableLine(false,this);

                  mayLearnLine->addItem(new SkillLevelElement(skill,unit->getSkillLevel(skill) +1 ));
                }
                if (unit->mayStudyWithTeacher(skill))
                {
                    mayLearnLine->addString("(needs teacher)");
                }
                lv->addItem(mayLearnLine);
            }

        }
        addWidget(lv);
    }


//Events
    if (true)
    {
      ClickableLine * events = new ClickableLine(false, this);
      events->addString("Events: ");
      ListViewer * lv = new ListViewer(events, this);

      auto eventList = unit->getAllCollectedEvents();

      for (auto iter = eventList.begin(); iter != eventList.end(); iter++)
        {
          lv->addItem(new ClickableLine(*iter));
        }
      addWidget(lv);
    }
//Orders
       showOrders(unit);

        ClickableLine * test = new ClickableLine(false, this);
        test->addString("----------------------    The End.    ----------------------");
        ListViewer * tv = new ListViewer(test, this);
        addWidget(tv);
    
    
}
//===================================================================
void UnitView::showOrders(TokenEntity * token)
{
        ClickableLine * orders = new ClickableLine(false, this);
        orders->addString("Orders: ");
        ListViewer * lv = new ListViewer(orders, this);

        auto ordersList = token->getOrderList();

        for (auto iter = ordersList.begin(); iter != ordersList.end(); iter++) 
        {
//            stringstream ss;
//            (*iter)->printOrderLine(ss);
//            string s =ss.str(); 
//            s.erase (s.begin()+ s.length()-1); // Remove new line 
//            ClickableLabel * cl = ClickableLabel::makeClickableString(s);
//            ClickableLine * line = new ClickableLine(false);
//            line->addLabel(cl);
//            lv->addItem(line);
            lv->addItem(new ClickableLine(*iter,true));
        }
        addWidget(lv);    
}



void UnitView::showConstructionView(ConstructionEntity * construction)
{

    ClickableLine * line = new ClickableLine(construction,false,this);
    line->addString(" (");
    line->addItem(construction->getConstructionType());
    line->addString(").");

    if(construction->isDisbanded())
        {
            line->addString("Destroyed.");
            addWidget(line);
            return;
        }


    if(construction->getFaction())
      {
          line->addString(" (belongs to ");
          line->addLabel(new ClickableLabel(construction->getFaction(),false));
          line->addString(")");
      }
    if(!construction->isCompleted())
      {
          line->addString(" (unfinished).");
      }



    if(true)// Just to restrict visibility of  line variable
    {
        line->addString(" at ");
        line->addItem(construction->getLocation());
        if(construction->isMoving())
        {
            // Moving
            line->addItem(construction->getTravelStatus()->getMode());
            line->addString(" from ");
            line->addItem(construction->getTravelStatus()->getOrigin());
            line->addString(" to ");
            line->addItem(construction->getTravelStatus()->getDestination());

            line->addString(" (");
            int timeToGo = construction->getTravelStatus()->getTravelTime() - construction->getTravelStatus()->getRemainingTime();
            line->addInteger(timeToGo);
            line->addString(" of ");
            line->addInteger(construction->getTravelStatus()->getTravelTime());
            line->addString(" days)");
        }
        line->addString(". ");
        addWidget(line);
    }

        if(!construction->isCompleted())
          {
              ClickableLine * line = new ClickableLine(false,this);
              for(vector<ConstructionWorksElement *>::iterator iter = construction->getAllBuildingWorks().begin();
              iter != construction->getAllBuildingWorks().end();iter++)
                {
                  if(iter == construction->getAllBuildingWorks().begin())
                  {
                      line->addString("Needs ");
                }
                  else
                  {
                      line->addString(", ");
                }
                  line->addLabel(new ClickableLabel(new RationalNumber((*iter)->getWorkAmount())));
                  line->addString(" ");
                  line->addString((*iter)->getWorkType()->getName());
                }
              addWidget(line);
          }
        // Units inside
        if(construction->getAllUnits().size()>0)
        {
            ClickableLine * units = new ClickableLine("Units Inside:",false,this);
            ListViewer * list = new ListViewer(units,this);
            for (vector<UnitEntity *>::iterator iter = construction->getAllUnits().begin();
            iter != construction->getAllUnits().end(); ++iter)
            {
                if((*iter)->getLeader()==0)
                {
                    recursiveShowStack(*iter,list);
                }
                else
                {
                        continue;
                    }
            }
            addWidget(list);
        }
// Staff

        if(construction->getAllStaff().size()>0)
        {
            bool isFirst = true;
            ClickableLine * units = new ClickableLine("Operated by:",false,this);
            for (vector<UnitEntity *>::iterator iter = construction->getAllStaff().begin();
            iter != construction->getAllStaff().end(); ++iter)
            {
                if(isFirst)
                {
                    isFirst = false;
                }
                else
                {
                    units->addString(", ");
                }
                units->addItem(*iter);
            }
            units->addString(". ");


            //            ListViewer * list = new ListViewer(units,this);
            //            for (vector<UnitEntity *>::iterator iter = construction->getAllStaff().begin();
            //            iter != construction->getAllStaff().end(); ++iter)
            //            {
            //                ClickableLine * unitLine = new ClickableLine();
            //                //unitLine->makeOffset(offset *5);
            //                unitLine->addItem(new ClickableLabel(*iter));
            //                list ->addItem(unitLine );

            //            }
            //            addWidget(list);
                        addWidget(units);
        }

        if(construction->getConstructionType()->getMaxStaff())
          {
            ClickableLine * units = new ClickableLine("Total staff is ",false,this);
            units->addInteger(construction->getEffectiveStaff());
            units->addString(" of ");
            units->addInteger(construction->getConstructionType()->getMaxStaff());
            units->addString(". ");
            addWidget(units);
          }
// Capacity!

        if(construction->mayMove())
        {
            bool firstMode = true;
            int capacity;
            int weight = 0;
            ClickableLine * capacityLine = new ClickableLine("Weight ",false,this);
            capacityLine->addInteger(construction->calculateTotalWeight(weight));

            for(int i=0; i < gameFacade->movementModes.size(); i++)
            {
                capacity =  construction->getCapacity(i);
                if( capacity )
                {
                    if(firstMode)
                    {
                        capacityLine->addString(". Capacity ");
                        firstMode = false;
                    }
                    else
                    {
                        capacityLine->addString(", ");
                    }
                    capacityLine->addInteger(capacity);
                    capacityLine->addString(" (");
                    capacityLine->addString((gameFacade->movementModes[i])->getName());
                    capacityLine->addString(")");
                }
            }
            addWidget(capacityLine);
        }

        // Inventory
        if(!construction->getAllInventory().empty())
        {
           vector <AbstractArray> v = construction->aPrintInventory();
           ClickableLine * inventory = new ClickableLine(false,this);
           inventory->addString("Inventory:");
           ListViewer * lv1 = new ListViewer(inventory,this);
           for (vector<AbstractArray>::iterator iter = v.begin();
           iter != v.end(); ++iter)
           {
               lv1->addItem( new ClickableLine(*iter));
           }
           addWidget(lv1);
        }


    // Skills
        if(!construction->getAllSkills().empty())
        {
           vector <AbstractArray> v = construction->aPrintSkills();
           ClickableLine * skills = new ClickableLine(false,this);
           skills->addString("Skills:");
           ListViewer * lv = new ListViewer(skills,this);
           for (vector<AbstractArray>::iterator iter = v.begin();
           iter != v.end(); ++iter)
           {
               lv->addItem( new ClickableLine(*iter));
           }
           addWidget(lv);
        }

    // Enchantments
        if(!construction->getAllEnchantments().empty())
        {
           vector <AbstractArray> v = construction->aPrintEnchant();
           ClickableLine * enchant = new ClickableLine(false,this);
           enchant->addString("Enchantments:");
           ListViewer * lv = new ListViewer(enchant,this);
           for (vector<AbstractArray>::iterator iter = v.begin();
           iter != v.end(); ++iter)
           {
               lv->addItem( new ClickableLine(*iter));
           }
           addWidget(lv);
        }


    // Stats
        if(true)
        {
           vector <AbstractArray> v = construction->getStats()->aPrint();
           ClickableLine * stats = new ClickableLine(false,this);
           stats->addString("Stats:");
           ListViewer * lv = new ListViewer(stats,this);
           for (vector<AbstractArray>::iterator iter = v.begin();
           iter != v.end(); ++iter)
           {
               lv->addItem( new ClickableLine(*iter));
           }
//           ClickableLine * upkeep = new ClickableLine(true,this);
//           upkeep->addItem(new ClickableLabel("upkeep:"));
//           upkeep->addItem(new ClickableLabel(new IntegerData(construction->getUpkeep())));
//           lv->addItem(upkeep);

           addWidget(lv);
        }
    // Targeting
        if(construction->getTarget())
        {
            ClickableLine * line = new ClickableLine(false,this);
            line->addString(" Targeting ");
            line->addItem(construction->getTarget());
            line->addString(". ");
            addWidget(line);
        }

        if(construction->isPublic())
        {
            ClickableLine * flags = new ClickableLine(false,this);
            flags->addString("This is a public construction that anyone can enter.");
            addWidget(flags);
        }

    // flags
        if(construction->isGuarding() /*|| unit->isDiscontent() || unit->isConsuming()*/)
        {
            ClickableLine * flags = new ClickableLine(false,this);
//            if( unit->isConsuming())
//            {
//                flags->addString(" Will not use food for upkeep.");
//            }
            if(construction->isGuarding())
            {
                flags->addString("On guard");
            }
//            if( construction->isDiscontent())
//            {
//                flags->addString(" Discontent.");
//            }
//
            addWidget(flags);
        }
    //Events
        if(true)
        {
           ClickableLine * events = new ClickableLine(false,this);
           events->addString("Events: ");
           ListViewer * lv = new ListViewer(events,this);

    //       vector<ReportElement *>::iterator iter;
    //      for ( iter = collectedReports_.begin(); iter != collectedReports_.end(); iter++)
    //        {
    //                     (*iter)->printReport(out);
    //            }


           addWidget(lv);
        }

//Orders
       showOrders(construction);
}
