#include <QMouseEvent>
#include "AbstractData.h"
#include "Rule.h"
#include "ResourceElement.h"
#include "RaceElement.h"
#include "ItemElement.h"
#include "InventoryElement.h"
#include "Entity.h"
#include "ClickableLabel.h"
//#include "SkillCondition.h"
#include "SkillLevelElement.h"

ClickableLabel::ClickableLabel(QString text)
{
   setText(text);
   data_ = 0;
   setup();
}


ClickableLabel::ClickableLabel(const char * txt)
{
   setText(QString::fromAscii(txt));
   data_ = 0;
   setup();
}




ClickableLabel::ClickableLabel(string text)
{
  setText(QString::fromStdString(text));
  data_ = 0;
  setup();
}



ClickableLabel::ClickableLabel(int value)
{
    setText(QString::number(value));
    data_ = 0;
    setup();
}



ClickableLabel::ClickableLabel(QPixmap icon, int sizeX, int sizeY)
{
    setPixmap(icon.scaled(sizeX,sizeY));
    data_ = 0;
    setup();
}


ClickableLabel * ClickableLabel::makeClickableString(std::string txt)
{
    ClickableLabel * label = new ClickableLabel(txt);
    StringData * data = new StringData(txt);
    label->data_ = data;
    //label->setup();
}

ClickableLabel::ClickableLabel(AbstractData * data, bool isPlural)
{
    if(GameData * gData = dynamic_cast<GameData *>(data))
    {
        if(Entity * entity = dynamic_cast<Entity *>(data))
        {
            setText(QString::fromStdString(entity->print()));
            makeButton(LIGHT_BLUE_COLOR);
            data_ = data;
            setup(true);
            return;
        }

        if(Rule * rule = dynamic_cast<Rule *>(data))
        {
            if(isPlural)
            {
                setText(QString::fromStdString(rule->getPluralName()));
            }
            else
            {
                setText(QString::fromStdString(rule->getName()));
            }
            makeButton(LIGHT_GREEN_COLOR);
            data_ = data;
            setup(true);
            return;
        }
//        if(SkillCondition * condition = dynamic_cast<SkillCondition *>(data))
//        {
//            setText(QString::fromStdString(entity->print()));
//            makeBold();
//            data_ = data;
//            setup(true);
//            return;
//        }
        //cout <<gData->getName()<<endl;
        setText(QString::fromStdString(gData->getName()));
        data_ = data;
        setup(true);
        return;
    }

    ResourceElement * resource = dynamic_cast<ResourceElement *>(data);
    if(resource != 0)
    {
        if(resource->getParameter1() > 1 )
        {
            setText(QString::fromStdString(resource->getRule()->getPluralName()));
        }
        else
        {
            setText(QString::fromStdString(resource->getRule()->getName()));
        }
        makeButton();
        data_ = resource->getRule();
        setup(true);
        return;
    }

    RaceElement * race = dynamic_cast<RaceElement *>(data);
    if(race != 0)
    {
        if(race->getParameter1() > 1 )
        {
            setText(QString::fromStdString(race->getRule()->getPluralName()));
        }
        else
        {
            setText(QString::fromStdString(race->getRule()->getName()));
        }
        makeButton();
        data_ = race->getRule();
        setup(true);
        return;
    }

    ItemElement * element = dynamic_cast<ItemElement *>(data);
    if(element != 0)
    {
        if(element->getParameter1() > 1 )
        {
            setText(QString::fromStdString(element->getRule()->getPluralName()));
        }
        else
        {
            setText(QString::fromStdString(element->getRule()->getName()));
        }
        makeButton();
        data_ = element->getRule();
        setup(true);
        return;
    }

    InventoryElement * inventory = dynamic_cast<InventoryElement *>(data);
    if(inventory != 0)
    {
        if(inventory->getParameter1() > 1 )
        {
            setText(QString::fromStdString(inventory->getRule()->getPluralName()));
        }
        else
        {
            setText(QString::fromStdString(inventory->getRule()->getName()));
        }
        makeButton(YELLOW_COLOR);
        data_ = inventory->getRule();
        setup(true);
        return;
    }
    SkillLevelElement * skill = dynamic_cast<SkillLevelElement *>(data);
    if(skill != 0)
    {
        setText(QString::fromStdString(skill->getSkill()->printLevel(skill->getLevel())));
        makeButton();
        data_ = skill;
        setup(true);
        return;
    }

    setText(QString::fromStdString(data->print()));
    data_ = data;
    setup(true);
}

void ClickableLabel::mousePressEvent ( QMouseEvent * event )
{
    emit clicked(data_,event);
    if(event->button () == Qt::LeftButton)
    {
        emit leftClicked(data_);
    }
//    if(event->button () == Qt::RightButton)
//    {
//        emit rightClicked(data_);
//    }


}
void ClickableLabel::mouseReleaseEvent ( QMouseEvent * event )
{
    emit clicked(data_,event);
//    if(event->button () == Qt::LeftButton)
//    {
//        emit leftClicked(data_);
//    }
    if(event->button () == Qt::RightButton)
    {
         //cout<<"ClickableLabel::mouseReleaseEvent "<<event->type()<<endl;
        emit rightClicked(data_);
    }


}

