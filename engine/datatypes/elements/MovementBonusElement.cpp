/***************************************************************************
                          MovementBonusElement.cpp
                             -------------------
    begin                : Feb 15 2005
    copyright            : (C) 2005 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "MovementBonusElement.h"
#include "GameFacade.h"
#include "IntegerData.h"
#include "StringData.h"
extern string longtostr(long u);

MovementBonusElement::MovementBonusElement(MovementVariety * mode, int bonusPoints)
{
	movementMode_ = mode;
	bonusPoints_ = bonusPoints;
}




MovementBonusElement MovementBonusElement::readElement (Parser * parser)
{
       MovementVariety * mode = gameFacade->movementModes[parser->getWord()];
        int bonusPoints = parser->getInteger();
        return MovementBonusElement(mode, bonusPoints);
}



string MovementBonusElement::print()
{
 if( bonusPoints_ >= 100)
   return string (1,'+') + longtostr(bonusPoints_ - 100)  + "% to "
	 	+ movementMode_->getName();
 else
   return string (1,'-') + longtostr(100 - bonusPoints_)  + "% to "
				+ movementMode_->getName();
}

vector <AbstractData *> MovementBonusElement::aPrintBonus()
{
    vector <AbstractData *> out;
    if( bonusPoints_ >= 100)
    {
        out.push_back(new StringData("+"));
        out.push_back(new IntegerData(bonusPoints_- 100));
    }


    else
    {
        out.push_back(new StringData("-"));
        out.push_back(new IntegerData(100 - bonusPoints_));
    }
    out.push_back(new StringData("% to "));
    out.push_back(new StringData(movementMode_->getName()));



    return out;
}

void MovementBonusElement::save(ostream & out)
{
	out << movementMode_->getTag() << " " <<  bonusPoints_   << endl;
}
