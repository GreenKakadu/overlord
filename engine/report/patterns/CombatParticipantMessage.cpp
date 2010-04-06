/***************************************************************************
                          CombatParticipantMessage.cpp
                             -------------------
    begin                : Mar 22 2005
    copyright            : (C) 2005 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "CombatParticipantMessage.h"
#include "BattleInstance.h"
#include "TokenEntity.h"
#include "CombatFileVariety.h"
#include "CombatRankVariety.h"
//#include "InventoryElement.h"

extern string longtostr(long in);

CombatParticipantMessage::CombatParticipantMessage(TokenEntity * participant)
{
    participant_ = participant;
    report_ = string("");
    int num = participant->getFiguresNumber();

    report_ += longtostr(num) + " ";
    if (num == 1)
        report_ += participant->getComposition()->getName() + ", ";
    else
        report_ += participant->getComposition()->getPluralName() + ", ";



    report_ += participant->getCombatFile()->getName() + " ";
    report_ += participant->getCombatRank()->getName();
    if (participant->isFanatic())
        report_ += ", fanatic";
    if (participant->getBattleInstantiation()->isFlying())
        report_ += ", flying";
    report_ += ". ";
    // Report Inventory
    vector < InventoryElement> & inventory = participant->getAllInventory();
    bool introduceInventory = true;
    ItemRule * item;

    for (InventoryIterator iter = inventory.begin();
            iter != inventory.end(); iter++)
    {
        num = (*iter).getEquipedNumber();
        if (num <= 0)
            continue;

        if (introduceInventory)
        {
            report_ += "Equipment: ";
            introduceInventory = false;
        } else
        {
            report_ += ", ";
        }

        item = (*iter).getItemType();
        if (num == 1)
        {
            report_ += longtostr(num) + " " + item->getName();
        } else
        {
            report_ += longtostr(num) + " " + item->getPluralName();
        }
    }
    if (!introduceInventory)
    {
        report_ += ". ";
    }

    // Report Stats

}

void CombatParticipantMessage::printReport(ReportPrinter &out) const
{
    out << participant_->print() << ", ";
    out.incr_indent();
    out << report_;
    if (!participant_->getStats()->empty())
    {
        out << " Stats: " << *(participant_->getStats());
    }
    out << endl;
    out.decr_indent();
}

vector <AbstractData *> CombatParticipantMessage::aPrint()
{
    vector <AbstractData *> v;
    return v;
}