void ClickableLabel::mouseDoubleClickEvent( QMouseEvent * event )
{
    emit doubleClicked(data_);
}


bool ClickableLabel::event ( QEvent * event )
{
    if(data_)
    {
           //cout<<"ClickableLabel::event "<<event->type()<<endl;
        switch(state_)
        {
        case NO_HOVERING:
            {
                switch(event->type())
                {
                case QEvent::HoverEnter:
                    {
                        if(timerId_ == 0) // Timer is not active
                        {
                            timerId_ = startTimer(1000);
                            //cout<<"ClickableLabel:startTimer"<<endl;
                            {
                                if(timerId_ ==0)
                                {
                                    cout<<"ClickableLabel: Could not start timer."<<endl;
                                }
                            }
                        }
                        state_ = ENTERING_HOVERING;
                        return true;

                    }
                case QEvent::Timer:
                    {
                        if(timerId_)
                        {
                            killTimer(timerId_);
                            timerId_ = 0;
                        }
                        return true;
                    }
                case QEvent::HoverLeave:
                default :
                    {
                        return QWidget::event(event);
                    }
                }
                break;
            }
        case ENTERING_HOVERING:
            {
                switch(event->type())
                {
                 case QEvent::HoverLeave:
                    {
                        if(timerId_)
                        {
                            killTimer(timerId_);
                            timerId_ = 0;
                        }
                        state_ = NO_HOVERING;
                        return true;
                    }
                case QEvent::Timer:
                    {
                        if(timerId_)
                        {
                           killTimer(timerId_);
                           timerId_ = 0;
                           emit mouseOver(data_);
                           state_ = HOVERING_IN;
                        }
                        return true;
                    }
                case QEvent::HoverEnter:
                default :
                    {
                        return QWidget::event(event);
                    }
                }
                break;
            }
        case EXITING_HOVERING:
            {
                switch(event->type())
                {
                case QEvent::HoverEnter:
                    {
                        if(timerId_)
                        {
                            killTimer(timerId_);
                            timerId_ = 0;
                        }
                        state_ = HOVERING_IN;
                        return true;

                    }
                case QEvent::Timer:
                    {
                        if(timerId_)
                        {
                           killTimer(timerId_);
                           timerId_ = 0;
                           emit mouseOver(0);
                           state_ =NO_HOVERING ;
                        }
                        return true;
                    }
                case QEvent::HoverLeave:
                default :
                    {
                        return QWidget::event(event);
                    }
                }
                break;
            }
        case HOVERING_IN:
            {
                switch(event->type())
                {
                case QEvent::HoverLeave:
                    {
                        if(timerId_ == 0) // Timer is not active
                        {
                            timerId_ = startTimer(500);
                            //cout<<"ClickableLabel:startTimer"<<endl;
                            {
                                if(timerId_ ==0)
                                {
                                    cout<<"ClickableLabel: Could not start timer."<<endl;
                                }
                            }
                        }
                        state_ = EXITING_HOVERING;
                        return true;
                    }
                case QEvent::Timer:
                    {
                        if(timerId_)
                        {
                            killTimer(timerId_);
                            timerId_ = 0;
                        }
                        return true;
                    }
                case QEvent::HoverEnter:
                default :
                    {
                        return QWidget::event(event);
                    }
                }
                break;
            }
        }
//        if(event->type() == QEvent::HoverEnter)
//        {
//            //cout<<"ClickableLabel:mouseOver Enter"<<endl;




//                    if(timerId_ == 0) // Timer is not active
//                    {
//                        timerId_ = startTimer(1000);
//                        //cout<<"ClickableLabel:startTimer"<<endl;
//                        {
//                            if(timerId_ ==0)
//                            {
//                                cout<<"ClickableLabel: Could not start timer."<<endl;
//                            }
//                        }
//                    }


//            return true;
//        }
//        if(event->type() == QEvent::HoverLeave)
//        {
//            //cout<<"ClickableLabel:mouseOver Leave"<<endl;

//            isMouseIn_ = false;
//            if(timerId_) // Timer is  active
//            {
//                killTimer(timerId_);
//                timerId_ = 0;
//            }
//            else
//            {
//                emit mouseOver(0);
//            }
//            return true;
//        }

//        if(event->type() == QEvent::Timer)
//        {
//            //cout<<"ClickableLabel:timeout "<< timerId_<<endl;

//            if(timerId_ == 0) // Timer is not active
//            {
//                return true;
//            }
//            else {
//                killTimer(timerId_);
//                timerId_ = 0;
//                emit mouseOver(data_);
//            }

//            return true;
//        }
    }
    return QWidget::event(event);
}

void ClickableLabel::setup(bool toSetIndent)
 {
    timerId_ =0;

    if(toSetIndent)
    {
    setIndent(0);
    }
    this->setMinimumSize(this->minimumSizeHint());
   setMouseTracking(true);          //?
   setAttribute(Qt::WA_Hover,true); //?

   state_ = NO_HOVERING;
 }




void ClickableLabel::makeButton(CustomStyle colorSet)
{
    QFont  font = this->font();
    font.setBold(true);
    setCustomStyle(this,colorSet);
    setFont(font);
}

